#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define LOG(...) do { fprintf(stdout, "[INFO]" __VA_ARGS__); putc('\n', stdout); } while(0)

static FILE* log_rcv = NULL;
static FILE* log_snd = NULL;

void logs_ctor ();
void logs_dtor ();

void __attribute__ ((constructor)) premain()
{
    logs_ctor();
}

void __attribute__ ((destructor)) postmain()
{
    logs_dtor();
}

void logs_ctor ()
{
    LOG("Logs ctor");

    log_rcv = stdout; //fopen("rcv.log", "w");
    log_snd = stdout; //fopen("snd.log", "w");
}

void logs_dtor ()
{
    LOG("Logs dtor");

    fclose(log_rcv);
    log_rcv = NULL;

    fclose(log_snd);
    log_snd = NULL;
}

void log (FILE* log_file, const char* format, va_list args)
{
    vfprintf(log_file, format, args);
    fputc('\n', log_file);
    fprintf(log_file, "%s\n", "Errors:");
    fprintf(log_file, "%s\n", strerror(errno));
    fflush(log_file);
}

void logr (const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(log_rcv, format, args);
    va_end(args);
}

void logs (const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(log_snd, format, args);
    va_end(args);
}