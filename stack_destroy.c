/* stack_destroy.c: 스택 구조 분석 2, 9월 19일, choijm@dku.edu */
#include <stdio.h>

void f1();
void f2();
void f3();

int main()
{
  f3();
  return 0;
}

void f1()
{
  int i;
  printf("In func1\n");
}

void f2()
{
  int j, *ptr;
  printf("f2 local: \t%p, \t%p\n", &j, &ptr);
  printf("In func2 \n");

  ptr = &j;
  *(ptr + 2) = f1;
}

void f3()
{
  printf("Before invoke f2()\n");
  f2();
  printf("After invoke f2()\n");
}
