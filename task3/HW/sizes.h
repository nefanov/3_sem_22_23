#define DEF_SIZE 512 //variable to 4096 and 65536 in S and L bufsizes

#ifdef S_SIZE 
#define SIZE 4096 
#endif

#ifdef M_SIZE 
#define SIZE 104857600
#endif

#ifdef L_SIZE 
#define SIZE 2147483648
#endif

