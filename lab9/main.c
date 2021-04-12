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

int main(int argc, char **argv) {
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
        int callingCode = execv(command, newArgv); // значение возвращается только при возникновении ошибки
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(EXIT_CODE);
        }
        // в случае успешного выполнения функции execv новая программа не возвращает управление в исходную
    }

    else { // родительский процесс
        int exitCode;
        pid_t childID = wait(&exitCode);//ожидание остановки или завершения порожденного процесса
        if (childID == ERROR) {
            perror("Error in wait");
            exit(EXIT_CODE);
        }

        int check = WIFEXITED(exitCode); //не равно нулю, если дочерний процесс успешно завершился

        if (check != 0) {
            int exitStatus = WEXITSTATUS(exitCode);
            printf("\nThe child process %d ended with code %d\n", childID, exitStatus);// возвращает код завершения подпроцесса
            exit(SUCCESS);
        } 
    }
}
