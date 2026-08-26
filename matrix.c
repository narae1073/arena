#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "matrix.h"


// 0.56 sec
f32 mat_sum0(const matrix *mat)
{
    f32 sum = 0.0f;

    for (u32 row = 0; row < mat->rows; row++) {
        for (u32 col = 0; col < mat->cols; col++) {
            sum += mat->data[col + row * mat->cols];
        }
    }

    return sum;
}

// 1.45 sec
f32 mat_sum1(const matrix *mat)
{
    f32 sum = 0.0f;

    for (u32 col = 0; col < mat->cols; col++) {
        for (u32 row = 0; row < mat->rows; row++) { 
            sum += mat->data[col + row * mat->cols];
        }
    }

    return sum;
}


/*
i, j, k: 2.5 sec
i, k, j: 0.15 sec
k, j, i: 4.65 sec
j, k, i: 5.2 sec
j, i, k: 2.3 sec
*/

void mat_mul(matrix* out, const matrix* a, const matrix* b) {
    out->rows = a->rows;
    out->cols = b->cols;

    memset(out->data, 0, sizeof(f32) * out->rows * out->cols);

    for (u32 i = 0; i < out->rows; i++) {
        for (u32 k = 0; k < a->cols; k++)  {
            for (u32 j = 0; j < out->cols; j++) {
                out->data[j + i * out->cols] +=  
                    a->data[k + i * a->cols] *
                    b->data[j + k * b->cols];
            }
        }
    }
}


// int main(void)
// {
//     // 시드 초기화
//     struct timespec ts;
//     timespec_get(&ts, TIME_UTC);
//     srand((unsigned int)ts.tv_nsec);

//     matrix a = { SIZE, SIZE };
//     matrix b = { SIZE, SIZE };
//     matrix c = { SIZE, SIZE };
//     a.data = (f32*)malloc(a.rows * a.cols * sizeof(f32)); 
//     b.data = (f32*)malloc(b.rows * b.cols * sizeof(f32)); 
//     c.data = (f32*)malloc(c.rows * c.cols * sizeof(f32)); 

//     for (u32 i = 0; i < SIZE * SIZE; i++) {
//         // rand()는 0부터 RAND_MAX사이의 임의의 정수. 
//         // RAND_MAX는 rand()의 가장 큰 정수값.
//         a.data[i] = (f32)rand() / (f32)RAND_MAX;
//     }
//     for (u32 i = 0; i < SIZE * SIZE; i++) {
//         // rand()는 0부터 RAND_MAX사이의 임의의 정수. 
//         // RAND_MAX는 rand()의 가장 큰 정수값.
//         b.data[i] = (f32)rand() / (f32)RAND_MAX;
//     }

//     mat_mul(&c, &a, &b);

//     f32 sum = 0.0f;
//     sum = mat_sum0(&c);
//     printf("%f\n", sum);

//     free(a.data);
//     free(b.data);
//     free(c.data);
//     return 0;
// }