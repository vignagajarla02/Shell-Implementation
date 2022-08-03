// Copyright 2022 Lilly Boyd and Vigna Gajarla
// references: https://www.delftstack.com/howto/c/trim-string-in-c/
// https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
// http://www.cs.ecu.edu/karl/4630/spr01/example1.html

#define BUFFLEN 514
#define MAXARGS 100

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// structure for aliasing linked list
struct node {
  char *data;
  char *key;
  struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

void ReversePrint(struct node *head) {
  struct node *tempHead = head;
  struct node *tempHead2 = head;
  if (tempHead2 == NULL) return;
  int length = 0;
  while (head != NULL) {
    head = head->next;
    length = length + 1;
  }
  char *data[length];
  char *key[length];
  int length2 = length;
  while (tempHead != NULL) {
    data[length - 1] = tempHead->data;
    key[length - 1] = tempHead->key;
    tempHead = tempHead->next;
    length = length - 1;
  }
  for (int i = 0; i < length2; i++) {
    fprintf(stdout, "%s %s\n", key[i], data[i]);
    fflush(stdout);
  }
}

// // freeing linked list
// void freeLinkedList(struct node *head) {
//   struct node *tempNode;
//   while (head != NULL) {
//     tempNode = head;
//     head = head->next;
//     free(tempNode);
//   }
// }

int alias(char *arguments, int numArgs) {
  // dangerous cases
  if ((strncasecmp(arguments, "alias alias", 11) == 0) ||
      (strncasecmp(arguments, "alias unalias", 13) == 0) ||
      (strncasecmp(arguments, "alias exit", 10) == 0)) {
    fprintf(stderr, "alias: Too dangerous to alias that.\n");
    fflush(stderr);
    return 0;  // 0 means to prompt >mysh again, return from parse as well
  }
  // display list of all aliases
  else if ((strncasecmp(arguments, "alias", 5) == 0) && numArgs == 1) {
    struct node *toPrint = head;
    ReversePrint(toPrint);
    return 0;  // 0 means to prompt >mysh again, return from parse as well
  }

  // find match for alias key and printing out corresponding data
  else if ((strncasecmp(arguments, "alias", 5) == 0) && numArgs == 2) {
    current = head;
    if (head == NULL) {
      return 0;  // 0 means to prompt >mysh again, return from parse as well
    } else {
      char *arg1 = strtok(arguments, " ");
      arg1 = strtok(NULL, " ");
      while (strncasecmp(current->key, arg1, strlen(arg1)) != 0) {
        // no match
        if (current->next == NULL) {
          return 0;  // 0 means to prompt >mysh again, return from parse as well
        } else {
          current = current->next;
        }
      }
      fprintf(stdout, "%s %s\n", current->key, current->data);
      fflush(stdout);
      return 0;  // 0 means to prompt >mysh again, return from parse as well
    }
  }
  // adding a new alias
  else if ((strncasecmp(arguments, "alias", 5) == 0) && numArgs > 2) {
    current = head;
    int newNode = 0;
    if (head == NULL) {
      struct node *alias = (struct node *)malloc(sizeof(struct node));
      char *argumentCopy = (char *)malloc(sizeof(char *) * 100);
      strcpy(argumentCopy, arguments);
      char *aliasName = strtok(argumentCopy, " ");
      aliasName = strtok(NULL, " ");
      alias->key = aliasName;
      char *temp = malloc(513);
      strtok(arguments, " ");
      strtok(NULL, " ");
      strcpy(temp, strtok(NULL, ""));
      alias->data = temp;
      alias->next = head;
      head = alias;
      return 0;
    }
    char *copyArgs = (char *)malloc(sizeof(char *) * 100);  // freed
    strcpy(copyArgs, arguments);
    char *arg1 = strtok(copyArgs, " ");
    arg1 = strtok(NULL, " ");
    while (strncasecmp(current->key, arg1, strlen(arg1)) != 0) {
      // no match
      if (current->next == NULL) {
        newNode = 1;  // need new node
        break;
      } else {
        current = current->next;
      }
    }
    free(copyArgs);
    // need new node
    if (newNode == 1) {
      struct node *alias = (struct node *)malloc(sizeof(struct node));
      char *argumentCopy = (char *)malloc(sizeof(char *) * 100);
      strcpy(argumentCopy, arguments);
      char *aliasName = strtok(argumentCopy, " ");
      aliasName = strtok(NULL, " ");
      alias->key = aliasName;
      char *temp = malloc(513);
      strtok(arguments, " ");
      strtok(NULL, " ");
      strcpy(temp, strtok(NULL, ""));
      alias->data = temp;
      alias->next = head;
      head = alias;
      return 0;
    } else {
      char *temp = malloc(513);
      strtok(arguments, " ");
      strtok(NULL, " ");
      strcpy(temp, strtok(NULL, ""));
      current->data = temp;
      return 0;
    }

  }
  // incorrect number of args for unaliasing
  else if ((strncasecmp(arguments, "unalias", 7) == 0) && numArgs > 2) {
    fprintf(stderr, "unalias: Incorrect number of arguments.\n");
    fflush(stderr);
    return 0;  // 0 means to prompt >mysh again, return from parse as well
  }
  // unaliasing
  else if ((strncasecmp(arguments, "unalias", 7) == 0) && numArgs == 2) {
    struct node *current = head;
    struct node *previous = NULL;

    // if list is empty - continue
    if (head == NULL) {
      return 0;  // 0 means to prompt >mysh again, return from parse as well
    } else {
      // navigate through list
      char *tempArgs = strtok(arguments, " \t\n");
      tempArgs = strtok(NULL, " \t\n");
      char *tempKey = current->key;
      strtok(tempKey, " \t\n");

      while (strncasecmp(tempKey, tempArgs, strlen(tempArgs)) != 0) {
        // if it's last node
        if (current->next == NULL) {
          return 0;  // 0 means to prompt >mysh again, return from parse as well
        } else {
          previous = current;
          current = current->next;
          tempKey = current->key;
          strtok(tempKey, " \t\n");
        }
      }

      // if a match is found
      if (current == head) {
        head = head->next;
      } else {
        previous->next = current->next;
      }
      return 0;  // 0 means to prompt >mysh again, return from parse as well
    }
  } else {
    // executing alias
    struct node *toFind = head;
    if (head == NULL) {
      return 1;  // go to fork
    }
    char *commandCopy = (char *)malloc(sizeof(char *) * 100);  // freed
    strcpy(commandCopy, arguments);
    char *tempArgs = strtok(commandCopy, " \t\n");
    char *tempKey = toFind->key;
    strtok(tempKey, " \t\n");
    while (strncasecmp(tempKey, tempArgs, strlen(tempArgs)) != 0) {
      if (toFind->next == NULL) {
        return 1;  // go to fork
      } else {
        toFind = toFind->next;
        tempKey = toFind->key;
        strtok(tempKey, " \t\n");
      }
    }
    char *anotherArgsCopy = (char *)malloc(sizeof(char *) * 100);  // freed
    char *finalCommand = (char *)malloc(sizeof(char *) * 100);     // freed
    strcpy(finalCommand, toFind->data);
    strcpy(anotherArgsCopy, arguments);
    anotherArgsCopy = strtok(anotherArgsCopy, " ");
    anotherArgsCopy = strtok(NULL, " ");
    while (anotherArgsCopy != NULL) {
      strcat(finalCommand, anotherArgsCopy);
      strcat(finalCommand, " ");
      anotherArgsCopy = strtok(NULL, " ");
    }
    strcpy(arguments, finalCommand);
    free(commandCopy);
    free(finalCommand);
    free(anotherArgsCopy);
    // freeLinkedList(head);
    return 1;  // go to fork
  }
}

// trimming string
char *trim(char *toTrim) {
  char *addNull;
  while (isspace((unsigned char)*toTrim)) {
    toTrim++;
  }
  if (*toTrim == 0) {
    return toTrim;
  }
  addNull = toTrim + strlen(toTrim) - 1;
  while (addNull > toTrim && isspace((unsigned char)*addNull)) {
    addNull--;
  }
  addNull[1] = '\0';
  return toTrim;
}

void parse_command(char *command, FILE *fp) {
  char delim[] = " \t\f\v\r\n";    // delimeter for strtok
  char commandArguments[MAXARGS];  // array for command args before >
  int redirectionCount = 0;        // check for number of >
  char files[MAXARGS];             // array for files after >
  int fileCount = 0;               // number of files after >
  int commandCount = 0;            // number of commands before >
  commandArguments[0] = '\0';
  files[0] = '\0';
  char *check = strchr(command, '>');
  if (check == NULL) {  // There is no redirection
    char *tokenNo = strtok(command, delim);
    while (tokenNo != NULL) {
      commandCount += 1;
      strcat(commandArguments, tokenNo);
      strcat(commandArguments, " ");
      tokenNo = strtok(NULL, delim);
    }
  } else {
    // Check for multiple redirection
    for (int i = 0; command[i] != '\0'; i++) {
      if (command[i] == '>') {
        redirectionCount += 1;
      }
    }
    if (redirectionCount > 1) {
      fprintf(stderr, "Redirection misformatted.\n");
      fflush(stderr);
      return;
    }
    char *beforeCopy = strtok(command, ">");
    char *before = beforeCopy;
    beforeCopy = strtok(NULL, ">");
    char *after = beforeCopy;
    if (before == NULL || after == NULL) {
      fprintf(stderr, "Redirection misformatted.\n");
      fflush(stderr);
      return;
    } else {
      char *before2 = strtok(before, delim);
      while (before2 != NULL) {
        commandCount += 1;
        strcat(commandArguments, before2);
        strcat(commandArguments, " ");
        before2 = strtok(NULL, delim);
      }
      char *after2 = strtok(after, delim);
      while (after2 != NULL) {
        fileCount += 1;
        if (fileCount == 1) {
          strcat(files, after2);
          strcat(files, " ");
        } else {
          fprintf(stderr, "Redirection misformatted.\n");
          fflush(stderr);
          return;
        }
        after2 = strtok(NULL, delim);
      }
    }
  }

  files[strlen(files) - 1] = '\0';

  if (commandCount >= 1 && redirectionCount == 1 && fileCount == 0) {
    fprintf(stderr, "Redirection misformatted.\n");
    fflush(stderr);
    return;
  }

  if (strncasecmp(commandArguments, "\0", 2) == 0) {
    return;
  }

  if (strncasecmp(commandArguments, "exit", 4) == 0) {
    if (fp != NULL) {
      fclose(fp);
    }
    exit(0);
  }
  // trim whitespace off commandargs - probably a better way to do this (have
  // extra \n or whitespace at the end of command args)
  char *trimmedArgs = trim(commandArguments);
  int result = alias(trimmedArgs, commandCount);
  if (result == 0) {
    return;
  } else {
    // START OF FORKING
    pid_t child_pid;
    int child_status;
    char *argv[MAXARGS];
    char *token2 = strtok(trimmedArgs, " ");
    int index = 0;
    // put command args into argv
    while (token2 != NULL) {
      argv[index] = token2;
      token2 = strtok(NULL, " ");
      index = index + 1;
    }
    argv[index] = NULL;
    child_pid = fork();
    if (child_pid == 0) {
      if (redirectionCount == 1 && fileCount == 1) {
        close(STDOUT_FILENO);
        open(files, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      }
      if (execv(argv[0], argv) == -1) {
        char *error = argv[0];
        strcat(error, ": Command not found.\n");
        write(STDERR_FILENO, error, strlen(error));
        fclose(fp);
        _exit(1);
      }
    } else {
      waitpid(child_pid, &child_status, 0);
    }
  }
}

int main(int argc, char **argv) {
  if (argc > 2) {
    write(STDERR_FILENO, "Usage: mysh [batch-file]\n",
          strlen("Usage: mysh [batch-file]\n"));
    exit(1);
  }
  FILE *fp;
  // batch mode to open
  if (argc == 2) {
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
      fprintf(stderr, "Error: Cannot open file %s.\n", argv[1]);
      fflush(stderr);
      exit(1);
    }
    char command[BUFFLEN];
    while (fgets(command, BUFFLEN, fp) != NULL) {
      write(1, command, strlen(command));
      // check if command is > 512
      if (strlen(command) > 512) {
        fprintf(stderr, "Error: command is too long. Command: %s.\n", command);
        fflush(stderr);
        continue;
      }
      parse_command(command, fp);
    }
    fclose(fp);
  }

  fp = NULL;
  // interactive
  if (argc == 1) {
    char command[BUFFLEN];
    while (1) {
      write(1, "mysh> ", 6);
      if (fgets(command, BUFFLEN, stdin) == NULL) break;
      if (command[strlen(command) - 1] != '\n') {
        fprintf(stderr,
                "Error: command is too long (command is greater than 512 "
                "characters).\n");
        fflush(stderr);
        continue;
      }
      fflush(stdout);
      parse_command(command, fp);
    }
  }
  return 0;
}
