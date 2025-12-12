#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint64_t max_k_subsequence(const char *s, int max_digits)
{
  int digit_count = 0;
  for (const char *p = s; *p; p++)
    if (*p >= '0' && *p <= '9')
      digit_count++;

  int to_remove = digit_count - max_digits;
  char stack[32];
  int top = 0;

  for (const char *p = s; *p; p++)
  {
    char c = *p;
    if (c < '0' || c > '9')
      continue;

    while (to_remove > 0 && top > 0 && stack[top - 1] < c)
    {
      top--;
      to_remove--;
    }

    if (top < max_digits)
      stack[top++] = c;
    else
      to_remove--;
  }

  uint64_t val = 0;
  for (int i = 0; i < max_digits; i++)
    val = val * 10 + (stack[i] - '0');

  return val;
}

uint64_t run_common(FILE *fp, int max_digits)
{
  uint64_t result_sum = 0;
  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fp) != -1)
  {
    result_sum += max_k_subsequence(line, max_digits);
  }

  free(line);
  return result_sum;
}

uint64_t run_part1(FILE *fp)
{
  return run_common(fp, 2);
}

uint64_t run_part2(FILE *fp)
{
  return run_common(fp, 12);
}

void process_file(const char *filename, const char *label)
{
  FILE *fp = fopen(filename, "r");
  if (!fp)
  {
    perror(filename);
    return;
  }

  uint64_t result_part1 = run_part1(fp);
  rewind(fp);
  uint64_t result_part2 = run_part2(fp);
  printf("%s: part1: %lld part2: %lld\n", label, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", "Sample 1 output joltage");
  process_file("input.txt", "Output joltage");

  return 0;
}
