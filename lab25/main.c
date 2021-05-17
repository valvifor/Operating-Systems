#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdint.h>

#define NUM_OF_PROC 2
#define ERROR -1
#define READ 0
#define WRITE 1
#define CHILD 0
#define TRUE 1
#define SUCCESS 0

int childProcess(int *fd) {
    close(fd[WRITE]); // Закрывает неиспользуемый конец для записи
    int count;
    int bytesRead = read(fd[READ], &count, sizeof(int));
    if (bytesRead == ERROR) {
        perror("read error");
        return ERROR;
    }
    char text[count];
    bytesRead = read(fd[READ], text, count);
    if (bytesRead == ERROR) {
        perror("read error");
        return ERROR;
    }
    for(int i = 0; i < count; i++) {
        text[i] = toupper(text[i]);
    }
    printf("%s\n", text);
    close(fd[READ]);
    return SUCCESS;
}

int parentProcess(int *fd) {
    close(fd[READ]); // Закрывает неиспользуемый конец для чтения
    char *text = "ThiS Is a tExt cOnsiSting oF uppeRcaSe And lowERCase charaCterS\n";
    int len = strlen(text) + 1;
    int bytesWrote = write(fd[WRITE], &len, sizeof(int));
    if (bytesWrote == ERROR) {
        perror("write error");
        return ERROR;
    }
    bytesWrote = write(fd[WRITE], text, len);
    if (bytesWrote == ERROR) {
        perror("write error");
        return ERROR;
    }
    int status;
    pid_t childID = wait(&status);//ожидание остановки или завершения порожденного процесса
    if (childID == ERROR) {
        perror("Error in wait");
        return ERROR;
    }
    if (WIFEXITED(status) == TRUE) {
        int exitStatus = WEXITSTATUS(status);
        close(fd[WRITE]);
        return exitStatus;
    } else if (WIFSIGNALED(status) == TRUE){
        int signal = WTERMSIG(status);
        printf("\nthe child process failed with signal%d\n", signal);
        return ERROR;
    }
}

int main(int argc, char **argv) {
    int fd[NUM_OF_PROC];
    pid_t pid; // pid_t тип данных для ID процесса
    int pipeStatus = pipe(fd); // создание канала для взаимодействия между процессами
    if (pipeStatus == ERROR) {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }
    pid = fork(); // создать новый прооцесс. Родительский процесс -> идентификатор порожденного процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno
    if (pid == ERROR) {
        close(fd[READ]);
        close(fd[WRITE]);
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
    if (pid == CHILD) { // порожденный процесс
        int childResult = childProcess(fd);
        if (childResult == ERROR) {
            close(fd[READ]);
            exit(EXIT_FAILURE);
        }
    }
    else { // родительский процесс
        int parentResult = parentProcess(fd);
        if (parentResult == ERROR) {
            close(fd[WRITE]);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
