#ifndef __UCLINUX_TO_WINDOWS_H__
#define __UCLINUX_TO_WINDOWS_H__

#ifdef linux
#include <pthread.h>
#include <semaphore.h>
#define pthread_return              void*
#define DWORD                       int
#define PVOID                       void*
#define Sleep(a)                    sleep(a*1000)
#else
#include <windows.h>
#define off_t                      long
#define sem_t                      HANDLE
#define pthread_return             DWORD WINAPI
#define pthread_attr_t             SECURITY_ATTRIBUTES
#define pthread_t                  HANDLE
#define pthread_mutex_t            CRITICAL_SECTION
#define pthread_mutex_unlock(a)    LeaveCriticalSection( a )
#define pthread_mutex_destroy(a)   DeleteCriticalSection( a )
#define pthread_detach(a)
#define pthread_cancel(a)          CloseHandle(a)
#define pthread_testcancel()
#define pthread_exit(a)            return *((DWORD*)a)
#define pthread_cleanup_pop(a)
#define pthread_cleanup_push(a,b)
#define usleep(a)                  Sleep(a/1000);
#define sleep(a)                   Sleep(a*1000);
#define ioctl(a,b,c)               rand()

int pthread_mutex_lock( pthread_mutex_t * );
int pthread_mutex_init( pthread_mutex_t *, void * );
int pthread_mutex_trylock( pthread_mutex_t * );
int pthread_create(pthread_t *tid, const pthread_attr_t *tattr,
           LPTHREAD_START_ROUTINE start_routine, void *arg);

#endif

#endif
