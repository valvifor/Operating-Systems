#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

#define EXIT_CODE 1
#define ERROR -1
#define CHILD 0
#define SUCCESS 0

int execvpe(char *file, char *argv[], char *envp[]){
    char **oldEnviron = environ;
    clearenv();
    char **p;
    for(p = envp ; *p ; p++) {
        putenv(*p);
    }
    printf("\nnew environ: \n");
    for(p = environ ; *p ; p++) {
        printf("%s\n", *p);
    }
    printf("\nresult of executing your command:\n");
    int callingCode = execvp(file, argv); // значение возвращается только при возникновении ошибки
    if (callingCode == ERROR) {
        perror("Execvp error");
        exit(EXIT_CODE);
    }
    environ = oldEnviron;
    printf("\nold environ: \n");
    for(p = environ ; *p ; p++) {
        printf("%s\n", *p);
    }
    return SUCCESS;
}

int main(int argc, char * argv[]){
    pid_t pid;
    char *fileName = argv[1];
    char *newArgv[] = {fileName, (char*)0};

    pid = fork();

    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    }

    if (pid == CHILD) {
        int res = execvpe(fileName, newArgv, argv + 1);
        if (res != SUCCESS) {
            perror("execvpe error");
            exit(EXIT_CODE);
        }
    }
    else {
        int status;
        pid_t childID = wait(&status);
        if (childID == ERROR) {
            perror("Error wait");
            exit(EXIT_CODE);
        }

        int check = WIFEXITED(status);//не равно нулю, если дочерний процесс успешно завершился.

        if (check != 0) {
            int exitStatus = WEXITSTATUS(status);
            printf("\nchild process completed successfully\n");
            exit(SUCCESS);
        } else {
            printf("\nthe child process failed with an error\n");
            exit(EXIT_CODE);
        }
    }
    exit(SUCCESS);
}
