int add(int x[2]) { return x[0] + x[1]; }

int mult4(int x) {
  x = add(x, x);
  x = x * 2;
  returnx;
}