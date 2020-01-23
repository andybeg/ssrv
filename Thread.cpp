#include "Thread.h"

pthread_return _Thread( void* ptr ) {
    CThread* pTS = (CThread*)ptr;
    // Вызовем целевую функци записи потомка
    pTS->Thread();
    // Разблокируем мутекс записи
    pTS->m_ThreadMutex.UnLock();
    pthread_exit(NULL);
#ifndef __linux__
    return 0;
#else
    return ptr;
#endif
}
