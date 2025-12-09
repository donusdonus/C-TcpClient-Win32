
#include "TcpClient_Win32.h"
#include "IPAddress.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

TcpClient_Win32::TcpClient_Win32(){

}

TcpClient_Win32::~TcpClient_Win32(){
    deinit();
}

void TcpClient_Win32::set_socket_mode(int mode)
{
    u_long tcp_block_mode = mode;
    ioctlsocket(_socket,FIONBIO,&tcp_block_mode);
}

int TcpClient_Win32::set_socket_timeout(uint8_t mode, int timeout_ms)
{
    if(_socket == INVALID_SOCKET)
        return SOCKET_ERROR;

    fd_set fds;
    timeval tv;
    int res = 0;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    FD_ZERO(&fds);
    FD_SET(_socket,&fds);

    if(mode == TCP_WRITE_TIMEOUT)
        res = select(0,NULL,&fds,NULL,&tv);
    else if(mode == TCP_READ_TIMEOUT)
        res = select(0,&fds,NULL,NULL,&tv);

    return res; 
}

bool TcpClient_Win32::init()
{
    int res = 0 ;

    /* Create TCP Socket */
    if(_socket == INVALID_SOCKET)
        _socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    res = (_socket == INVALID_SOCKET)? 0 : 1 ;
    
    return res;
}

bool TcpClient_Win32::deinit()
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
    return connect(ip.toString(),port);
}


int TcpClient_Win32::connect(const char *host, uint16_t port)
{
    int tmp = 0;
    char port_str[7];

    /* Connecting Host */
    addrinfo details = {};
    addrinfo* result = nullptr;

    /* init Socket */
    tmp = init();
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
            deinit();
            return 0;   
        }

        /* Copy host address */
        memcpy(&_addr,result->ai_addr,sizeof(_addr));

        /* Clear result */
        freeaddrinfo(result);
    }

    /* Enable NonBlocking */
    set_socket_mode(TCP_MODE_NONBLOCKING);

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
            set_socket_mode(TCP_MODE_NONBLOCKING);
            deinit();
            return 0;   
        }

        /********* Process Wait Timeout ***************/
        
        int tmr_res = set_socket_timeout(TCP_WRITE_TIMEOUT,_socket_timeout_ms);
        
        if(tmr_res <= 0)
        {
            set_socket_mode(TCP_MODE_NONBLOCKING);
            deinit();
            return 0;   
        }

        /********* Check Connection ***************/
 
        int so_error = 0;
        int optlen   = sizeof(so_error);
        getsockopt(_socket, SOL_SOCKET, SO_ERROR, (char*)&so_error, &optlen);
        if (so_error != 0)
        {
            set_socket_mode(TCP_MODE_NONBLOCKING);
            deinit();
            return 0;      
        }
    }

    /* Disable NonBlocking */
    set_socket_mode(TCP_MODE_BLOCKING);

    return 1;
}

size_t TcpClient_Win32::write(const uint8_t *buf, size_t size)
{
     if (_socket == INVALID_SOCKET || buf == nullptr || size == 0)
        return 0;

    int bufferWrite = 0 ;
    int tcnt = 0; 
    bool flag_cancel = false;
    bool flag_tout = false;

    set_socket_mode(TCP_MODE_NONBLOCKING);

    while((bufferWrite < size) && (flag_tout == false) && (flag_cancel == false))
    {
        int buffersend = send(_socket,(char*)buf+bufferWrite,size-bufferWrite,0);

        /* Reference Return send(...) Keyword "WSABASEERR" */

        if(buffersend == 0)
        {
            flag_cancel = true ;
            /* Connection deinit */
            //return 0 ;
        }
        else if(buffersend > 0)
        {
            /* Buffer Update */
            bufferWrite+=buffersend;
        }
        else
        {
            /* Check Socket Ready */
            int _socket_err = WSAGetLastError();

            /* Wait Socket Ready */
            if(_socket_err == WSAEWOULDBLOCK)
            {
                if(tcnt <= _socket_timeout_ms)
                {
                    int tmr_res =set_socket_timeout(TCP_WRITE_TIMEOUT,_socket_timeout_ms/TCP_TIMEOUT_STEP_Milisec);

                    if(tmr_res == 0)
                    {   
                        tcnt+=(_socket_timeout_ms/TCP_TIMEOUT_STEP_Milisec);  
                    }
                    else if (tmr_res < 0)
                    {   
                        flag_cancel = true;
                    }
                }
                else 
                {
                    flag_tout = true;
                }
            }
            else 
            {
                flag_cancel =true;
            }

        }

    }

    /* Disable NonBlocking */
    set_socket_mode(TCP_MODE_BLOCKING);

    if(flag_cancel | flag_tout )
        return -1; 

    return bufferWrite;
}

int32_t TcpClient_Win32::available()
{
    u_long byteIncome = 0;

    if(_socket == INVALID_SOCKET)
        return 0;

    if(ioctlsocket(_socket,FIONREAD,&byteIncome) == SOCKET_ERROR)
        return -1;


    return (int32_t)byteIncome;

}


int32_t TcpClient_Win32::readBytes(char *buffer, size_t length)
{
    if(_socket == INVALID_SOCKET)
        return -1;

    int bufferRead = 0;
    uint32_t tcnt = 0;
    bool flag_cancel = false;
    bool flag_tout = false;

    set_socket_mode(TCP_MODE_NONBLOCKING);

    while( (bufferRead < length) && (flag_cancel == false) && (flag_tout == false))    
    {
        int res = recv(_socket,buffer+bufferRead,length-bufferRead,0);

        if(res > 0)
        {
            bufferRead += res;
        }
        else if(res == 0)
        {
            /* lost connection */
            flag_cancel = true;
        }
        else
        {
            /* Error Case  */
            int _socket_err = WSAGetLastError();

            /* socket have blocking process */
            if(_socket_err == WSAEWOULDBLOCK)
            {
                if(tcnt <= _socket_timeout_ms)
                {
                    int tmr_res = set_socket_timeout(TCP_READ_TIMEOUT,_socket_timeout_ms/TCP_TIMEOUT_STEP_Milisec);

                    if(tmr_res==0)
                    {
                        tcnt+=(_socket_timeout_ms/TCP_TIMEOUT_MIN_CONFIG_Milliseconds);
                    }
                    else if(tmr_res < 0)
                    {
                        flag_cancel = true;
                    }
                }
                else 
                {
                    flag_tout = true;
                }
            }   
            /* any error */
            else 
            {
                flag_cancel = true;
            }         

        }
    }

    set_socket_mode(TCP_MODE_BLOCKING);

    if(flag_cancel | flag_tout)
        return -1;
    
    return bufferRead;
}

int32_t TcpClient_Win32::readBytes(uint8_t *buffer, size_t length)
{ 
    return readBytes((char *)buffer, length); 
}

/*

void TcpClient_Win32::flush()
{

}


void TcpClient_Win32::clear()
{
    deinit();
}

void TcpClient_Win32::stop()
{

}

uint8_t TcpClient_Win32::connected()
{
    return 0;
}


int TcpClient_Win32::peek()
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
*/

void TcpClient_Win32::setConnectionTimeout(uint32_t milliseconds)
{
     _socket_timeout_ms = (milliseconds > TCP_TIMEOUT_MIN_CONFIG_Milliseconds)? milliseconds : TCP_TIMEOUT_MIN_CONFIG_Milliseconds;
}

IPAddress TcpClient_Win32::remoteIP()
{
    if (_socket == INVALID_SOCKET)
        return IPAddress(0);

    sockaddr_in addr;
    int addr_len = sizeof(addr);

    if (getpeername(_socket, (sockaddr*)&addr, &addr_len) == SOCKET_ERROR)
        return IPAddress(0);

    uint32_t ip_raw = ntohl(addr.sin_addr.s_addr);

    return IPAddress(ip_raw);

}

uint16_t TcpClient_Win32::remotePort()
{
    if (_socket == INVALID_SOCKET)
        return 0;

    sockaddr_in addr;
    int addr_len = sizeof(addr);

    if (getpeername(_socket, (sockaddr*)&addr, &addr_len) == SOCKET_ERROR)
        return 0;

    return ntohs(addr.sin_port);
    
}

        