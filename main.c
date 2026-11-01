#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 무작위 데이터 생성
void generate_data(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

// 배열 복사
void copy_array(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// 오름차순 정렬 확인
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0; // 정렬 실패
        }
    }
    return 1; // 정렬 성공
}

// [1] 삽입 정렬
long long insertion_sort(int arr[], int n) {
    long long comparisons = 0; // 비교 횟수

    for (int i = 1; i < n; i++) {
        int key = arr[i]; // 삽입할 값
        int j = i - 1;

        // 인덱스 조건 검사 (비교 횟수 제외)
        while (j >= 0) {
            comparisons++; // 데이터 값 비교 횟수 1 증가
            if (arr[j] > key) {
                arr[j + 1] = arr[j]; // 오른쪽으로 이동
                j--;
            }
            else {
                break; // 삽입 위치 확정
            }
        }
        arr[j + 1] = key; // 값 삽입
    }

    return comparisons;
}

// [2] 쉘 정렬 - 기본 간격 (n/2, n/4, ..., 1)
long long shell_sort_original(int arr[], int n) {
    long long comparisons = 0; // 비교 횟수

    // 간격 h를 절반씩 감소
    for (int h = n / 2; h > 0; h /= 2) {
        // 간격 h만큼 떨어진 요소들 삽입 정렬
        for (int i = h; i < n; i++) {
            int key = arr[i];
            int j = i - h;

            while (j >= 0) {
                comparisons++; // 데이터 값 비교 횟수 1 증가
                if (arr[j] > key) {
                    arr[j + h] = arr[j]; // h칸 오른쪽으로 이동
                    j -= h;
                }
                else {
                    break; // 삽입 위치 확정
                }
            }
            arr[j + h] = key; // 값 삽입
        }
    }

    return comparisons;
}

// [3] 쉘 정렬 - Knuth 간격 수열 (1, 4, 13, 40, ...)
long long shell_sort_knuth(int arr[], int n) {
    long long comparisons = 0; // 비교 횟수

    // n보다 작은 가장 큰 Knuth 간격 계산
    int h = 1;
    while (3 * h + 1 < n) {
        h = 3 * h + 1;
    }

    // 간격 수열 역순 적용
    for (; h > 0; h = (h - 1) / 3) {
        // 간격 h만큼 떨어진 요소들 삽입 정렬
        for (int i = h; i < n; i++) {
            int key = arr[i];
            int j = i - h;

            while (j >= 0) {
                comparisons++; // 데이터 값 비교 횟수 1 증가
                if (arr[j] > key) {
                    arr[j + h] = arr[j]; // h칸 오른쪽으로 이동
                    j -= h;
                }
                else {
                    break; // 삽입 위치 확정
                }
            }
            arr[j + h] = key; // 값 삽입
        }
    }

    return comparisons;
}

int main(void) {
    // 난수 시드 설정
    srand((unsigned int)time(NULL));

    // 실험할 데이터 크기 배열
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];

        // 원본 및 복사본 배열 동적 할당
        int* original = (int*)malloc(sizeof(int) * n);
        int* arr_ins = (int*)malloc(sizeof(int) * n);
        int* arr_shell_orig = (int*)malloc(sizeof(int) * n);
        int* arr_shell_knuth = (int*)malloc(sizeof(int) * n);

        // 메모리 할당 실패 확인
        if (!original || !arr_ins || !arr_shell_orig || !arr_shell_knuth) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        // 원본 데이터 생성 및 복사
        generate_data(original, n);
        copy_array(original, arr_ins, n);
        copy_array(original, arr_shell_orig, n);
        copy_array(original, arr_shell_knuth, n);

        // 세 가지 정렬 수행 및 비교 횟수 측정
        long long comp_ins = insertion_sort(arr_ins, n);
        long long comp_shell_orig = shell_sort_original(arr_shell_orig, n);
        long long comp_shell_knuth = shell_sort_knuth(arr_shell_knuth, n);

        // 정렬 검증
        if (!is_sorted(arr_ins, n) || !is_sorted(arr_shell_orig, n) || !is_sorted(arr_shell_knuth, n)) {
            printf("Sorting verification failed at size %d!\n", n);
            return 1;
        }

        // 결과 출력
        printf("Data Size : %d\n\n", n);
        printf("Insertion Sort       : %lld comparisons\n", comp_ins);
        printf("Shell Sort (Original): %lld comparisons\n", comp_shell_orig);
        printf("Shell Sort (Knuth)   : %lld comparisons\n\n", comp_shell_knuth);

        // 동적 메모리 해제
        free(original);
        free(arr_ins);
        free(arr_shell_orig);
        free(arr_shell_knuth);
    }

    return 0;
}