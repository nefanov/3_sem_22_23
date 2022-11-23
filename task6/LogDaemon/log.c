#include <logs.h>
#include <stdarg.h>

static FILE* log_child = NULL;
static FILE* log_parent = NULL;
static FILE* log_file = NULL;

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
    log_file = stdout;
    log("Logs ctor");

    // log_child  = fopen("child.log", "w");
    // log_parent = fopen("parent.log", "w");
}

void logs_dtor ()
{
    log("Logs dtor");

    // fclose(log_child);
    // log_child = NULL;

    // fclose(log_parent);
    // log_parent = NULL;
}

void _log (FILE* log_file, const char* format, va_list args)
{
    vfprintf(log_file, format, args);
    fputc('\n', log_file);
    fflush(log_file);
}

void log (const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _log(stdout, format, args);
    fprintf(stdout, "%s\n", strerror(errno));
    fputc('\n', stdout);
    va_end(args);
}

void logc (const char* format, ...)
{

    va_list args;
    va_start(args, format);
    _log(log_child, format, args);
    va_end(args);
}

void logp (const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _log(log_parent, format, args);
    va_end(args);
}
