#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint64_t apply_op(uint64_t a, uint64_t b, char op)
{
  return (op == '+') ? a + b : a * b;
}

uint64_t reduce_column(const uint64_t *col, size_t n, char op)
{
  uint64_t acc = (op == '+') ? 0 : 1;
  for (size_t i = 0; i < n; i++)
    acc = apply_op(acc, col[i], op);
  return acc;
}

void transpose(void *src, void *dst, size_t elem_size, size_t rows, size_t cols)
{
  for (size_t r = 0; r < rows; r++)
    for (size_t c = 0; c < cols; c++)
      memcpy((char *)dst + elem_size * (c * rows + r),
             (char *)src + elem_size * (r * cols + c),
             elem_size);
}

uint64_t run_part1(FILE *fp)
{
  uint64_t total = 0;
  char *line = NULL;
  size_t cap = 0;

  uint64_t nums[4][1024];
  size_t rows = 0;

  while (getline(&line, &cap, fp) != -1)
  {
    size_t col = 0;

    for (char *tok = strtok(line, " \t\n");
         tok;
         tok = strtok(NULL, " \t\n"), col++)
    {

      if (tok[1] == '\0' && (*tok == '+' || *tok == '*'))
      {
        uint64_t column[4];
        for (size_t r = 0; r < rows; r++)
          column[r] = nums[r][col];

        total += reduce_column(column, rows, *tok);
      }
      else
      {
        nums[rows][col] = strtoul(tok, NULL, 10);
      }
    }
    rows++;
  }

  free(line);
  return total;
}

#define ROWS 8
#define COLS 4096

uint64_t run_part2(FILE *fp)
{

  uint64_t total = 0;

  char buf[ROWS][COLS] = {0};
  char buf_t[COLS][ROWS] = {0};

  for (size_t r = 0; r < ROWS && fgets(buf[r], COLS, fp); r++)
    ;

  transpose(buf, buf_t, sizeof(char), ROWS, COLS);

  uint64_t nums[8] = {0};
  size_t count = 0;
  char op = 0;

  for (size_t r = 0; r < COLS; r++)
  {
    sscanf(buf_t[r], "%lu %c", &nums[count], &op);

    if (nums[count])
    {
      count++;
      continue;
    }

    uint64_t sub = reduce_column(nums, count, op);
    if (!sub)
      break;

    total += sub;
    memset(nums, 0, sizeof(nums));
    count = 0;
    op = 0;
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

  uint64_t result_part1 = run_part1(fp);
  rewind(fp);
  uint64_t result_part2 = run_part2(fp);
  printf("Day %d %s: part1: %lld part2: %lld\n", day, filename, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", 6);
  process_file("input.txt", 6);

  return 0;
}
