#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h> //для getopt()
#include <ulimit.h>
#include <stdlib.h>
#include <stdio.h>
#define ERROR -1

extern char **environ;
extern  char *optarg; //указатель на символ. Когда getopt() обрабатывает опцию, у которой есть аргументы, optarg содержит адрес этого аргумента.
extern int optind, opterr, optopt; //Optind – индекс на следующий указатель argv, который будет обработан при следующем вызове ф-и
//Optopt – последний из известных параметров

/*
 * Когда getopt() возвращает -1, argv[optind] указывает на первый аргумент не-опцию.
 * Когда getopt() обрабатывает недопустимые опции, сообщение об ошибке выводится на стандартный вывод диагностики. Печать может быть подавлена установкой opterr в ноль.
 * Когда getopt() возвращает '?', optopt содержит значение недопустимой опции.
 */

int main(int argc, char *argv[]){
    int curArg, uLimitResult;
    char options[] = "ispuU:cC:dvV:"; // Создание строки допустимых опций. Опции, за которыми следует двоеточие, требуют соответствующих параметров
    struct rlimit rlp;
    char **curEnvVar;
    long int arg;
    char *dir = NULL;

    if(argc < 2) { // роверка на корректное количество аргументов
        fprintf(stderr,"Usage: %s options\n", argv[0]);
        exit(0);
    }

    //Вход в цикл вызова getopt() для просмотра командной строки, возвращается одна опция за один проход  цикла.
    while ((curArg = getopt(argc, argv, options)) != ERROR) { // getopt - выделить опцию из командной строки
        switch (curArg) {
            case 'i': //Печатает реальные и эффективные идентификаторы пользователя и группы.
                printf("Real UID: %ld\n", getuid());
                printf("Effective UID: %ld\n", geteuid());
                printf("Real GID: %ld\n", getgid());
                printf("Effective GID: %ld\n", getegid());
                break;

            case 's': //Процесс становится лидером группы.
                if (setpgid(0, 0) == ERROR) { //setpgid(pid_t pid, pid_t pgid);устанавливает PGID у процесса с идентификатором pid равным pgid [ setpgrp()]
                    perror("Error [-s]");
                } else {
                    printf("%d is now leader of group\n", getpid()); //getpid() возвращает идентификатор вызвавшего процесса
                }
                break;

            case 'p': //Печатает иидентификаторы процесса, процесса-родителя и группы процессов.
                printf("Process ID: %ld\n", getpid());//getpid() возвращает идентификатор вызвавшего процесса
                printf("Parent PID: %ld\n", getppid());//возвращает идентификатор процесса, являющегося родительским по отношению к вызвавшему процессу.
                printf("Group PID: %ld\n", getpgid(0));//возвращает PGID процесса с заданным pid. Если значение pid равно нулю, то используется идентификатор вызывающего процесса
                break;

            case 'u': //Печатает значение ulimit // ограничения текущего процесса, например, количество одновременно открытых файлов.
                uLimitResult = ulimit(UL_GETFSIZE);// UL_GETFSIZE -- Возвращается ограничение на размер файла. Вызов ulimit позволяет получить или установить ограничения вызывающего процесса
                if (uLimitResult == ERROR) {
                    perror ("Error [-u]");
                } else {
                    printf("ulimit = %ld\n",uLimitResult);
                }
                break;

            case 'U': //Изменяет значение ulimit.
                arg = atol(optarg); // преобразовывает строку string в длинное целое
                //optarg – указатель на текущий аргумент, если таковой имеется.
                if (arg < 0 || arg > LONG_MAX) {
                    printf ("Wrong argument [-U]\n");
                } else {
                    uLimitResult = ulimit(UL_SETFSIZE, arg); // UL_SETFSIZE -- Устанавливается ограничение размера файла.
                    if (uLimitResult == ERROR) {
                        perror("Must be super-user to increase ulimit\n");
                    }
                }
                break;

            case 'c': //Печатает размер в байтах core-файла, который может быть создан.
                if (getrlimit(RLIMIT_CORE, &rlp) == ERROR) { //считывает ограничения использования ресурсов
                    //RLIMIT_CORE Максимальный размер файла core
                    perror("Error [-c]");
                } else {
                    printf("core size = %ld\n", rlp.rlim_cur);
                }
                break;

            case 'C': //Изменяет размер core-файла
                if (getrlimit(RLIMIT_CORE, &rlp) == ERROR) { //считывает ограничения использования ресурсов
                    //RLIMIT_CORE Максимальный размер файла core
                    perror("Error [-c]");
                } else {
                    rlp.rlim_cur = atol(optarg);
                    if (setrlimit(RLIMIT_CORE, &rlp) == ERROR)
                        perror("Must be super-user to increase core\n");
                }
                break;

            case 'd': //Печатает текущую рабочую директорию
                dir = getcwd(NULL, 100);
                printf("Current working directory is: %s\n", dir); //getcwd() копирует полный путь (максимум len символов) текущего рабочего каталога диска в строку, на которую указывает параметр dir.
                free(dir);
                break;

            case 'v': //Распечатывает переменные среды и их значения
                printf("Environment variables are:\n");
                for (curEnvVar = environ; *curEnvVar; curEnvVar++)
                    printf("%s\n", *curEnvVar);
                break;

            case 'V': //Вносит новую переменную в среду или изменяет значение существующей переменной
                if (putenv(optarg) == ERROR) {
                    perror("Error [-V]");
                }
                break;

            case '?': // Если обнаружена недопустимая опция, getopt() вернет '?' и выдаст сообщение об ошибке на стандартный вывод диагностики. Выдача
                //сообщения может быть выключена установкой opterr в ноль. getopt содержит значение недопустимой опции.
                printf("Invalid option is %c\n", optopt);
                break;
        }
    }
}