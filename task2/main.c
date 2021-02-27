#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

extern char *tzname[];

int main() {
    time_t now;
    struct tm *times;
    putenv ("TZ=America/Los_Angeles");
    (void) time(&now);
    printf("%s", ctime(&now));
    times = localtime(&now);
    printf("%d/%d/%02d %d:%02d %s\n",
           times->tm_mon + 1, times->tm_mday, times->tm_year,
           times->tm_hour, times->tm_min,
           tzname[times->tm_isdst]);
    exit(0);
}
