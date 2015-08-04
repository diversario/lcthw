#include <stdio.h>

int main (int argc, char *argv[]) {
  int i = 0;
  
  if (argc == 2) {
    printf("Only one arg!");
  } else if (argc > 1 && argc < 5) {
    printf("Here's arguments:\n");

    char **args = argv;

    for (i = 0; i < argc; i++) {
      printf("%s ", *(args + i));
    }

    printf("\n");
  } else {
    printf("Too many args\n");
  }

  return 0;
}
