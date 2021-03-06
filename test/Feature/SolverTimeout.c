// RUN: %llvmgcc %s -emit-llvm -O0 -c -o %t1.bc
// RUN: rm -rf %t.klee-out
// RUN: %klee --output-dir=%t.klee-out --max-solver-time=1 %t1.bc
// FIXME: This test occasionally fails when using Z3 4.4.1 but
// not when using Z3 from the master branch. So disable the test for now.
// REQUIRES: stp
#include <stdio.h>

int main() {
  long long int x, y = 102*75678 + 78, i = 101;

  klee_make_symbolic(&x, sizeof(x), "x");

  if (x*x*x*x*x*x*x*x*x*x*x*x*x*x*x*x + (x*x % (x+12)) == y*y*y*y*y*y*y*y*y*y*y*y*y*y*y*y % i)
    printf("Yes\n");
  else printf("No\n");
  
  return 0;
}
