#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int parse_line(char *line, ssize_t len) {
  char first = 0;
  char last = 0;

  for (int i = 0; i < len; i++) {
    char curr = line[i];
    if (!isdigit(curr))
      continue;

    if (first == 0)
      first = curr;

    last = curr;
  }

  int lhs = (int)first - '0';
  int rhs = (int)last - '0';

  return (lhs * 10) + rhs;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    perror("Usage: ./solution <filename>\n");
    return 1;
  }

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  int sum = 0;

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Retrieved line of length %zu:\n", read);
    printf("%s", line);
    sum += parse_line(line, read);
  }

  printf("\n");
  printf("Sum is: %d\n", sum);

  fclose(fp);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
