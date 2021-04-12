#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <wait.h>

#define WRONG_ARGC 1
#define EXIT_CODE 1
#define ERROR -1
#define SUCCESS 0
#define CHILD 0

int main(int argc, char * argv[]){
    pid_t pid;// pid_t тип данных для ID процесса
    if(argc == WRONG_ARGC) {
        printf("No program availiable\n");
        exit(EXIT_CODE);
    }
    pid = fork();// создать новый прооцесс. Родительский процесс -> идентификатор порожденного
    //процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno

    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    }

    if (pid == CHILD) {// порожденный процесс
        char *command = argv[1];
        int callingCode = execvp(command, argv + 1);
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(EXIT_CODE);
        }
    }

    else {
        int exitCode;
        pid_t childID = wait(&exitCode);//ожидание остановки или завершения порожденного процесса
        if (childID == ERROR) {
            perror("Error in wait");
            exit(EXIT_CODE);
        }

        int check = WIFEXITED(exitCode);//не равно нулю, если дочерний процесс успешно завершился.

        if (check != 0) {
            int exitStatus = WEXITSTATUS(exitCode);
            printf("\nExit code %lld: %d\n", pid, exitStatus);
            exit(SUCCESS);
        } else {
            printf("\nthe child process %lld failed with an error\n", pid);
            exit(EXIT_CODE);
        }
    }
}
