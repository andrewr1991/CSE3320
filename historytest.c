#include <stdio.h>
#include <string.h>

int main() {
  char history[11][100];
  strcpy(history[0], "history command 1");
  strcpy(history[1], "history command 2");
  strcpy(history[2], "history command 3");
  strcpy(history[3], "history command 4");
  strcpy(history[4], "history command 5");
  strcpy(history[5], "history command 6");
  strcpy(history[6], "history command 7");
  strcpy(history[7], "history command 8");
  strcpy(history[8], "history command 9");
  strcpy(history[9], "history command 10");


  for (int i = 0; i < 10; i++) {
    printf("%s\n", history[i]);
  }

  printf("g\n");

  for (int i = 0; i < 10; i++) {
    char temp[100];
    strcpy(temp, history[i + 1]);
    strcpy(history[i + 1], temp);
  }

  for (int i = 0; i < 10; i++) {
    printf("%s\n", history[i]);
  }
  return 0;
}
