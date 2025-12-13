#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tileType
{
  Empty,
  PaperRoll,
  RemovedRoll
};

int dirs[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

struct mapCoordinate
{
  int x;
  int y;
  enum tileType tileType;
};

struct commonReturn
{
  struct mapCoordinate **mapWithRemovedRolls;
  int rows;
  int cols;
  int count;
};

struct mapCoordinate **make_map(FILE *fp, int *outRows, int *outCols)
{
  char *line = NULL;
  size_t len = 0;
  int rows = 0;
  int cols = 0;

  struct mapCoordinate **map = NULL;

  while (getline(&line, &len, fp) != -1)
  {
    if (cols == 0)
      cols = strcspn(line, "\n");

    map = realloc(map, sizeof(struct mapCoordinate *) * (rows + 1));
    map[rows] = malloc(sizeof(struct mapCoordinate) * cols);

    for (int i = 0; i < cols; i++)
    {
      char c = line[i];
      enum tileType tile = (c == '@') ? PaperRoll : Empty;

      map[rows][i].x = i;
      map[rows][i].y = rows;
      map[rows][i].tileType = tile;
    }

    rows++;
  }

  free(line);

  *outRows = rows;
  *outCols = cols;

  return map;
}

struct commonReturn run_common(struct mapCoordinate **map, int rows, int cols)
{
  struct commonReturn ret;
  ret.rows = rows;
  ret.cols = cols;
  ret.count = 0;

  ret.mapWithRemovedRolls = malloc(sizeof(struct mapCoordinate *) * rows);
  for (int r = 0; r < rows; r++)
  {
    ret.mapWithRemovedRolls[r] = malloc(sizeof(struct mapCoordinate) * cols);
    memcpy(ret.mapWithRemovedRolls[r], map[r], sizeof(struct mapCoordinate) * cols);
  }

  for (int r = 0; r < rows; r++)
  {
    for (int c = 0; c < cols; c++)
    {
      if (map[r][c].tileType != PaperRoll)
        continue;

      int neighbour_count = 0;

      for (int d = 0; d < 8; d++)
      {
        int rr = r + dirs[d][0];
        int cc = c + dirs[d][1];

        if (rr >= 0 && rr < rows &&
            cc >= 0 && cc < cols &&
            map[rr][cc].tileType == PaperRoll)
        {
          neighbour_count++;
        }
      }

      if (neighbour_count < 4)
      {
        ret.mapWithRemovedRolls[r][c].tileType = RemovedRoll;
        ret.count++;
      }
    }
  }

  return ret;
}

int run_part1(FILE *fp)
{
  int rows, cols;
  struct mapCoordinate **map = make_map(fp, &rows, &cols);

  struct commonReturn result = run_common(map, rows, cols);

  for (int r = 0; r < rows; r++)
    free(map[r]);
  free(map);

  return result.count;
}

int run_part2(FILE *fp)
{
  int rows, cols;
  struct mapCoordinate **map = make_map(fp, &rows, &cols);

  int total = 0;

  while (1)
  {
    struct commonReturn out = run_common(map, rows, cols);
    if (out.count == 0)
    {
      for (int r = 0; r < rows; r++)
        free(out.mapWithRemovedRolls[r]);
      free(out.mapWithRemovedRolls);
      break;
    }

    total += out.count;

    for (int r = 0; r < rows; r++)
      free(map[r]);
    free(map);

    map = out.mapWithRemovedRolls;
  }

  return total;
}

void process_file(const char *filename, const int day)
{
  FILE *fp = fopen(filename, "r");
  if (!fp)
  {
    perror(filename);
    return;
  }

  int result_part1 = run_part1(fp);
  rewind(fp);
  int result_part2 = run_part2(fp);
  printf("Day %d %s:part1: %d part2: %d\n", day, filename, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", 4);
  process_file("input.txt", 4);

  return 0;
}
