#include <stdio.h>

const unsigned N = 3;

// 为什么当N足够大时，traverse_n_matrix_1会比traverse_n_matrix_2有更好的性能
void traverse_n_matrix_1(int matrix[N][N], unsigned n) {
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < n; ++j) {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void traverse_n_matrix_2(int matrix[N][N], unsigned n) {
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < n; ++j) {
      printf("%d\t", matrix[j][i]);
    }
    printf("\n");
  }
}

int main() {
  int matrix[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  traverse_n_matrix_1(matrix, N);
  printf("\n");
  traverse_n_matrix_2(matrix, N);

  return 0;
}
