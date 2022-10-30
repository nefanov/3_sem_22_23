#define MEDIUM_TEST

#define RED "\x1b[31m"
#define ESC "\x1b[0m"
#define NAME "MAYHEM"
#define DEF_KEY 1493
#define DEF_T 1488

#ifdef SMALL_TEST
#define SIZE 256
#endif

#ifdef MEDIUM_TEST
#define SIZE 4096
#endif

#ifdef LARGE_TEST
#define SIZE 65536
#endif

#define SHM_SIZE 134217728 //67108864 //8388608 

#define DEFAULT_SIZE 1024