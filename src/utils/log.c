#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "log.h"

#define LOG_FILE "library.log"


void log_register_internal(
    const char *level,
    const char *file,
    int line,
    const char *func,
    const char *fmt,
    ...
) {
    
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) {
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (!t) {
        fclose(log);
        return;
    }

    fprintf(log,
        "%04d-%02d-%02d %02d:%02d:%02d | %s | %s:%d (%s) | ",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        level,
        file,
        line,
        func
    );

    va_list args;
    va_start(args, fmt);

    vfprintf(log, fmt, args);

    va_end(args);

    fprintf(log, "\n");

    fclose(log);
}

void log_read() {
    FILE *log = fopen("library.log", "r");

    if(log == NULL) return;

    char *line[256];

    while(fgets(line, sizeof(line), log)) {
        printf("%s", line);
    }

    fclose(log);
}
 
void log_reset(){
    FILE *log = fopen("library.log", "w");
    if (!log) return;

    fclose(log);
}