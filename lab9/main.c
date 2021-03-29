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
#define SIZE 4

int main(int argc, char **argv){
    pid_t pid; // pid_t тип данных для ID процесса
    pid = fork(); // создать новый прооцесс. Родительский процесс -> идентификатор порожденного
    //процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno
    if (pid == ERROR) {
        perror("Fork error");
        exit(1);
    } else if (pid == SUCCESS) { // порожденный процесс
        char *command = "cat";
        char *newArgv[] = {command, argv[1], (char*) 0}; //конец списка  параметров. Он необходим, потому что
        //execvp может  быть передано  произвольное  число параметров.
        int callingCode = execvp(command, newArgv);
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(1);
        }
    }

    int exitCode;
    pid_t childID = wait(&exitCode);//ожидание остановки или завершения порожденного процесса
    int check = WIFEXITED(exitCode); //не равно нулю, если дочерний процесс успешно завершился.
    if (check != ERROR_WIFEXITED) {
        printf("\nWaited long enough for child %d to die his own death with code %d\n", childID, WEXITSTATUS(exitCode));
        // возвращает код завершения подпроцесса
        exit(0);
    }
    return 0;
}
