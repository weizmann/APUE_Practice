/*
 * =====================================================================================
 *
 *       Filename:  rw_lock.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/09 16时46分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pthread.h>
#include <string.h>
#include <stdio.h>

#include "rw_lock.h"

int rwlock_init(rwlock_t* rw, rwlock_attr_t* attr) {
	int result;

	if (attr != NULL) {
		return -1;
	}

	if ((result = pthread_mutex_init(&rw->rw_mutex, NULL)) != 0) {
		goto error_handler_1;
	}
	if ((result = pthread_cond_init(&rw->rw_cond_readers, NULL)) != 0) {
		goto error_handler_2;
	}
	if ((result = pthread_cond_init(&rw->rw_cond_writers, NULL)) != 0) {
		goto error_handler_3;
	}

	rw->rw_magic = RW_MAGIC;
	rw->rw_nwaitreaders = 0;
	rw->rw_nwaitwriters = 0;
	rw->rw_refcount = 0;
	return 0;

error_handler_3:
	pthread_cond_destroy(&rw->rw_cond_readers);
error_handler_2:
	pthread_mutex_destroy(&rw->rw_mutex);
error_handler_1:
	return result;
}

int rwlock_destory(rwlock_t* rw) {
	if (rw->rwmagic != RW_MAGIC) {
		printf("destory before initializing...\n");
		return EINVAL;
	}

	if (rw->rw_refcount != 0 || rw->rw_nwaitreaders != 0
			|| rw->rw_nwaitwriters != 0) {
		return EBUSY;
	}

	pthread_mutex_destroy(&rw->rw_mutex);
	pthread_cond_destroy(&rw->rw_cond_readers);
	pthread_cond_destroy(&rw->rw_cond_writers);
	rw->rw_magic = 0;

	return 0;
}

int rwlock_rdlock(rwlock_t* rw) {
	int result;
	
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	while(rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) {
		rw->rw_nwaitreaders++;
		result = pthread_cond_wait(&rw->rw_cond_readers, &rw->rw_mutex);
		rw->rw_nwaitreaders--;
		if (result != 0) {
			break;
		}
	}

	if (result == 0) {
		rw->rw_refcount++;
	}
	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int rwlock_tryrdlock(rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	if (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) {
		result = EBUSY;
	} else {
		rw->rw_refcount++;
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int rwlock_wrlock(rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	while(rw->rw_refcount != 0) {
		rw->rw_nwaitwriters++;
		result = pthread_cond_wait(&rw->rw_cond_writers, &rw->rw_mutex);
		rw->rw_nwaitwriters--;
		if (result != 0) {
			break;
		}
	}

	if (result == 0) {
		rw->rw_refcount = -1;
	}
	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int rw_trywrlock(rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}
	if (rw->rw_refcount != 0) {
		result = EBUSY;
	} else {
		rw->rw_refcount = -1;
	}
	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int rwlock_unlock(rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	if (rw->rw_refcount > 0) {
		rw->refcount--;
	} else if (rw->rw_refcount == -1) {
		rw->rw_refcount = 0;
	} else {
		printf("error in the rw_refcount...\n");
	}

	if (rw->rw_nwaitwriters > 0) {
		if (rw->rw_refcount == 0) {
			result = pthread_cond_signal(&rw->rw_cond_writers);
		}
	} else if (rw->rw_nwaitreaders > 0) {
		result = pthread_cond_broadcast(&rw->rw_cond_readers);
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}
























