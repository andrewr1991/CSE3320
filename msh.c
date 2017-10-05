/*
    Some code belongs to Trevor Bakker
    professor at UT Arlington
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main( void ) {

  while (1) {
    printf("msh> ");
    char command1[100];
    fgets(command1, 100, stdin);

    command1[strlen(command1)-1] = '\0';
    char path[100] = "/bin";
    //strcat...

    //if strcmp cd == 0 then change to second part of command - chdir command

    if (strcmp(command1, "exit") == 0 || strcmp(command1, "quit") == 0) {
      exit(1);
    }
//put everything else above the fork
    else if (strcmp(command1, "ls") == 0) {
      pid_t child_pid = fork();
      int status;

      if( child_pid == 0 ) {
        execl("/bin/ls", "ls", NULL );
        exit( EXIT_SUCCESS );
      }
      waitpid( child_pid, &status, 0 );
    }

    else {
      if ((int)*command1 != 0) {
        printf("%s : Command not found.\n", command1);
      }
    }
  }
  return 0;
}
