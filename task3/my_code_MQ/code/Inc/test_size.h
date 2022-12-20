

#define MASSAGE_SIZE 4000

#define MB_40

#ifdef TEST_FILE
#define FILE_NAME "../test_input"
#define FILE_SIZE 20
#endif

#ifdef MB_40
#define FILE_NAME "../40Mb"
#define FILE_SIZE 42991616
#endif

#ifdef MB_400
#define FILE_NAME "../400Mb"
#define FILE_SIZE 400000000
#endif

#ifdef GB_4
#define FILE_NAME "../4Gb"
#define FILE_SIZE 4294967296
#endif


#define KEY  508
#define TYPE 0xDED