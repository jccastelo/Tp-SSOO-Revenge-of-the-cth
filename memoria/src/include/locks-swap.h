#ifndef LOCKS_SWAP_H
#define LOCKS_SWAP_H

#include <pthread.h>

/**
*@brief Mutex para acceso exclusivo al archivo swap
*/
extern pthread_mutex_t mutex_swap_file;

/**
*@brief Mutex para proteger el bitmap de marcos
*/
extern pthread_mutex_t mutex_frames_bitmap;

/**
*@brief Read-Write lock para swap metadata
*/
extern pthread_rwlock_t rwlock_swap_metadata;


#endif