#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define ERROR -1

int main(int argc, char *argv[]) {
    printf("\nReal user ID: %d\n", getuid());
    printf("Effective user ID: %d\n", geteuid());
    FILE *file1 = fopen("file1.txt", "r");
    if (file1 == NULL) { 
        perror ("The file cannot be opened"); 
    } else {
        printf("The file opened successfully\n");
        fclose(file1); 
    }
    int checkID = seteuid(getuid());
    
    if (checkID == ERROR) { 
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
