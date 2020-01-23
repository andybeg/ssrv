// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "../CConverter.h"
#include <cstdio>
#include <netinet/tcp.h>
#define TCP_USER_TIMEOUT 2
#define TCP_PACKET_SIZE 100

Socket::Socket() :
  m_sock ( -1 )
{
  memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,
            SOCK_STREAM,
            0 );

  if ( ! is_valid() )
    return false;
  // TIME_WAIT - argh
  int on = 1;
  //set_non_blocking(true);
//  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
//    return false;
  return true;
}
bool Socket::close()
{
    char tmpBuf;
//    ::shutdown(listener,SHUT_RDWR);MSG_DONTWAIT
//    ::recv( m_sock, &tmpBuf, 1, 0 );
    return ::close ( m_sock );
}


bool Socket::bind ( const int port )
{
    if ( ! is_valid() )
    {
        return false;
    }
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons ( port );

    int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, 0 );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}


bool Socket::send ( std::string &s )
{
    s.insert(0,anyToStr(s.size(),sizeof(int)));
    s.insert(0,"ANS>",4);

    int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
    if ( status == -1 )
    {
        return false;
    }else
    {
        return true;
    }
/*    int c = s.size();

    for(int i = 0;i<c;i++)
    {
        int status = ::send ( m_sock, s.substr(TCP_PACKET_SIZE*i,TCP_PACKET_SIZE).c_str(), TCP_PACKET_SIZE, MSG_NOSIGNAL );
        CSession::Instance()->Collecting();
        if ( status == -1 )
        {
            return false;
        }else
        {
            return true;
        }
    }*/
}


int Socket::recv ( std::string& s ) const
{
#ifdef DEBUG
    int cnt;
#endif
    int bufCount = 0;

    std::string compare = "SND>";
    int status;

    s.clear();
    while( 1 ){
        char tmpBuf;
        status = ::recv ( m_sock, &tmpBuf, 1, 0 );//MSG_DONTWAIT
        //printf("status %d\n",status);
#ifdef DEBUG
//        printfd("%d|",status);
//        if( ((cnt++)%10)==0 ){
//            printf("\n");
//        }
#endif
        if(status>0){
            s.append(1,tmpBuf);

            //проверка правильности заголовка
            if( (s.size()==4) && (s.compare(compare)!=0) ){
                printf("header cutter\n");
                s.erase(0,1);
            }
            int pkgSize;
            if(s.size()==8){
                std::string tmp = s.substr(4,4);
                strToAny(tmp,pkgSize);
            }
            if( s.size()==(pkgSize+8)){
                s.erase(0,8);
                break;
            }
        }else{
            for(int i=0;i<s.size();i++)
                printf("0x%X|",s.data()[i]);
           break;
        }
    }
    if ( s.size() >0 )
    {
        return s.size();
    }
    return status;
}



bool Socket::connect ( const std::string host, const int port )
{
    if ( ! is_valid() ) return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );

    int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

    struct timeval tv;

    tv.tv_sec = 10;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(struct timeval));
    int timeout = 100;  // user timeout in milliseconds [ms]
    setsockopt (m_sock, SOL_TCP, TCP_USER_TIMEOUT, (char*) &timeout, sizeof (timeout));

    if ( status == 0 )
        return true;
    else
        return false;
}

void Socket::set_non_blocking ( const bool b )
{

  int opts;

  //opts = fcntl ( m_sock,
         //F_GETFL );

  //if ( opts < 0 )
    //{
      //return;
    //}

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
      F_SETFL,opts );
}
