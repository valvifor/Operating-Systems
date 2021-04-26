#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

#define EXIT_CODE 1
#define ERROR -1
#define SUCCESS 0

int execvpe(char *file, char *argv[], char *envp[]){
    char **oldEnviron = environ;
    environ = envp;
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
    char *newArgv[] = {"prog", (char*)0};
    char *newEnvp[3] = {"TZ=America/Los_Angeles", "PATH=/home/students/19200/v.petrova/task11", (char*)0};
    int res = execvpe(newArgv[0], newArgv, newEnvp);
    if (res == ERROR) {
        perror("execvpe error");
        exit(EXIT_CODE);
    }
    exit(SUCCESS);
}
