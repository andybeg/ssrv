#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "linux2windows.h"

class CMutex {
private:
    pthread_mutex_t m_Mutex;
    pthread_mutexattr_t   m_Attr;

public:
    // ������������� �������
    CMutex() {
        pthread_mutexattr_init(&m_Attr);
        pthread_mutexattr_settype(&m_Attr,PTHREAD_MUTEX_NORMAL);
        pthread_mutex_init( &m_Mutex, &m_Attr );
    }

    // ���������� �������
    ~CMutex() {
        pthread_mutex_destroy( &m_Mutex );
        pthread_mutexattr_destroy(&m_Attr);
    }

    // ���� � ����������� ������
    int Lock() {
        return pthread_mutex_lock( &m_Mutex );
    }

    // ����� �� ����������� ������
    void UnLock( void ) {
        pthread_mutex_unlock( &m_Mutex );
    }

    // �������� - ������ ����������?
    bool IsLocked( void ) {
        // ������� ������������� ������
        if ( pthread_mutex_trylock( &m_Mutex ) )
            // ������ ����������
            return true;

        // ������������ ������
        pthread_mutex_unlock( &m_Mutex );

        // ������ �� ����������
        return false;
    }
    // �������� - ������ ����������?
    bool WaitUnlocking( void ) {
        // ������� ������������� ������
        if(!pthread_mutex_lock( &m_Mutex ) )
        {
            pthread_mutex_unlock( &m_Mutex );
            return true;
        }
        return false;
    }

};

#endif
