#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.c" // 앞서 작성한 arena 관련 코드들이 들어있는 파일

typedef struct Node {
    u64 id;
    f64 val;
    struct Node* next;
} Node; // 24바이트짜리 작은 노드 구조체

#define NODE_COUNT 1000000 // 100만 개
#define ITERATIONS 50      // 50회 반복

double get_time_sec(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main(void) {
    printf("=== 100만 개 객체 x 50회 반복 할당/해제 성능 테스트 ===\n\n");

    // -------------------------------------------------------------
    // 1. malloc / free 방식 테스트
    // -------------------------------------------------------------
    Node** malloc_ptrs = (Node**)malloc(sizeof(Node*) * NODE_COUNT);

    double start = get_time_sec();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        // 100만 번 malloc
        for (int i = 0; i < NODE_COUNT; i++) {
            malloc_ptrs[i] = (Node*)malloc(sizeof(Node));
        }
        // 100만 번 free
        for (int i = 0; i < NODE_COUNT; i++) {
            free(malloc_ptrs[i]);
        }
    }
    double malloc_time = get_time_sec() - start;
    free(malloc_ptrs);

    printf("[malloc / free]\n");
    printf("소요 시간: %.4f 초\n\n", malloc_time);

    // -------------------------------------------------------------
    // 2. Memory Arena 방식 테스트
    // -------------------------------------------------------------
    // 100만 개 노드에 필요한 크기(약 24MB)보다 여유 있게 1GB 예약
    mem_arena* arena = arena_create(GiB(1), MiB(4));

    start = get_time_sec();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        // 100만 번 arena_push
        for (int i = 0; i < NODE_COUNT; i++) {
            Node* n = PUSH_STRUCT_NZ(arena, Node);
        }
        // 단 1줄로 100만 개 메모리 통째로 롤백 ($O(1)$ 해제)
        arena_clear(arena);
    }
    double arena_time = get_time_sec() - start;
    arena_destroy(arena);

    printf("[Memory Arena]\n");
    printf("소요 시간: %.4f 초\n\n", arena_time);

    printf("=> 아레나가 malloc보다 약 %.1f배 빠름!\n", malloc_time / arena_time);

    return 0;
}