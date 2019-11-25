/*
 * Assignment 1          CSCI 3120
 *
 * <A c program serves as a simple shell>
 *
 *
 * -the program awaits for the user input
 * -once a arguments is entered, the program parse it into token stream then store it in a arg list
 * -by creating a child process, the shell then run the command that is asked by the user
 * -the user can always check 10 previously entered command, listed in asending order
 * -the program also runs the most recent conmmand by enters '!!', or '! + interger<10" to run the n th recent command
 *
 *
 * ***This is entirely my own work***
 *
 * Qu Yuze
 *
 * B00713367
 *
 * Dalhousie Univeristy October/06/2019
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_LINE 80
#define DELIMETER " \t\r\n\a"

int main() {

    char * args[MAX_LINE / 2 + 1];
    int should_run = 1;
    int count = 0;

    while(should_run){

        char command[MAX_LINE];
        char * commandList;
        int processID[MAX_LINE / 2 + 1];
        char * history[MAX_LINE / 2 + 1];
        char * tokenStream;
        char * recall;

        printf("CSCI3120>");
        fflush(stdout);

        command[0] = '\0';
        memset(command, 0, MAX_LINE);

        int i;
        for(i=0; i<MAX_LINE/2+1; i++){
            args[i] = '\0';
        }

        fgets(command, MAX_LINE, stdin);
        strtok(command, "\n");
        commandList = command;

        /*
         * check if the user want to review the command already entered
         * the shell than will dispaly the most recent command in asending order
         * only 10 commands will be dispalyed to the user
         */
        if(strcmp(commandList, "history")!=0 && commandList[0] != '!'){
            if(strcmp(commandList, "exit") == 0){
                exit(EXIT_SUCCESS);
            }
            history[count] = strdup(commandList);
            count++;

        } else if(strcmp(commandList, "history")==0){
            if(count == 0){
                printf("Nothing to display.\n");
                continue;
            }
            int k;
            int q = 0;
            if(count < 10){
                for(k=count; k>0; k--){
                    q++;
                    printf("%d %d %s\n", q, processID[k], history[k-1]);
                }
            }else{
                for(k=count; k>count-10; k--){
                    q++;
                    printf("%d %d %s\n", q, processID[k], history[k-1]);
                }
            }
        }
        /*
         * check is the user want to recall the recently entered command
         * if the user enters ! + num, fins the nth command in the history list
         * otherwise, run the most recent command
         */
        if(commandList[0] == '!' && commandList[1] - '0' > 0){
            int num = commandList[1] - '0';
            if( commandList[2] - '0' == 0){
                num = 10;
            }
            if(count == 0){
                printf("No command in history\n");
                continue;
            }else if(num > count){
                printf("No such command in the history\n");
                continue;
            }
            char * cmd;
            cmd = history[count - num];
            recall = strdup(cmd);
            history[count++] = strdup(recall);
            strcpy(command, recall);

        }else if(strcmp(commandList, "!!") == 0){
            if(count == 0){
                printf("No command in the history");
                continue;
            }
            char * cmd;
            cmd = history[count - 1];
            recall = strdup(cmd);
            history[count++] = strdup(recall);
            strcpy(command, recall);
        }
        /*
         * parse the command into tokens, then store it in the args array
         * create a child procss to run the command in the arg list
         * store the process id in the id list
         * wait the child to be finihsed
         */
        tokenStream = strtok(command, DELIMETER);

        i = 0;
        while(tokenStream != NULL){
            args[i] = tokenStream;
            i++;
            tokenStream = strtok(NULL, DELIMETER);
        }
        args[i] = NULL;

        pid_t pid;
        pid = fork();
        if(pid == 0){
            execvp(args[0], args);
            exit(EXIT_FAILURE);

        }else if(pid < 0){
            fprintf(stderr, "Forking error");
        }else{
            processID[count] = pid;
            wait(NULL);
        }
    }
    return EXIT_SUCCESS;
}


