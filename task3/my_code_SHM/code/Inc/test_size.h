

#define MASSAGE_SIZE 4000

#define MB_40


#ifdef TEST_FILE
#define FILE_NAME "../test_input"
#define FILE_SIZE 10
#endif


#ifdef MB_40
#define FILE_NAME "../40Mb"
#define FILE_SIZE 42991610
#endif

#ifdef MB_400
#define FILE_NAME "../400Mb"
#define FILE_SIZE 428867584
#endif

#ifdef GB_4
#define FILE_NAME "../4Gb"
#define FILE_SIZE 4294967290
#endif


#define SHM_NAME "IVAN"

#define SEMAFOR_A "FF"//"A"
#define SEMAFOR_B "UU"//"B"