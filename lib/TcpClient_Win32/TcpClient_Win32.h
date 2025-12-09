
#ifndef __LIB__TCPCLIENT_WIN32__H
#define __LIB__TCPCLIENT_WIN32__H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "IPAddress.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define TCP_TIMEOUT_STEP_Milisec 50
#define TCP_TIMEOUT_MIN_CONFIG_Milliseconds 500

#define TCP_STATE_IDLE 0
#define TCP_STATE_CONNECTING 1
#define TCP_STATE_CONNECTED 2
#define TCP_STATE_DISCONNECTED 3
#define TCP_STATE_ERROR 4

#define TCP_MODE_BLOCKING 0
#define TCP_MODE_NONBLOCKING 1
#define TCP_READ_TIMEOUT 0
#define TCP_WRITE_TIMEOUT 1


class TcpClient_Win32 {
public:             
    TcpClient_Win32();
    ~TcpClient_Win32();

  int connect(IPAddress ip, uint16_t port);
  int connect(const char *host, uint16_t port);
  size_t write(const uint8_t *buf, size_t size);

  int32_t available();
  int32_t readBytes(char *buffer, size_t length);
  int32_t readBytes(uint8_t *buffer, size_t length);

  /*
    uint8_t connected();
    void flush();
    int peek();
    void clear();
    void stop();
    int setSocketOption(int option, char *value, size_t len);
    int setSocketOption(int level, int option, const void *value, size_t len);
    int getSocketOption(int level, int option, const void *value, size_t size);
    int setOption(int option, int *value);
    int getOption(int option, int *value);
  */

  void setConnectionTimeout(uint32_t milliseconds);
  IPAddress remoteIP();
  uint16_t remotePort();

private:

    SOCKET _socket = INVALID_SOCKET;
    sockaddr_in _addr = {0};
    uint32_t _socket_timeout_ms = TCP_TIMEOUT_MIN_CONFIG_Milliseconds ; /* default 1 Second */

    bool init();
    bool deinit();
    void set_socket_mode(int mode);
    int set_socket_timeout(uint8_t mode,int timeout_ms);


}; 
#endif // __LIB__TCPCLIENT_WIN32__H
            
/*

Future for FREERTOS Version adding Guard protect race condition 
by multi call difference function in class

class SemaphoreGuard {
public:
    SemaphoreGuard(SemaphoreHandle_t sem, TickType_t timeout = portMAX_DELAY)
        : _sem(sem), _taken(false)
    {
        if (_sem) {
            if (xSemaphoreTake(_sem, timeout) == pdTRUE) {
                _taken = true;
            }
        }
    }

    ~SemaphoreGuard() {
        if (_sem && _taken) {
            xSemaphoreGive(_sem);
        }
    }

    bool ok() const { return _taken; }

private:
    SemaphoreHandle_t _sem;
    bool _taken;
};

*/