/* quick sort */
int quick_sort(int arr[10], int left, int right) {
  int i, j, pivot;
  i = left;
  j = right;
  pivot = arr[left];
  if (i >= j) {
    return 0;
  }
  while (i < j) {
    while (i < j && arr[j] > pivot) {
      j = j - 1;
    }
    if (i < j) {
      arr[i] = arr[j];
      i = i + 1;
    }
    while (i < j && arr[i] < pivot) {
      i = i + 1;
    }
    if (i < j) {
      arr[j] = arr[i];
      j = j - 1;
    }
  }
  arr[i] = pivot;
  quick_sort(arr, left, i - 1);
  quick_sort(arr, i + 1, right);
  return 0;
}

int main() {
  int cnt, a[10];
  cnt = 0;
  while (cnt < 10) {
    a[cnt] = read();
    cnt = cnt + 1;
  }
  quick_sort(a, 0, 9);
  cnt = 0;
  while (cnt < 10) {
    write(a[cnt]);
    cnt = cnt + 1;
  }
  return 0;
}