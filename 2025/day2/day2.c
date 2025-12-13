#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint64_t run_part1(FILE *fp)
{
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  rewind(fp);

  char *sequences = malloc(file_size + 1);
  size_t n = fread(sequences, 1, file_size, fp);
  sequences[n] = '\0';

  uint64_t invalid_sum = 0;

  char *range = strtok(sequences, ",");
  while (range)
  {
    uint64_t start, end;
    if (sscanf(range, "%lld-%lld", &start, &end) == 2)
    {
      for (uint64_t num = start; num <= end; num++)
      {
        char buf[32];
        sprintf(buf, "%lld", num);

        int len = strlen(buf);
        if (len % 2 != 0)
          continue;

        int mid = len / 2;
        if (strncmp(buf, buf + mid, mid) == 0)
          invalid_sum += num;
      }
    }

    range = strtok(NULL, ",");
  }

  free(sequences);
  return invalid_sum;
}

uint64_t run_part2(FILE *fp)
{
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  rewind(fp);

  char *sequences = malloc(file_size + 1);
  size_t n = fread(sequences, 1, file_size, fp);
  sequences[n] = '\0';

  uint64_t invalid_sum = 0;

  char *range = strtok(sequences, ",");
  while (range)
  {
    uint64_t start, end;
    sscanf(range, "%lld-%lld", &start, &end);

    for (uint64_t num = start; num <= end; num++) {

        char s[32];
        sprintf(s, "%lld", num);
        int len = strlen(s);

        int valid = 0;

        for (int p = 1; p <= len/2 && !valid; p++) {
            if (len % p) continue;

            int ok = 1;
            for (int i = p; i < len; i++)
                if (s[i] != s[i % p]) { ok = 0; break; }

            if (ok) valid = 1;
        }

        if (valid)
            invalid_sum += num;
    }

    range = strtok(NULL, ",");
  }

  free(sequences);
  return invalid_sum;
}

void process_file(const char *filename, const int day)
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
  printf("Day %d %s:part1: %lld part2: %lld\n", day, filename, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", 2);
  process_file("input.txt", 2);

  return 0;
}
