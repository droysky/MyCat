#include "cat.h"

/*
(1) -b (GNU: --number-nonblank) - нумерует только непустые строки
(2) -e предполагает и -v (GNU only: -E то же самое, но без применения -v) -
также отображает символы конца строки как $
(3) -n (GNU: --number) - нумерует все выходные строки
(4) -s (GNU: --squeeze-blank) сжимает несколько смежных пустых строк
(5) -t предполагает и -v (GNU: -T то же самое, но без применения -v) также
отображает табы как ^I
*/

int main(int argc, char *argv[]) {
  flags options = {0};
  if ((CatParserFlags(argc, argv, &options)) == 1) {
    // optind индекс тeкущего параметра
    for (int i = optind; i < argc; i++) {
      CatReadFlags(i, argv, &options);
    }
  }
  return 0;
}

int CatParserFlags(int argc, char *argv[], flags *options) {
  int current_flag;
  // man 3 getopt_long
  struct option longopts[] = {{"number-nonblank", 0, NULL, 'b'},
                              {"number", 0, NULL, 'n'},
                              {"squeeze-blank", 0, NULL, 's'},
                              {NULL, 0, NULL, 0}};  // обозначаем конец

  while ((current_flag =
              getopt_long(argc, argv, "+bevnstTE", longopts, NULL)) != -1) {
    switch (current_flag) {
      case 'b':
        options->flag_b = 1;
        break;
      case 'e':
        options->flag_e = 1;
        options->flag_v = 1;
        break;
      case 'n':
        options->flag_n = 1;
        break;
      case 's':
        options->flag_s = 1;
        break;
      case 't':
        options->flag_t = 1;
        options->flag_v = 1;
        break;
      case 'v':
        options->flag_v = 1;
        break;
      case 'T':
        options->flag_t = 1;
        break;
      case 'E':
        options->flag_e = 1;
        break;
      default:
        fprintf(stderr, "cat: illegal option -- %c\n", current_flag);
        printf("usage: cat [-benstuv] [file ...]\n");
        break;
    }
    if (options->flag_n && options->flag_b) {
      options->flag_n = 0;
    }
  }
  return 1;
}

void CatReadFlags(int i, char *argv[], flags *options) {
  FILE *fp = fopen(argv[i], "r");
  if (fp == NULL) {
    fprintf(stderr, "cat: No such file or directory\n");
    return;
  }
  if (fp) {
    int line_count = 1;
    int line_empty = 0;
    int last = '\n';
    int ch;

    // size_t fread(void *buf, size_t size, size_t count, FILE *stream)
    while (fread(&ch, 1, 1, fp)) {
      //
      // flag -s сжимает несколько смежных пустых строк
      if (options->flag_s && ch == '\n' && last == '\n') {
        line_empty++;
        if (line_empty > 1) {
          continue;
        }
      } else {
        line_empty = 0;
      }

      // flag -b и flag -n - нумерация строк
      if (((options->flag_b && ch != '\n') || options->flag_n) &&
          last == '\n') {
        printf("%6d\t", line_count++);
      }

      //  flag -e - отображает символы конца строки как $
      if (options->flag_e && ch == '\n') {
        printf("$");
      }

      // flag -t - отображает табы как ^I
      if (options->flag_t && ch == '\t') {
        printf("^");
        ch = 'I';
      }

      if (options->flag_v) {
        if (ch >= 128 && ch < 160) {
          printf("M-");
        }
        if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32) ||
            (ch > 126 && ch < 160)) {
          printf("^");

          if (ch > 126 && ch < 160) {
            ch -= 64;
          } else {
            ch += 64;
          }
        } else if (ch == 127) {
          printf("^?");
        }
      }

      printf("%c", ch);
      last = ch;
    }

    fclose(fp);
  }
}
