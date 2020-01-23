#include "uclinux2windows.h"


int pthread_mutex_init( pthread_mutex_t *__mutex, void *ptr ) {
    InitializeCriticalSection( __mutex );
    return 0;
}


WINBASEAPI
BOOL
WINAPI
TryEnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection
    );


int pthread_mutex_lock( pthread_mutex_t *__mutex ) {
    EnterCriticalSection( __mutex );
    return 0;
}


int pthread_mutex_trylock( pthread_mutex_t *__mutex ) {
    if ( TryEnterCriticalSection( __mutex ) ) return 0;
    return 1;
}


int pthread_create(pthread_t *tid, const pthread_attr_t *tattr,
                   LPTHREAD_START_ROUTINE start_routine, void *arg) {
    DWORD dwCreateThread;
    *tid = CreateThread( NULL, 0, start_routine, arg, 0, &dwCreateThread );
    if ( *tid ) return 0;
    return 1;
}

/*
int sem_init(sem_t *sem, int pshared, int value) {
    *sem = CreateSemaphore( NULL, 0, 1, "");
    if ( *sem ) return 0;
    return 1;
}
*/
