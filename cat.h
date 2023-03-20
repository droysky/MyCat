#ifndef C3_SIMPLEBASHUTILS_0_S21_CAT_H
#define C3_SIMPLEBASHUTILS_0_S21_CAT_H

#include <getopt.h>  // man 3 getopt
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
} flags;

int CatParserFlags(int argc, char *argv[], flags *options);
void CatReadFlags(int i, char *argv[], flags *options);

#endif  // C3_SIMPLEBASHUTILS_0_S21_CAT_H
