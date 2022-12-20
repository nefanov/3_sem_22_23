#include "header.hpp"
#include "crctable.hpp"


extern int HIT;
extern pthread_mutex_t mutex;


size_t hash_crc_32 (void *ptr, size_t len) {
    
    char *str = (char *)ptr;

    size_t key = 0xFFFFFFFFUL;

    for (int i = 0; i < len; ++i) {

        key = CRCTable [(key ^ *(str)) & 0xFF] ^ (key >> 8);
        str++;
    }
    return key;
}


double func (double x) {
    return (x * x);
}

bool equal (double x, double y) {
    return (fabs (x - y) < EPS);
}

int find_local_hits (Args *args) {

/////////////////////////////////////////////////GENERATE SEED
    pthread_t id = pthread_self ();

    unsigned int seed = hash_crc_32 (&id, sizeof (pthread_t));

    srand(seed);

/////////////////////////////////////////////////TRY HIT A POINT
    int hit = 0;
    int count = POINT_NUM / THREAD_NUM;
    
    for (int i = 0; i < count; ++i) {
        double cur_x = (double) rand_r (&seed) / RAND_MAX * (args->max_x - args->min_x) + args->min_x;  
        double cur_y = (double) rand_r (&seed) / RAND_MAX * (args->max_y - args->min_y) + args->min_y;
        
        hit += (cur_y <= args->func (cur_x)); 
    }

    return hit;
}

void *find_all_hits (void* data) {

    int cur = find_local_hits ((Args *)data);

    pthread_mutex_lock  (&mutex);
        HIT += cur;
    pthread_mutex_unlock(&mutex);
}
