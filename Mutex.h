#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "linux2windows.h"

class CMutex {
private:
    pthread_mutex_t m_Mutex;
    pthread_mutexattr_t   m_Attr;

public:
    // Инициализация мютекса
    CMutex() {
        pthread_mutexattr_init(&m_Attr);
        pthread_mutexattr_settype(&m_Attr,PTHREAD_MUTEX_NORMAL);
        pthread_mutex_init( &m_Mutex, &m_Attr );
    }

    // Разрушение мютекса
    ~CMutex() {
        pthread_mutex_destroy( &m_Mutex );
        pthread_mutexattr_destroy(&m_Attr);
    }

    // Вход в критическую секцию
    int Lock() {
        return pthread_mutex_lock( &m_Mutex );
    }

    // Выход из критической секции
    void UnLock( void ) {
        pthread_mutex_unlock( &m_Mutex );
    }

    // Проверка - мютекс блокирован?
    bool IsLocked( void ) {
        // Попытка заблокировать мютекс
        if ( pthread_mutex_trylock( &m_Mutex ) )
            // Мютекс блокирован
            return true;

        // Разблокируем мютекс
        pthread_mutex_unlock( &m_Mutex );

        // Мютекс не блокирован
        return false;
    }
    // Проверка - мютекс блокирован?
    bool WaitUnlocking( void ) {
        // Попытка заблокировать мютекс
        if(!pthread_mutex_lock( &m_Mutex ) )
        {
            pthread_mutex_unlock( &m_Mutex );
            return true;
        }
        return false;
    }

};

#endif
