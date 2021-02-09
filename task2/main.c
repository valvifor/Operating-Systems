#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

extern char *tzname[];

int main() {
    time_t now;
    struct tm *time;
    putenv ("TZ=America/Los_Angeles");
    (void) time(&now);
    printf("%s", ctime(&now));
    time = localtime(&now);
    printf("%d/%d/%02d %d:%02d %s\n",
           time->tm_mon + 1, time->tm_mday, time->tm_year,
           time->tm_hour, time->tm_min,
           tzname[sp->tm_isdst]);
    exit(0);
}
