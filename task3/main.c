#include <sys/types.h>
//файл библиотеки. В этом файле объявляются системные типы данных, такие как u_int, u_char... В нашем случае нужен для uid_t -- user IDs.
#include <unistd.h>
//unistd.h -- заголовочный файл объявляет ряд специфичных для реализации функций: 	getuid(), 	geteuid(), setuid()...
#include <stdio.h>
//заголовочный файл ввода-вывода.
#define ERROR -1
//Директива #define определяет идентификатор и последовательность символов, которые будут подставляться вместо идентификатора каждый раз, когда он встретится

int main(int argc, char *argv[]) {
    printf("\nReal user ID: %d\n", getuid());
    /*Функция printf() записывает в stdout строку "Real user ID:" и значение, полученное функцией getuid().
    %d -- спецификатор формата (десятичного числа целого типа со знаком).
    getuid() возвращает фактический идентификатор (ID) пользователя вызывающего процесса.
    geteuid() возвращает эффективный идентификатор (ID) пользователя в текущем процессе.
     */
    printf("Effective user ID: %d\n", geteuid());
    FILE *file1 = fopen("file1.txt", "r");
    //создание объекта типа FILE, открытие файла под названием file1.txt для чтения
    if (file1 == NULL) { // проверка на успешное открытие файла
        perror ("The file cannot be opened"); // Функция perror() выводит сообщение об ошибке в stderr
    } else {
        printf("The file opened successfully\n");
        fclose(file1); //при успешном открытии файла выводим в stdout строку "The file opened successfully\n" и закрываем файл, с которым работали
    }
    int checkID = seteuid(getuid());
    //  seteuid -- установка идентификаторов пользователя
    if (checkID == ERROR) { //если функция выполнена с ошибкой
        perror("This id cannot be set");
    }
    printf("\nReal user ID: %d\n", getuid());
    printf("Effective user ID: %d\n", geteuid());
    FILE *file2 = fopen("file2.txt", "r");
    //создание объекта типа FILE, открытие файла под названием file2.txt для чтения
    if (file2 == NULL) { // проверка на успешное открытие файла
        perror ("The file cannot be opened"); // perror() записывает в stderr ошибку
    } else {
        printf("The file opened successfully\n");
        fclose(file2); //при успешном открытии файла выводим в stdout строку "The file opened successfully\n" и закрываем файл, с которым работали
    }
    return 0;
}