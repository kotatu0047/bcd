// test3.c
#include <stdio.h>
#include "sample.h"
#include "sample2.h"

int a=0;
void hprint(const char* str)
{
  printf("hoge hoge %s\n", str);
  printf("a= %i\n", a);
  printf("hoge(a)= %i\n", hoge(a));
}