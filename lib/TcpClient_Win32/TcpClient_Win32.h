
#ifndef __LIB__TCPCLIENT_WIN32__H
#define __LIB__TCPCLIENT_WIN32__H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "IPAddress.h"

class TcpClient_Win32 {
public:             
    TcpClient_Win32();
    ~TcpClient_Win32();

  int connect(IPAddress ip, uint16_t port);
  int connect(IPAddress ip, uint16_t port, int32_t timeout_ms);
  int connect(const char *host, uint16_t port);
  int connect(const char *host, uint16_t port, int32_t timeout_ms);
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
}; 
#endif // __LIB__TCPCLIENT_WIN32__H
            