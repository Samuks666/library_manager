#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_INFO(fmt, ...) \
    log_register_internal("INFO", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    log_register_internal("ERROR", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define REALLOCATION_ERROR "Reallocation Failed"
#define ALLOCATION_ERROR "Allocation Failed"
#define NULL_ERROR "NULL Argument"
#define EXPAND_CAPACITY "Expand Capacity Failed"

void log_register_internal(
    const char *level,
    const char *file,
    int line,
    const char *func,
    const char *fmt,
    ...
);
void log_read();
void log_reset();

#ifdef __cplusplus
}
#endif

#endif