#include <stdio.h>
#include <stdlib.h>

int run_part1(FILE *fp)
{
  int dial = 50;
  int possible_passwords = 0;

  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fp) != -1)
  {
    if (line[0] == '\n' || line[0] == '\0')
      continue;

    char dir = line[0];

    int value = (int)strtol(line + 1, NULL, 10);

    dial += (dir == 'R') ? value : -value;
    dial = (dial + 100) % 100;

    if (dial == 0)
      possible_passwords++;
  }

  free(line);
  return possible_passwords;
}

int run_part2(FILE *fp)
{
  int dial = 50;
  int possible_passwords = 0;

  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fp) != -1)
  {
    if (line[0] == '\n' || line[0] == '\0')
      continue;

    char dir = line[0];

    int value = (int)strtol(line + 1, NULL, 10);

    for (int i = 0; i < value; i++)
    {
      if (dir == 'R')
        dial = (dial + 1) % 100;
      else
        dial = (dial - 1 + 100) % 100;
      if (dial == 0)
        possible_passwords++;
    }
  }

  free(line);
  return possible_passwords;
}

void process_file(const char *filename, const char *label)
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
  printf("%s: part1: %d part2:%d\n", label, result_part1, result_part2);

  fclose(fp);
}

int main(void)
{
  process_file("sample1.txt", "Sample 1 possible passwords");
  process_file("input.txt", "Possible passwords");

  return 0;
}
