/*
 * cmdinterpreter.cpp
 *
 * Copyright 2014 kon <kon@kononov>
 *
 *
 *
 */

#include <fcntl.h>
#include "CCmdInterpreter.h"
#include "./Socket/SocketException.h"
#include "./Socket/ServerSocket.h"
#include <math.h>
#include <iostream>

CCmdInterpreter::CCmdInterpreter()
{

}

CCmdInterpreter::~CCmdInterpreter()
{
}
void CCmdInterpreter::run()
{
    SetThreadParam(PTHREAD_CREATE_DETACHED, SCHED_RR, 1);
    ThreadStart();
}
char const * CCmdInterpreter::ShortAnsErr[CMD_ERR_COUNT] = {
        "OK",
        "UE",
        "WC",
        "UC",
        "HW",
        "SW",
        "VE",
        "PE",
        "IS",
        "UN",
        "ND"
    };

//=============================================================================
//поток обмена по сети
//=============================================================================
void CCmdInterpreter::Thread( void ){

    //CSystem::Instance()->Log(CLogger::Info,"TCP сервер запущен");
    try{
        // Create the socket
        ServerSocket server ( 30000 );
        try{
            while ( true ){
                ServerSocket new_sock;
                server.accept ( new_sock );
                //клиент подключился
                try{
                    while ( true ){
                        std::string m_Received;
                        new_sock >> m_Received;
                        printf(">> %s size %d\n",m_Received.substr(0,4).c_str(),m_Received.size());
                        if(m_Received.size()>0){
                            std::string m_Send;
                            //mkAnswer(m_Send,m_Received);
                            printf("<< %s size %d\n",m_Send.substr(0,2).c_str(),m_Send.size());
                            //for(int i=0;i<m_Send.size();i++)
                                //printf("|0x%X",m_Send.data()[i]);
                            //printf("\n");
                            new_sock << m_Send;
                        }
                    }
                    usleep(50000);
                }
                catch ( SocketException& ){
                    //CSystem::Instance()->Log(CLogger::Info,"SocketException %s");
                }
                //CSystem::Instance()->Log(CLogger::Info,"Клиент отключился");
            }
        }
        catch ( SocketException& ){
            //CSystem::Instance()->Log(CLogger::Info,"SocketException0");
        }
    }
    catch ( SocketException& e ){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
}
void CCmdInterpreter::ShortAnswere(int err,std::string &send){
    send = ShortAnsErr[err];
    //printf("ShortAnsErr %s\n",ShortAnsErr[err]);
}

void CCmdInterpreter::BASH(const std::string &data, std::string &buf)
{
    std::string tmp;
    int size = data.size();
    tmp = data.substr(4,size - 4);
    char buff[BUFSIZ];
    FILE *ptr;

    if ((ptr = popen(tmp.c_str(), "r")) != NULL)
        while (fgets(buff, BUFSIZ, ptr) != NULL)
        {
            buf.append(buff);
        }
        (void) pclose(ptr);
}

