/* f_pointer.c: for function pointer exercise, by choijm, choijm@dku.edu */
#include <stdio.h>

int a = 10;

int func1(int arg1)
{
    printf("In func1: arg1 = %d\n", arg1);
    return 0;
}

int main()
{
    int *pa;
    int (*func_ptr)(int);

    pa = &a;
    printf("pa = %p, *pa = %d\n", pa, *pa);
    func1(3);

    func_ptr = func1;
    func_ptr(5);

    printf("Bye..^^\n");
    return 0;
}
