#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_ARGS_ERROR 2
#define EXIT_FILE_OPEN_ERROR 3
#define EXIT_MEM_ERROR 4
#define EXIT_INPUT_ERROR 5

#define MAX_STR_LEN 2000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flags_t;

void check_stdio(char *str);
int regex_file_input(flags_t *flags, char *pattern, char *filename, int flag);
int get_flags(int argc, char **argv, flags_t *flags, char *pattern);
void process(flags_t *flags, FILE *f, regex_t reg, char *file, int flag);
int grep(flags_t *flags, int argc, char **argv, char *filename);
int exec_f_flag(char *pattern, char *filename, int flag);

int main(int argc, char **argv) {
  flags_t flags = {.e = 0,
                   .i = 0,
                   .v = 0,
                   .c = 0,
                   .l = 0,
                   .n = 0,
                   .h = 0,
                   .s = 0,
                   .f = 0,
                   .o = 0};
  char pattern[MAX_STR_LEN];
  if (argc < 2) return EXIT_ARGS_ERROR;
  if (argc == 2) check_stdio(argv[1]);

  int rc = get_flags(argc, argv, &flags, pattern);
  if (rc) return rc;
  rc = grep(&flags, argc, argv, pattern);
  if (rc) return rc;
  return EXIT_SUCCESS;
}

void check_stdio(char *str) {
  char tmp[MAX_STR_LEN];
  while (scanf("%s", tmp) == 1)
    if (strstr(tmp, str)) printf("%s\n", tmp);
}

int get_flags(int argc, char **argv, flags_t *flags, char *pattern) {
  if (strlen(argv[1]) < 2 || argv[1][0] != '-') return EXIT_SUCCESS;
  for (int i = 1; i < strlen(argv[1]); ++i) {
    switch (argv[1][i]) {
      case ('i'):
        ++flags->i;
        break;
      case ('e'):
        ++flags->e;
        snprintf(pattern, MAX_STR_LEN, "%s", argv[1]);
        break;
      case ('v'):
        ++flags->v;
        break;
      case ('c'):
        ++flags->c;
        break;
      case ('l'):
        ++flags->l;
        break;
      case ('n'):
        ++flags->n;
        break;
      case ('h'):
        ++flags->h;
        break;
      case ('s'):
        ++flags->s;
        break;
      case ('f'):
        ++flags->f;
        snprintf(pattern, MAX_STR_LEN, "%s", argv[2]);
        break;
      case ('o'):
        ++flags->o;
        break;
      default:
        return EXIT_ARGS_ERROR;
    }
  }

  if (flags->e > 1) return EXIT_ARGS_ERROR;
  if (flags->i > 1) return EXIT_ARGS_ERROR;
  if (flags->v > 1) return EXIT_ARGS_ERROR;
  if (flags->c > 1) return EXIT_ARGS_ERROR;
  if (flags->l > 1) return EXIT_ARGS_ERROR;
  if (flags->n > 1) return EXIT_ARGS_ERROR;
  if (flags->h > 1) return EXIT_ARGS_ERROR;
  if (flags->s > 1) return EXIT_ARGS_ERROR;
  if (flags->f > 1) return EXIT_ARGS_ERROR;
  if (flags->o > 1) return EXIT_ARGS_ERROR;

  return EXIT_SUCCESS;
}

void process(flags_t *flags, FILE *f, regex_t reg, char *file, int flag) {
  char text[MAX_STR_LEN];
  regmatch_t pmatch[1];
  int lines_num = 0;
  int count = 1;
  int res;
  char tmp[MAX_STR_LEN];
  int tmp_count = 0;

  while (fgets(text, MAX_STR_LEN - 1, f)) {
    res = 0;
    char *p = text;
    int rc = regexec(&reg, text, 1, pmatch, 0);
    if (!strchr(text, '\n')) strcat(text, "\n");
    if ((!rc && !flags->v) || (rc == REG_NOMATCH && flags->v)) res = 1;

    if (flag && res && !flags->c && !flags->l) printf("%s:", file);

    if (res && !flags->l && !flags->c && flags->n) printf("%d:", count);

    if (res && !flags->l && !flags->c && !flags->o) printf("%s", text);

    if (res && flags->o) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
        tmp[tmp_count++] = text[i];
      }
      printf("\n");
      tmp[tmp_count] = '\0';
      tmp_count = 0;
      while ((p = strstr(p, tmp))) {
        p += strlen(tmp);
        tmp_count++;
      }
      for (int i = 0; i < tmp_count - 1; i++) printf("%s\n", tmp);
    }

    lines_num += res;
    count++;
    text[0] = '\0';
    tmp[0] = '\0';
    tmp_count = 0;
  }

  if (flags->l && lines_num > 0) printf("%s\n", file);

  if (flags->c && !flags->l) {
    if (flag)
      printf("%s:%d\n", file, lines_num);
    else
      printf("%d\n", lines_num);
  }
}

int grep(flags_t *flags, int argc, char **argv, char *filename) {
  char pattern[MAX_STR_LEN];
  int rc;
  if (argv[1][0] == '-') ++optind;
  if (!flags->f && !flags->e)
    snprintf(pattern, MAX_STR_LEN, "%s", argv[optind++]);

  if (flags->f) {
    rc = exec_f_flag(pattern, filename, flags->s);
    ++optind;
    if (rc) return rc;
  }

  if (!flags->f && flags->e) snprintf(pattern, MAX_STR_LEN, "%s", filename);

  if (flags->e) ++optind;

  for (int i = optind; i < argc; i++) {
    int flag = 0;
    if ((argc - optind > 1) && !flags->h && !flags->l) flag = 1;
    regex_file_input(flags, pattern, argv[i], flag);
  }

  return EXIT_SUCCESS;
}

int exec_f_flag(char *pattern, char *filename, int flag) {
  FILE *f;
  int i = 0;
  f = fopen(filename, "r");
  if (!f) {
    if (!flag) printf("s21_grep: %s: No such file\n", filename);
    return EXIT_FILE_OPEN_ERROR;
  }
  int ch;
  while ((ch = fgetc(f)) != EOF) {
    pattern[i++] = ch;
    if (i >= MAX_STR_LEN) return EXIT_FAILURE;
  }
  pattern[i] = '\0';
  fclose(f);
  return EXIT_SUCCESS;
}

int regex_file_input(flags_t *flags, char *pattern, char *filename, int flag) {
  int mode = (!flags->i) ? REG_EXTENDED : REG_ICASE;
  regex_t exp;
  FILE *f;
  f = fopen(filename, "r");
  if (!f) {
    if (!flags->s) printf("s21_grep: %s: No such file\n", filename);
    return EXIT_FILE_OPEN_ERROR;
  }
  if (flag && !flags->h && !flags->l && !flags->c && !flags->n && !flags->o)
    printf("%s\n", filename);
  regcomp(&exp, pattern, mode);
  process(flags, f, exp, filename, flag);
  regfree(&exp);
  fclose(f);
  return EXIT_SUCCESS;
}
