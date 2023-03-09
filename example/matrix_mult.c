int matrix_mult(int dest[4][4], int src1[4][4], int src2[4][4]) {
  int i, j, k;

  while (i < 4) {
    while (j < 4) {
      while (k < 4) {
        dest[i][j] = dest[i][j] + src1[i][k] + src2[k][j];
        k = k + 1;
      }
      j = j + 1;
    }
    i = i + 1;
  }
}