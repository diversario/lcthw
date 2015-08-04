#include <stdio.h>
#include <ctype.h>
#include <string.h>

int can_print(char ch);
void print_letters(char arg[], int len);

void print_arguments(int argc, char *argv[]) {
  int i = 0;

  for (i = 0; i < argc; i++) {
    print_letters(argv[i], strlen(argv[i]));
  }
}

void print_letters(char arg[], int len) {
  int i = 0;

  for (i = 0; i < len; i++) {
    char c = arg[i];

    if (can_print(c)) {
      printf("%c == %d\n", c, c);
    }
  }

  printf("\n");
}

int can_print(char c) {
  return isalpha(c) || isblank(c);
}

int main (int argc, char *argv[]) {
  print_arguments(argc, argv);
  return 0;
}
