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
#define TRUE 1
#define NUM_OF_ARGS 2

int execvpe(char *file, char *argv[], char *envp[]){
    char **oldEnviron = environ;
    int result = clearenv();
    if (result != SUCCESS) {
        printf("Error with clearenv\n");
        return EXIT_CODE;
    }
    char **p;
    for(p = envp ; *p ; p++) {
        if (putenv(*p) == ERROR) {
            printf("Error with putenv\n");
            return EXIT_CODE;
        }
    }
    printf("\nnew environ: \n");
    for(p = environ ; *p ; p++) {
        printf("%s\n", *p);
    }
    printf("\nresult of executing your command:\n");
    int callingCode = execvp(file, argv); // значение возвращается только при возникновении ошибки
    if (callingCode == ERROR) {
        environ = oldEnviron;
        perror("Execvp error");
        return EXIT_CODE;
    }
    return SUCCESS;
}

int main(int argc, char * argv[]){
    pid_t pid;
    char *command = argv[1];
    char *newArgv[] = {command, (char*)0};

    if (argc < NUM_OF_ARGS) {
        printf ("incorrect number of arguments\n");
        exit(EXIT_CODE);
    }
    pid = fork();

    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    }

    if (pid == CHILD) {
        int res = execvpe(command, newArgv, argv + 1);
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
    exit(SUCCESS);
}
