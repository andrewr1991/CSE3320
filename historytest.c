#include <stdio.h>
#include <string.h>

int main() {
  char history[11][100];
  strcpy(history[0], "history command 1");
  strcpy(history[1], "history command 2");
  strcpy(history[2], "history command 3");
  strcpy(history[3], "history command 4");
  strcpy(history[4], "history command 5");


  for (int i = 0; i < 5; i++) {
    printf("%s\n", history[i]);
  }

  printf("----\n");

  for (int i = 0; i < 4; i++) {
    strcpy(history[i], history[i + 1]);
  }
  strcpy(history[4], "got it!");

  for (int i = 0; i < 5; i++) {
    printf("%s\n", history[i]);
  }
  return 0;
}
