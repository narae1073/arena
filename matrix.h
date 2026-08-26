// cache.h
#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

typedef float f32;
typedef uint32_t u32;

typedef struct {
    u32 rows;
    u32 cols;
    f32* data;
} matrix;

#define SIZE 2048

// cache.c에 구현되어 있는 함수 원형(Prototypes) 선언
f32 mat_sum0(const matrix *mat);
void mat_mul(matrix* out, const matrix* a, const matrix* b);

#endif // CACHE_H