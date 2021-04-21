#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wait.h>
#include <string.h>

#define ERROR -1
#define CHILD 0
#define SUCCESS 0
#define EXIT_CODE 1
#define NUM_OF_ARG 2
#define TRUE 1

int main(int argc, char **argv) {
    if (argc != NUM_OF_ARG){
        printf("incorrect number of arguments\n");
        exit(EXIT_CODE);
    }

    pid_t pid; // pid_t тип данных для ID процесса
    pid = fork(); // создать новый прооцесс. Родительский процесс -> идентификатор порожденного
    //процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno

    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    }

    if (pid == CHILD) { // порожденный процесс
        char *command = "cat";
        char *newArgv[] = {command, argv[1], (char *) 0}; //конец списка  параметров. Он необходим, потому что
        //execvp может  быть передано  произвольное  число параметров.
        int callingCode = execvp(command, newArgv); // значение возвращается только при возникновении ошибки
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(EXIT_CODE);
        }
        // в случае успешного выполнения функции execv новая программа не возвращает управление в исходную
    }

    else { // родительский процесс
        int status;
        pid_t childID = wait(&status);//ожидание остановки или завершения порожденного процесса
        if (childID == ERROR) {
            perror("Error in wait");
            exit(EXIT_CODE);
        }

        if (WIFEXITED(status) == TRUE) {
            int exitStatus = WEXITSTATUS(status);
            printf("\nThe child process %d ended with code %d\n", childID, exitStatus);// возвращает код завершения подпроцесса
            exit(SUCCESS);
        } else if (WIFSIGNALED(status) == TRUE){
            int signal = WTERMSIG(status);
            printf("\nthe child process %lld failed with signal%d\n", pid, signal);
            exit(EXIT_CODE);
        }
    }
}
