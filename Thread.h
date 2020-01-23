#ifndef __THREAD_H__
#define __THREAD_H__

#include "Mutex.h"
#include "CVariable.h"
#include <cstdio>
#include <pthread.h>
#include <asm-generic/errno-base.h>
#include "linux2windows.h"

extern pthread_return _Thread( void* ptr );

class CThread {
    // Поток записи
    friend pthread_return _Thread( void* ptr );

private:
    CMutex    m_ThreadMutex;
    pthread_t m_ThreadId;
    CVariable <int> defDetachState;
    CVariable <int> defPolicy;
    CVariable <int> defPriority;

    // Виртуальная функция потока
    virtual void Thread( void ) = 0;
public:
    CThread() : m_ThreadMutex() {}

    // Запись идёт?
    bool IsThreadStarted( void ) {
        return m_ThreadMutex.IsLocked();
    }
    bool Wait(void){
        return m_ThreadMutex.WaitUnlocking();
    }
    // Начать запись в отдельном потоке
    int ThreadStart( void ) {
        int ret;
        // Заблокируем мютекс записи
        ret =  m_ThreadMutex.Lock();
        if ( ret ) {
            //System.Log( "Ошибка: Блокировки мютекса потока" );
            return ret;
        }

        // Создадим поток записи
        ret = RunThread( &m_ThreadId, &_Thread, PTHREAD_CREATE_DETACHED,SCHED_RR,50 );
        if ( ret ) {
            //System.Log( "Ошибка: Создание потока" );
            m_ThreadMutex.UnLock();
        }

        return ret;
    }
    void WaitThreadEnded(void)
    {
        pthread_join(m_ThreadId, NULL);
    }
    void SetThreadParam(int detachstate, int policy, int priority)
    {
        defDetachState.Set(detachstate);
        defPolicy.Set(policy);
        defPriority.Set(priority);
    }
    void SetDetachState(int detachstate)
    {
        defDetachState.Set(detachstate);
    }

    void SetPolicy(int policy)
    {
        defPolicy.Set(policy);
    }
    void SetPriority(int priority)
    {
        defPriority.Set(priority);
    }
    int RunThread(pthread_t *thread, pthread_return (*f)( void* ptr ),int detachstate, int policy, int priority)
    {
        int ret;

        pthread_attr_t attr;
        struct sched_param param;

        ret = pthread_attr_init( &attr );
        if ( ret ) {
            printf("инициализация pthread_attr_t\n" );
            return ret;
        }
        ret = pthread_attr_setinheritsched(&attr,  PTHREAD_EXPLICIT_SCHED );
        if ( ret ) {
            printf("задание отсоединённого потока\n" );
            return ret;
        }

        ret = pthread_attr_setdetachstate(&attr, defDetachState.Get(detachstate));
        if ( ret ) {
            printf("задание отсоединённого потока\n" );
            return ret;
        }


        ret = pthread_attr_setschedpolicy(&attr, defPolicy.Get(policy));
        if ( ret ) {
            printf("задание приоритета потока\n" );
            return ret;
        }

        param.sched_priority = defPriority.Get(priority);
        ret = pthread_attr_setschedparam(&attr, &param);
        if ( ret ) {
            printf("задание приоритета потока %d errno %d %d\n",param.sched_priority, ret, EINVAL);
            return ret;
        }

        ret = pthread_create( thread, &attr, (void* (*)(void*))f, this );
//        ret = pthread_create( thread, NULL, f, this );
        if ( ret ) {
            printf("Создание потока записи тренда\n" );
            return ret;
        }
//        ret = pthread_detach( *thread );

        ret = pthread_attr_destroy( &attr );
        if ( ret ) {
            printf("Инициализация pthread_attr_t\n" );
            return ret;
        }

        return ret;
    };


};

#endif
