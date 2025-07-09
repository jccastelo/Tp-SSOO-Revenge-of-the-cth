#include "../include/locks-swap.h"

pthread_mutex_t mutex_swap_file = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_frames_bitmap = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock_swap_metadata = PTHREAD_RWLOCK_INITIALIZER;

