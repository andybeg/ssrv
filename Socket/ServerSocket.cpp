// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include <cstdio>

ServerSocket::ServerSocket ( int port )
{
    if ( ! Socket::create() )
    {
        throw SocketException ( "Could not create server socket." );
    }

    if ( ! Socket::bind ( port ) )
    {
        throw SocketException ( "Could not bind to port." );
    }

    if ( ! Socket::listen() )
    {
        throw SocketException ( "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


ServerSocket& ServerSocket::operator << ( std::string& s )
{
    int res = Socket::send ( s );
    //printf("snd %d",res);
    if ( (!res) || (res == -1) )
    {
        throw SocketException ( "Could not write to socket." );
    }

  return *this;
}


ServerSocket& ServerSocket::operator >> ( std::string& s )
{
    int res = Socket::recv ( s );
    //printf("recv %d",res);
    if ( (!res) || (res == -1) )
    {
        throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

void ServerSocket::accept ( ServerSocket& sock )
{
    int res = Socket::accept ( sock );
    if ( (!res) || (res == -1) )
    {
        throw SocketException ( "Could not accept socket." );
    }
}
