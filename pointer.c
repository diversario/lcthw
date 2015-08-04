#include <stdio.h>

int main(){
  char a1[] = "a1";
  char *a2[] = {"a2_1", "a2_2"};
  char a3[] = {"a3_1"};
  int x = 1; int y = 2; int z = 3;
  int *i1[] = {&x, &y, &z};

  printf("%i\n", *i1[0]);
  printf("%i\n", *i1[1]);

  return 0;
}
