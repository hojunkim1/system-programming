/* test_combine.c : optimize test by Hojun Kim. meringyee@gmail.com */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define IDENT 0
#define OP +

typedef long data_t;

typedef struct
{
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

vec_ptr new_vec(long len);
data_t *get_vec_start(vec_ptr v);
int get_vec_element(vec_ptr v, long index, data_t *dest);
long vec_length(vec_ptr v);

void combine1(vec_ptr v, data_t *dest);
void combine2(vec_ptr v, data_t *dest);
void combine3(vec_ptr v, data_t *dest);
void combine4(vec_ptr v, data_t *dest);
void combine5(vec_ptr v, data_t *dest);
void combine6(vec_ptr v, data_t *dest);
void combine7(vec_ptr v, data_t *dest);

void test_combine(void combine(vec_ptr, data_t *), vec_ptr v, data_t *dest);

int main()
{
    int i;
    vec_ptr v = new_vec(10000000);
    data_t result = 0;

    printf("Test: combine1\n");
    for (i = 0; i < 10; i++)
        test_combine(combine1, v, &result);

    printf("\nTest: combine2\n");
    for (i = 0; i < 10; i++)
        test_combine(combine2, v, &result);

    printf("\nTest: combine3\n");
    for (i = 0; i < 10; i++)
        test_combine(combine3, v, &result);

    printf("\nTest: combine4\n");
    for (i = 0; i < 10; i++)
        test_combine(combine4, v, &result);

    printf("\nTest: combine5\n");
    for (i = 0; i < 10; i++)
        test_combine(combine5, v, &result);

    printf("\nTest: combine6\n");
    for (i = 0; i < 10; i++)
        test_combine(combine6, v, &result);

    printf("\nTest: combine7\n");
    for (i = 0; i < 10; i++)
        test_combine(combine7, v, &result);

    return 0;
}

void test_combine(void combine(vec_ptr, data_t *), vec_ptr v, data_t *dest)
{
    struct timeval stime, etime, gap;

    gettimeofday(&stime, NULL);
    (*combine)(v, dest);
    gettimeofday(&etime, NULL);

    gap.tv_sec = etime.tv_sec - stime.tv_sec;
    gap.tv_usec = etime.tv_usec - stime.tv_usec;

    if (gap.tv_usec < 0)
    {
        gap.tv_sec = gap.tv_sec - 1;
        gap.tv_usec = gap.tv_usec + 1000000;
    }

    printf("Elapsed time %ldsec :%dusec\n", gap.tv_sec, gap.tv_usec);
}

/* Implementation with maximum use of data abstraction */
void combine1(vec_ptr v, data_t *dest)
{
    long i;

    *dest = IDENT;
    for (i = 0; i < vec_length(v); i++)
    {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

/* Move call to vec_length out of loop */
void combine2(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);

    *dest = IDENT;
    for (i = 0; i < length; i++)
    {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

data_t *get_vec_start(vec_ptr v)
{
    return v->data;
}

void combine3(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);

    *dest = IDENT;
    for (i = 0; i < length; i++)
    {
        *dest = *dest OP data[i];
    }
}

void combine4(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

void combine5(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < limit; i += 2)
    {
        acc = (acc OP data[i])OP data[i + 1];
    }

    for (; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

void combine6(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;

    for (i = 0; i < limit; i += 2)
    {
        acc0 = acc0 OP data[i];
        acc1 = acc1 OP data[i + 1];
    }

    for (; i < length; i++)
    {
        acc0 = acc0 OP data[i];
    }
    *dest = acc0 OP acc1;
}

void combine7(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < limit; i += 2)
    {
        acc = acc OP(data[i] OP data[i + 1]);
    }

    for (; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

/* Create vector of specified length */
vec_ptr new_vec(long len)
{
    /* Allocate header structure */
    vec_ptr result = (vec_ptr)malloc(sizeof(vec_rec));
    data_t *data = NULL;
    if (!result)
        return NULL; /* Couldn't allocate storage */
    result->len = len;
    /* Allocate array */
    if (len > 0)
    {
        data = (data_t *)calloc(len, sizeof(data_t));
        if (!data)
        {
            free((void *)result);
            return NULL; /* Couldn't allcate storage */
        }
    }
    /* Data will either be NULL or allocated array */
    result->data = data;
    return result;
}

/*
 * Retrieve vector element and store at dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec_ptr v, long index, data_t *dest)
{
    if (index < 0 || index >= v->len)
        return 0;
    *dest = v->data[index];
    return 1;
}

/* Return length of vector */
long vec_length(vec_ptr v)
{
    return v->len;
}
