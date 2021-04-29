#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define ERROR -1
#define ERR_ISATTY 0

char reply(char *question) {
    char answer;
    struct termios termReserve, term;
    int fd = 0;
    printf("%s (y/n)?\n", question);
    if(fflush(stdout) == EOF) {
        perror("fflush error");
        return ERROR;
    }
    if (tcgetattr(fd, &term) == ERROR) {
        perror("tcgetattr error");
        return ERROR;
    }
    termReserve = term;
    int count = 1;
    term.c_lflag &= ~(ISIG | ICANON);
    term.c_cc[VMIN] = count;
    if (tcsetattr(fd, TCSAFLUSH, &term) == ERROR) {
        perror("tcsetattr error");
        return ERROR;
    }
    if (read(fd, &answer, count) == ERROR) {
        perror("read error");
        return ERROR;
    }
    if (tcsetattr(fd, TCSANOW, &termReserve) == ERROR) {
        perror("tcsetattr error");
        return ERROR;
    }
    return answer;
}

int main(){
    char answer;
    int fd = 0;
    if(isatty(fd) == ERR_ISATTY){
        perror("Issatty error");
        exit(EXIT_FAILURE);
    }
    answer = reply("Is this task number 16?");
    printf("\n");
    if (answer == ERROR) {
        exit(EXIT_FAILURE);
    }
    switch (answer) {
        case 'y':
            printf("Your answer is yes\n");
            break;
        case 'n':
            printf("Your answer is no\n");
            break;
        default:
            printf("Invalid reply\n");
            break;
    }
    exit(EXIT_SUCCESS);
}

//tcflag_t c_iflag;      /* режимы ввода */
//tcflag_t c_oflag;      /* режимы вывода */
//tcflag_t c_cflag;      /* режимы управления */
//tcflag_t c_lflag;      /* локальные режимы */
//cc_t     c_cc[NCCS];   /* специальные символы */
