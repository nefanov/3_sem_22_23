#define SMALL_TEST

#define RED "\x1b[31m"
#define ESC "\x1b[0m"
#define NAME "MAYHEM"
#define DEF_KEY 0xCAFEBABE//0xDEAD
#define DEF_T 1488
#define SEMAPHORE_NAME "/EURONIMUS"
#define SEMA_NAME "/DEAD"

#ifdef SMALL_TEST
#define SIZE 256
#endif

#ifdef MEDIUM_TEST
#define SIZE 4096
#endif

#ifdef LARGE_TEST
#define SIZE 65536
#endif

#define SHM_SIZE 65536//67108864 //8388608 

#define DEFAULT_SIZE 1024