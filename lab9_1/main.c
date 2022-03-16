#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wait.h>
#include <string.h>

#define ERROR -1
#define SUCCESS 0
#define ERROR_WIFEXITED 0
#define ERROR_WIFSIGNALED 0
#define EXIT_CODE 1

int main(int argc, char **argv){
    pid_t pid; // pid_t тип данных для ID процесса
    pid = fork(); // создать новый прооцесс. Родительский процесс -> идентификатор порожденного
    //процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno
    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    } else if (pid == SUCCESS) { // порожденный процесс
        char *command = "cat";
        char *newArgv[] = {command, argv[1], (char*) 0}; //конец списка  параметров. Он необходим, потому что
        //execvp может  быть передано  произвольное  число параметров.
        int callingCode = execvp(command, newArgv);
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(EXIT_CODE);
        }
    }
    if((pid = wait((int*)0)) == -1) {
        perror("Error wait");
        exit(1);
    }
    printf("I'm parent of %lld\n", pid);
}
