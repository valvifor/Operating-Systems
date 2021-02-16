#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

extern char *tzname[];

int main() {
    time_t now; // Определяет переменную now типа time_t.
    struct tm *times; // Определяет указатель sp на struct tm.
    if (putenv ("TZ=America/Los_Angeles")) {
        perror("Error changing TZ variable\n");
    }
    (void) time(&now); // Вызывается time.  Время в  секундах  от  00:00:00  UTC  1 января 1970 сохраняется в переменной now.
    printf("%s", ctime(&now)); // Библиотечная функция ctime(3C) преобразует календарное времяв ASCII-строку  формата date(1).
    // Адрес,  возвращенный этой функцией,  используется  в  качестве  параметра  printf  для печати ASCII-строки.
    times = localtime(&now); // Вызывается библиотечная  функция localtime(3C).  Эта функция заполняет значениями поля структуры tm.
    printf("%d/%d/%02d %d:%02d %s\n",
         times->tm_mon + 1, times->tm_mday, times->tm_year,
         times->tm_hour, times->tm_min,
         tzname[times->tm_isdst]); // Распечатываются значения полей структуры tm.
    exit(0);
}
