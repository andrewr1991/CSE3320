/*
  name: Andrew Ridout
  ID: 1001489714

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
    char command[100];
    fgets(command, 100, stdin);

    command[strlen(command)-1] = '\0';

    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
      exit(0);
    }
// tokenize anythign after command and put in place of NULL in exec - up through 9 command
// initialize all to null
    /* Call exec with ./ - the local directory */
    char path[100] = "./";
    strcat(path, command);

    pid_t child_pid = fork();
    int status;

    if ( child_pid == 0 ) {
      execl(path, command, NULL );

      /* Call exec with /usr/local/bin */
      memset(path, 0, 100);
      strncpy(path, "/usr/local/bin/", strlen("/usr/local/bin/"));
      strcat(path, command);
      execl(path, command, NULL );

      /* Call exec with /usr/bin */
      memset(path, 0, 100);
      strncpy(path, "/usr/bin/", strlen("/usr/bin/"));
      strcat(path, command);
      execl(path, command, NULL );

      /* Call exec with /bin */
      memset(path, 0, 100);
      strncpy(path, "/bin/", strlen("/bin/"));
      strcat(path, command);
      execl(path, command, NULL );
      printf("%s: Command not found.\n", command);
      exit( EXIT_SUCCESS );
  }

  wait( &status);
    // strcat ./
    // call exec
    // strcat /usr/local/bin
    // call execl
    // strcat /usr/bin
    // call execl
    // strcat /bin
    // call execl
    // print command not found

    //if strcmp cd == 0 then change to second part of command - chdir command



    //else if ()
//put everything else above the fork
    }
  return 0;
}
