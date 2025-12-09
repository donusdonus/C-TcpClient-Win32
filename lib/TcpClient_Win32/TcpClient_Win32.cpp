
#include "TcpClient_Win32.h"
#include "IPAddress.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

TcpClient_Win32::TcpClient_Win32(){

}

TcpClient_Win32::~TcpClient_Win32(){
    Close();
}


bool TcpClient_Win32::Init()
{
    int tmp = 0 ;

    SocketReadytoUse = true;

    /* Create TCP Socket */
    if(_socket == INVALID_SOCKET)
        _socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    tmp = (_socket == INVALID_SOCKET);
    if(tmp)
        SocketReadytoUse = false;

    
    return SocketReadytoUse ;
}

bool TcpClient_Win32::Close()
{
    if(_socket != INVALID_SOCKET)
    {
        shutdown(_socket,SD_BOTH);
        closesocket(_socket);
        _socket = INVALID_SOCKET;   
    }
    return true;
}

int TcpClient_Win32::connect(IPAddress ip, uint16_t port)
{
    
    return 0;
}

int TcpClient_Win32::connect(IPAddress ip, uint16_t port, int32_t timeout_ms)
{
    return 0;
}


int TcpClient_Win32::connect(const char *host, uint16_t port, int32_t timeout_ms = 1000)
{
    int tmp = 0;
    char port_str[7];

    /* Connecting Host */
    addrinfo details = {};
    addrinfo* result = nullptr;

    /* Init Socket */
    tmp = Init();
    if(tmp == false)
        return 0;

    /* prepare socket config */
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);

    /* check is ip or hostname */
    tmp = inet_pton(AF_INET,host,&_addr.sin_addr);

    /* is not ipaddress*/
    if(tmp == false)
    {
        details.ai_family = AF_INET;
        details.ai_socktype = SOCK_STREAM;
        sprintf(port_str,"%d",port);

        /* get hostaddr */
        tmp = getaddrinfo(host,port_str,&details,&result);

        if(tmp != 0 || result == nullptr)
        {
            closesocket(_socket);
            return 0;   
        }

        /* Copy host address */
        memcpy(&_addr,result->ai_addr,sizeof(_addr));

        /* Clear result */
        freeaddrinfo(result);
    }



    /* Enable NonBlocking */
    tcp_block_mode = TCP_MODE_NONBLOCKING;
    ioctlsocket(_socket,FIONBIO,(u_long*)&tcp_block_mode);

    /* Connnecting */
    tmp = ::connect(_socket,(sockaddr*)&_addr,sizeof(_addr));
    if(tmp == 0) 
    {
        /* Connected */
    }
    else
    {
        /* Check Some Error */
        int err = WSAGetLastError();
        if((err != WSAEWOULDBLOCK)&&(err != WSAEINPROGRESS))
        {
            Close();
            return 0;
        }

        /********* Process Wait Timeout ***************/

        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(_socket,&wfds);

        timeval tv;
        tv.tv_sec = TCPCLIENT_TIMEOUT_CONNECITON;
        tv.tv_usec = 0;

        tmp = select(0,NULL,&wfds,NULL,&tv);
        if(tmp <= 0)
        {
            // timeout error
            Close();
            return 0;
        }

        /********* Check Connection ***************/
        int so_error = 0;
        int optlen   = sizeof(so_error);
        getsockopt(_socket, SOL_SOCKET, SO_ERROR, (char*)&so_error, &optlen);
        if (so_error != 0)
        {
            // timer error
            Close();
            return 0;
        }
    }

    /* Disable NonBlocking */
    tcp_block_mode = TCP_MODE_BLOCKING;
    ioctlsocket(_socket,FIONBIO,&tcp_block_mode);

    return 1;
}

size_t TcpClient_Win32::write(uint8_t data)
{
    return 0;
}

size_t TcpClient_Win32::write(const uint8_t *buf, size_t size)
{
     if (_socket == INVALID_SOCKET || buf == nullptr || size == 0)
        return 0;

    int tmp = 0 ;
    int tcnt = 0;
    const int tout = 100; 
    bool cancel = false;

    /* Enable NonBlocking */
    tcp_block_mode = TCP_MODE_NONBLOCKING;
    ioctlsocket(_socket,FIONBIO,(u_long*)&tcp_block_mode);

    while((tmp < size) && (tcnt <= 10) && (cancel != true))
    {
        int buffersend = send(_socket,(char*)buf+tmp,size-tmp,0);

        /* Reference Return send(...) Keyword "WSABASEERR" */

        if(buffersend == 0)
        {
            cancel = true ;
            /* Connection Close */
            //return 0 ;
        }
        else if(buffersend > 0)
        {
            /* Buffer Update */
            tmp+=buffersend;
        }
        else
        {
            /* Check Socket Ready */
            //int _socket_err = WSAGetLastError();

            /* Wait Socket Ready */
            //if(_socket_err == WSAEWOULDBLOCK)
            //{
                fd_set wfds;
                FD_ZERO(&wfds);
                FD_SET(_socket,&wfds);

                timeval tv;
                /* Check Interval 100 ms */
                tv.tv_sec = 0;
                tv.tv_usec = 100000;

                select(0,NULL,&wfds,NULL,&tv);
                tcnt+=1;
            //}

        }

    }

    /* Disable NonBlocking */
    tcp_block_mode = TCP_MODE_BLOCKING;
    ioctlsocket(_socket,FIONBIO,&tcp_block_mode);

    return tmp;
}


void TcpClient_Win32::flush()
{

}

int TcpClient_Win32::available()
{
    u_long byteIncome = 0;

    ioctlsocket(_socket,FIONREAD,&byteIncome);

    return (int)byteIncome;
}


size_t TcpClient_Win32::readBytes(char *buffer, size_t length)
{
    int tmp = 0 ;
    tmp = recv(_socket,buffer,length,0);
    return 0;
}

size_t TcpClient_Win32::readBytes(uint8_t *buffer, size_t length)
{ 
    return readBytes((char *)buffer, length); 
}

int TcpClient_Win32::peek()
{
    return 0;
}

void TcpClient_Win32::clear()
{

}

void TcpClient_Win32::stop()
{

}

uint8_t TcpClient_Win32::connected()
{
    return 0;
}

int TcpClient_Win32::setSocketOption(int option, char *value, size_t len)
{
    return 0;
}

int TcpClient_Win32::setSocketOption(int level, int option, const void *value, size_t len)
{
    return 0;
}

int TcpClient_Win32::getSocketOption(int level, int option, const void *value, size_t size)
{
    return 0;
}

int TcpClient_Win32::setOption(int option, int *value)
{
    return 0;
}

int TcpClient_Win32::getOption(int option, int *value)
{
    return 0;
}

void TcpClient_Win32::setConnectionTimeout(uint32_t milliseconds)
{

}

int TcpClient_Win32::setNoDelay(bool nodelay)
{
    return 0;
}

bool TcpClient_Win32::getNoDelay()
{
    return false;
}

IPAddress TcpClient_Win32::remoteIP()
{
    return IPAddress(0);
}

uint16_t TcpClient_Win32::remotePort()
{
    return 0 ;
}

        