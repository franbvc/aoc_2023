#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NEW_NUMBER_STRING(name, val) {name, val, sizeof name - 1}

struct NumberString {
  const char *name;
  int value;
  ssize_t nameLen;
};

const struct NumberString numbers[9] = {
    NEW_NUMBER_STRING("one", 1),   NEW_NUMBER_STRING("two", 2),
    NEW_NUMBER_STRING("three", 3), NEW_NUMBER_STRING("four", 4),
    NEW_NUMBER_STRING("five", 5),  NEW_NUMBER_STRING("six", 6),
    NEW_NUMBER_STRING("seven", 7), NEW_NUMBER_STRING("eight", 8),
    NEW_NUMBER_STRING("nine", 9),
};

int is_number_string(char *line, ssize_t pos, ssize_t len,
                     struct NumberString ns) {
  if (ns.nameLen > (len - pos))
    return 0;

  for (int i = 0; i < ns.nameLen; i++) {
    if (line[pos] != ns.name[i])
      return 0;
    pos++;
  }

  return 1;
}

int parse_digit(char *line, ssize_t *pos, ssize_t len) {
  if (*pos >= len)
    return -1;

  char curr = line[*pos];
  if (isdigit(curr))
    return (int)curr - '0';

  for (int i = 0; i < 9; i++) {
    if (is_number_string(line, *pos, len, numbers[i])) {
      // *pos += (ssize_t)numbers[i].nameLen - 1;
      *pos += 1;
      return numbers[i].value;
    }
  }

  return -1;
}

int parse_line(char *line, ssize_t len) {
  int first = 0;
  int last = 0;

  for (ssize_t i = 0; i < len; i++) {
    int val = parse_digit(line, &i, len);
    if (val == -1)
      continue;

    if (first == 0)
      first = val;

    last = val;
  }

  printf("First: %d, Last: %d\n", first, last);

  return (first * 10) + last;
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
    // printf("Parsed number: %d\n", parse_line(line, read));
    sum += parse_line(line, read);
  }

  printf("\n");
  printf("Sum is: %d\n", sum);

  fclose(fp);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
