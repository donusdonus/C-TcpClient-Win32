
#include "TcpClient_Win32.h"
#include "IPAddress.h"

TcpClient_Win32::TcpClient_Win32(){

}

TcpClient_Win32::~TcpClient_Win32(){
    
}

int TcpClient_Win32::connect(IPAddress ip, uint16_t port)
{
    return 0;
}

int TcpClient_Win32::connect(IPAddress ip, uint16_t port, int32_t timeout_ms)
{
    return 0;
}

int TcpClient_Win32::connect(const char *host, uint16_t port)
{
    return 0;
}

int TcpClient_Win32::connect(const char *host, uint16_t port, int32_t timeout_ms)
{
    return 0;
}

size_t TcpClient_Win32::write(uint8_t data)
{
    return 0;
}

size_t TcpClient_Win32::write(const uint8_t *buf, size_t size)
{
    return 0;
}


void TcpClient_Win32::flush()
{

}

int TcpClient_Win32::available()
{
    return 0;
}

int TcpClient_Win32::read(uint8_t *buf, size_t size)
{
    return 0;
}

size_t TcpClient_Win32::readBytes(char *buffer, size_t length)
{
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

        