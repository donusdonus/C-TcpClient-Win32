
#ifndef __LIB__TCPCLIENT_WIN32__H
#define __LIB__TCPCLIENT_WIN32__H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "IPAddress.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define TCP_STATE_IDLE 0
#define TCP_STATE_CONNECTING 1
#define TCP_STATE_CONNECTED 2
#define TCP_STATE_DISCONNECTED 3
#define TCP_STATE_ERROR 4

#define TCP_MODE_BLOCKING 0
#define TCP_MODE_NONBLOCKING 1

#define TCPCLIENT_TIMEOUT_CONNECITON_MS 5000


class TcpClient_Win32 {
public:             
    TcpClient_Win32();
    ~TcpClient_Win32();

  int connect(IPAddress ip, uint16_t port);
  int connect(IPAddress ip, uint16_t port, int32_t timeout_ms);
  int connect(const char *host, uint16_t port, int32_t timeout_ms);
  int connect(const char *host, uint16_t port);
  size_t write(uint8_t data);
  size_t write(const uint8_t *buf, size_t size);

  void flush();
  int available();
  int read(uint8_t *buf, size_t size);
  size_t readBytes(char *buffer, size_t length);
  size_t readBytes(uint8_t *buffer, size_t length);
  int peek();
  void clear();
  void stop();
  uint8_t connected();

  int setSocketOption(int option, char *value, size_t len);
  int setSocketOption(int level, int option, const void *value, size_t len);
  int getSocketOption(int level, int option, const void *value, size_t size);
  int setOption(int option, int *value);
  int getOption(int option, int *value);
  void setConnectionTimeout(uint32_t milliseconds);
  int setNoDelay(bool nodelay);
  bool getNoDelay();

  IPAddress remoteIP();
  uint16_t remotePort();

private:

    u_long tcp_block_mode = TCP_MODE_BLOCKING;
    SOCKET _socket = INVALID_SOCKET;
    sockaddr_in _addr {};
    bool SocketReadytoUse = false;

    bool Init();
    bool Close();

}; 
#endif // __LIB__TCPCLIENT_WIN32__H
            