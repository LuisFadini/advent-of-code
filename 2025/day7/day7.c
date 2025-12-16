#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define MAX 256

int run_part1(FILE *fp)
{
  char row[MAX];
  bool beams[MAX] = {false};
  bool nextBeams[MAX] = {false};

  int splits = 0;
  int rowIndex = 0;
  int cols = 0;

  while (fgets(row, sizeof(row), fp))
  {
    int len = strlen(row);
    if (len > 0 && row[len - 1] == '\n')
    {
      row[len - 1] = '\0';
      len--;
    }
    cols = len;

    if (rowIndex == 0)
    {
      for (int c = 0; c < cols; c++)
      {
        if (row[c] == 'S')
        {
          beams[c] = true;
          break;
        }
      }
      rowIndex++;
      continue;
    }

    memset(nextBeams, false, sizeof(nextBeams));

    for (int c = 0; c < cols; c++)
    {
      if (!beams[c])
        continue;

      if (row[c] == '^')
      {
        splits++;

        if (c > 0)
          nextBeams[c - 1] = true;
        if (c < cols - 1)
          nextBeams[c + 1] = true;
      }
      else
      {
        nextBeams[c] = true;
      }
    }

    memcpy(beams, nextBeams, sizeof(beams));
    rowIndex++;
  }

  return splits;
}

uint64_t run_part2(FILE *fp)
{
  char row[MAX];
  uint64_t ways[MAX] = {0};
  uint64_t nextWays[MAX] = {0};

  int rowIndex = 0;
  int cols = 0;

  while (fgets(row, sizeof(row), fp))
  {
    int len = strlen(row);
    if (len > 0 && row[len - 1] == '\n')
    {
      row[len - 1] = '\0';
      len--;
    }
    cols = len;

    if (rowIndex == 0)
    {
      for (int c = 0; c < cols; c++)
      {
        if (row[c] == 'S')
        {
          ways[c] = 1;
          break;
        }
      }
      rowIndex++;
      continue;
    }

    memset(nextWays, 0, sizeof(nextWays));

    for (int c = 0; c < cols; c++)
    {
      if (ways[c] == 0)
        continue;

      if (row[c] == '^')
      {
        if (c > 0)
          nextWays[c - 1] += ways[c];
        if (c < cols - 1)
          nextWays[c + 1] += ways[c];
      }
      else
      {
        nextWays[c] += ways[c];
      }
    }

    memcpy(ways, nextWays, sizeof(ways));
    rowIndex++;
  }

  uint64_t total = 0;
  for (int c = 0; c < cols; c++)
  {
    total += ways[c];
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
  uint64_t result_part2 = run_part2(fp);
  printf("Day %d %s: part1: %d part2: %lld\n", day, filename, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", 7);
  process_file("input.txt", 7);

  return 0;
}
