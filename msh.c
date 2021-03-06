/*
  name: Andrew Ridout
  ID: 1001489714

  Some code belongs to Trevor Bakker
  professor at UT Arlington
*/

// The MIT License (MIT)
//
// Copyright (c) 2016, 2017 Trevor Bakker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports five arguments

int main( void ) {

  /* Parse input */
  char *token[MAX_NUM_ARGUMENTS];

  // Declare 2D array that will hold the history of user commands
  // tempCmd will be used later on for history
  // counter will be used to keep track of how many commands the user has entered
  // to know how many to show
  char history[15][100];
  char tempCmd[100];
  int counter = 0;

  while (1) {
    printf("msh> ");

    int i = 0;
    for( i = 0; i < 10; i ++ ) {
      token[i] = NULL;
    }

    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    int token_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;

    char *working_str  = strdup( cmd_str );

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && (token_count<MAX_NUM_ARGUMENTS)) {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 ) {
        token[token_count] = NULL;
      }
      token_count++;
    }

    free( working_root );

    // If the user hit return with no command, the code will return back to the
    // beginning of the loop and wait for another command.
    if( token[0] == '\0') {
      continue;
    }

    // If the counter variable is less than or equal to 14, then add the latest
    // command to the history array. In other words, before the history list must
    // rotate through commands, the current command will just simply be added to
    // the array.
    if (counter <= 14) {
      strcpy(history[counter], token[0]);
    }

    // Before the counter variable is less than 15, counter will simply be incremented
    // however whenever counter is greater than or equal to 15, the subroutine
    // to rotate the history array will be called. The tempCmd variable below will
    // hold the earliest user command to be used later.
    if (counter < 15) {
      counter++;
    }
    else {
      strcpy(tempCmd, history[0]);
      for (int i = 0; i < counter; i++) {
        strcpy(history[i], history[i + 1]);
      }
      strcpy(history[14], token[0]);
    }

    // These variables below are all to keep track of the number entered after the
    // exclamation point. For example, with the command !12.
    int first = 0;
    int second = 0;
    int charSum = 0;
    first = (*(token[0] + 1)) - 48;
    second = (*(token[0] + 2)) - 48;

    // Code to concatenate the numbers after the exclamation point. For example,
    // !12 is 1 and 2 which becomes 12.
    // If !10 through !14 is entered
    if ((*(token[0] + 2)) != 0) {
      charSum = (first * 10) + second;
    }
    // If !0 through !9 is entered
    else {
      charSum = first;
    }

    // If !1 through !14 is entered, token[0]'s value becomes the command at
    // index charSum of the history array.
    if (strchr("!", *token[0])) {
      if (charSum > 0 && charSum <= 14) {
        strcpy(token[0], history[charSum - 1]);
      }
      // If !0 is entered, token[0]'s value becomes tempCmd
      else if (charSum == 0) {
        strcpy(token[0], tempCmd);
      }
      // If anything other than !0 through !14 is entered
      else {
        printf("Command not in history.\n");
        continue;
      }
    }

    // Loop through the history array and print the conents if the current command
    // equals "history".
    if (strcmp(token[0], "history") == 0) {
      for (int i = 0; i < counter; i++) {
        printf("%d: %s\n", i, history[i]);
      }
    continue;
    }

    // If the current command equals "exit" or "quit", exit the shell via exit(0).
    if (strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0) {
      exit(0);
    }

    // If the current command equals "cd", call the cd shell command via chdir.
    if (strcmp(token[0], "cd") == 0) {
      chdir(token[1]);
      continue;
    }

    // Initialize the path array to the current directory and then concatenate the
    // current command onto the end of "./"
    char path[100] = "./";
    strcat(path, token[0]);

    pid_t child_pid = fork();

    // In all of the exec calls below, memset is used to set the path array to all
    // zeros, each new directory is then copied into path using strcpy, then strcat
    // is used to concatenate the current command onto the end of the new directory.
    if ( child_pid == 0 ) {
      // Call exec with the current command and search in the current directory
      execl(path, token[0], token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8], token[9],NULL );

      // Call exec with the current command and search in the /usr/local/bin/ directory
      memset(path, 0, 100);
      strncpy(path, "/usr/local/bin/", strlen("/usr/local/bin/"));
      strcat(path, token[0]);
      execl(path, token[0], token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8], token[9], NULL );

      // Call exec with the current command and search in the /usr/bin/ directory
      memset(path, 0, 100);
      strncpy(path, "/usr/bin/", strlen("/usr/bin/"));
      strcat(path, token[0]);
      execl(path, token[0], token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8], token[9], NULL );

      // Call exec with the current command and search in the /bin/ directory
      memset(path, 0, 100);
      strncpy(path, "/bin/", strlen("/bin/"));
      strcat(path, token[0]);
      execl(path, token[0], token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8], token[9], NULL );
      printf("%s: Command not found.\n", token[0]);
      exit( EXIT_SUCCESS );
    }
    else {
      int status;
      wait(&status);
    }
  }
return 0;
}
