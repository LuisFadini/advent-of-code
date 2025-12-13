#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
  uint64_t start, end;
} Range;

int run_part1(FILE *fp)
{
  Range ranges[1024];
  size_t range_count = 0;
  char line[128];

  while (fgets(line, sizeof line, fp) && line[0] != '\n')
  {
    sscanf(line, "%lld-%lld",
           &ranges[range_count].start,
           &ranges[range_count].end);
    range_count++;
  }

  int fresh = 0;
  uint64_t id;

  while (fscanf(fp, "%lld", &id) == 1)
  {
    for (size_t i = 0; i < range_count; i++)
    {
      if (id >= ranges[i].start && id <= ranges[i].end)
      {
        fresh++;
        break;
      }
    }
  }

  return fresh;
}

uint64_t run_part2(FILE *fp)
{
  Range *ranges = NULL;
  size_t count = 0, cap = 0;
  char line[128];

  while (fgets(line, sizeof line, fp))
  {
    Range r;
    if (sscanf(line, "%lld-%lld", &r.start, &r.end) != 2)
      break;

    if (count == cap)
    {
      cap = cap ? cap * 2 : 16;
      ranges = realloc(ranges, cap * sizeof *ranges);
    }
    ranges[count++] = r;
  }

  int merged;
  do
  {
    merged = 0;

    for (size_t i = 0; i < count; i++)
    {
      for (size_t j = i + 1; j < count;)
      {

        if (ranges[i].start <= ranges[j].end &&
            ranges[i].end >= ranges[j].start)
        {

          if (ranges[j].start < ranges[i].start)
            ranges[i].start = ranges[j].start;
          if (ranges[j].end > ranges[i].end)
            ranges[i].end = ranges[j].end;

          ranges[j] = ranges[count - 1];
          count--;

          merged = 1;
        }
        else
        {
          j++;
        }
      }
    }
  } while (merged);

  uint64_t total = 0;
  for (size_t i = 0; i < count; i++)
    total += ranges[i].end - ranges[i].start + 1;

  free(ranges);
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
  printf("Day %d %s:part1: %d part2: %lld\n", day, filename, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", 5);
  process_file("input.txt", 5);

  return 0;
}
