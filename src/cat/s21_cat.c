#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGS_ERROR 2
#define FILE_ERROR 3

typedef struct {
  int b;
  int e;
  int v;
  int E;
  int s;
  int t;
  int T;
  int n;
} flags_t;

int process(char *argv[], flags_t *flag);
int get_flags(int argc, char **argv, flags_t *flags);

int main(int argc, char **argv) {
  if (argc < 2) return ARGS_ERROR;
  flags_t flags = {
      .b = 0, .e = 0, .v = 0, .E = 0, .s = 0, .t = 0, .T = 0, .n = 0};
  int rc;
  rc = get_flags(argc, argv, &flags);
  if (rc) return ARGS_ERROR;
  while (optind < argc) {
    rc = process(argv, &flags);
    if (rc) return FILE_ERROR;
    ++optind;
  }
  return EXIT_SUCCESS;
}

int get_flags(int argc, char **argv, flags_t *flags) {
  int cmd;
  const char all_flags[] = "bevEstTn";
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  while ((cmd = getopt_long(argc, argv, all_flags, long_options, NULL)) != -1) {
    switch (cmd) {
      case 'b':
        flags->b = 1;
        break;
      case 'E':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'T':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      default:
        return -1;
        break;
    }
    if (flags->b && flags->n) flags->n = 0;
  }
  if ((flags->t && !flags->v) || (flags->e && !flags->v)) return -1;
  return EXIT_SUCCESS;
}

int process(char *argv[], flags_t *flag) {
  FILE *f;
  f = fopen(argv[optind], "r");
  if (f == NULL) return -1;
  int str_num = 1;
  int empty_str_num = 0;
  int last = '\n';

  while (!feof(f)) {
    int c = fgetc(f);
    if (c == EOF) break;
    if (flag->s && c == '\n' && last == '\n') {
      ++empty_str_num;
      if (empty_str_num > 1) continue;
    } else {
      empty_str_num = 0;
    }

    if (last == '\n' && ((flag->b && c != '\n') || flag->n))
      printf("%6d\t", str_num++);
    if (flag->t && c == '\t') {
      printf("^");
      c = 'I';
    }
    if (flag->e && c == '\n') printf("$");
    if (flag->v && flag->t) {
      if (c == '\t') printf("^");
    }
    printf("%c", c);
    last = c;
  }

  fclose(f);
  return EXIT_SUCCESS;
}
