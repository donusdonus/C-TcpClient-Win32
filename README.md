# C-TcpClient-Win32

A Windows-based TCP Client library written in C++ for establishing and managing TCP socket connections on Windows platforms. This library provides a high-level interface for socket communication, handling the complexity of low-level Winsock2 operations.

## Overview

`C-TcpClient-Win32` is a lightweight TCP client implementation designed for Windows applications that need reliable TCP/IP network communication. It encapsulates Windows Sockets 2 (Winsock2) functionality and provides an intuitive API similar to Arduino's networking libraries, making it easy to integrate network capabilities into C++ applications.

### Key Features

- **Simple Connection Management**: Easy-to-use API for establishing TCP connections
- **Non-Blocking Operations**: Support for non-blocking socket operations with configurable timeouts
- **Flexible Connection Methods**: Connect using either IP addresses or hostnames (with automatic DNS resolution)
- **Data Transfer**: Reliable send and receive operations with automatic timeout handling
- **Remote Connection Info**: Query remote peer's IP address and port number
- **Cross-Platform Ready**: Built with CMake for easy compilation and portability

## Project Structure

```
C-TcpClient-Win32/
├── lib/
│   ├── TcpClient_Win32/
│   │   ├── TcpClient_Win32.h         # Main TCP client class header
│   │   ├── TcpClient_Win32.cpp       # TCP client implementation
│   │   └── CMakeLists.txt
│   └── IPAddress/
│       ├── IPAddress.h               # IP address wrapper class
│       ├── IPAddress.cpp
│       └── CMakeLists.txt
├── src/
│   └── main.cpp                      # Main application example
├── examples/
│   └── SendRecived/
│       └── main.cpp                  # Example: Send/Receive demo
├── build/                            # CMake build directory
├── cmd/                              # Build scripts (batch files)
├── CMakeLists.txt                    # Main CMake configuration
└── README.md                         # This file
```

## Core Components

### TcpClient_Win32 Class

The main class providing TCP client functionality with the following public methods:

#### Connection Methods

- **`int connect(IPAddress ip, uint16_t port)`**
  - Connects to a remote server using an IP address object
  - Returns: 1 on success, 0 on failure

- **`int connect(const char *host, uint16_t port)`**
  - Connects to a remote server using hostname or IP string
  - Automatically handles DNS resolution for hostnames
  - Returns: 1 on success, 0 on failure

#### Data Transfer Methods

- **`size_t write(const uint8_t *buf, size_t size)`**
  - Sends data to the connected server
  - Parameters: buffer pointer and size in bytes
  - Returns: number of bytes sent, -1 on error

- **`int32_t available()`**
  - Checks how many bytes are available to read from the socket
  - Returns: number of available bytes, -1 on error

- **`int32_t readBytes(char *buffer, size_t length)`**
  - Reads data from the socket into a character buffer
  - Returns: number of bytes read, -1 on error

- **`int32_t readBytes(uint8_t *buffer, size_t length)`**
  - Reads data from the socket into an unsigned byte buffer
  - Returns: number of bytes read, -1 on error

#### Utility Methods

- **`void setConnectionTimeout(uint32_t milliseconds)`**
  - Sets the timeout duration for socket operations
  - Minimum value: 500 milliseconds

- **`IPAddress remoteIP()`**
  - Returns the IP address of the connected remote peer

- **`uint16_t remotePort()`**
  - Returns the port number of the connected remote peer

### IPAddress Class

A utility class for handling IP addresses:

- Supports initialization from a 32-bit integer or four octets
- Provides `toString()` method for converting to string representation
- Overloaded operators for comparison and assignment
- Indexed access to individual octets

## Building the Project

### Prerequisites

- **Compiler**: MinGW (g++) for Windows
- **Build System**: CMake 3.14 or higher
- **Libraries**: Windows Winsock2 (included with Windows SDK)

### Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .
```

### Using Build Scripts

The project includes batch files for quick building:

- **`cmd/init.bat`** - Initialize the build environment
- **`cmd/build.bat`** - Build the project
- **`cmd/build_debug.bat`** - Build with debug information
- **`cmd/clean.bat`** - Clean build artifacts
- **`cmd/run.bat`** - Run the built application

## Usage Example

### Basic Connection and Communication

```cpp
#include <stdio.h>
#include "TcpClient_Win32.h"
#include <winsock2.h>

WSADATA WSA = {0};
TcpClient_Win32 tcpclient;

int main()
{
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &WSA);

    // Connect to server at 127.0.0.1:19910
    if (tcpclient.connect("127.0.0.1", 19910)) {
        printf("Connected successfully\n");

        // Send data
        const char *message = "Hello Server";
        tcpclient.write((uint8_t*)message, strlen(message));

        // Receive data
        int available = tcpclient.available();
        if (available > 0) {
            uint8_t buffer[available];
            int received = tcpclient.readBytes(buffer, available);
            printf("Received %d bytes\n", received);
        }
    } else {
        printf("Connection failed\n");
    }

    // Cleanup
    WSACleanup();
    return 0;
}
```

## Implementation Details

### Socket Management

- **Initialization**: Sockets are created on demand during the first `connect()` call
- **Cleanup**: Automatic cleanup in the destructor, or manual via `deinit()`
- **Non-Blocking Mode**: Used internally for timeout-aware operations with automatic state management

### Timeout Handling

- **Read/Write Timeouts**: Configurable per-socket operation using `select()` function
- **Default Timeout**: 500 milliseconds (minimum configurable value)
- **Timeout Steps**: 50 millisecond intervals for granular control

### Error Handling

- **Connection Errors**: Returns 0 on failure, 1 on success
- **Socket Errors**: Returns -1 on socket errors during read/write operations
- **Availability**: Returns -1 on query errors, 0 or positive number for bytes available

## Configuration Constants

Defined in `TcpClient_Win32.h`:

- **`TCP_MODE_BLOCKING`** (0): Blocking socket mode
- **`TCP_MODE_NONBLOCKING`** (1): Non-blocking socket mode
- **`TCP_READ_TIMEOUT`** (0): Identifier for read timeout
- **`TCP_WRITE_TIMEOUT`** (1): Identifier for write timeout
- **`TCP_TIMEOUT_STEP_Milisec`** (50): Timeout increment in milliseconds
- **`TCP_TIMEOUT_MIN_CONFIG_Milliseconds`** (500): Minimum configurable timeout

## Thread Safety

**Note**: The current implementation is not thread-safe. If used in multi-threaded environments, external synchronization is required. The header file includes a commented-out `SemaphoreGuard` class for future FreeRTOS-based implementations that will add built-in thread safety.

## Limitations and Future Work

### Currently Not Implemented

- `connected()` - Check if socket is currently connected
- `stop()` / `flush()` / `clear()` - Connection management utilities
- `peek()` - Preview next byte without consuming
- Socket option getters/setters - Advanced socket configuration

### Future Enhancements

- FreeRTOS support with semaphore-based thread safety
- Additional socket options (SO_RCVBUF, SO_SNDBUF, etc.)
- Connection status querying
- Buffer flushing and peek operations

## Dependencies

- **System Libraries**:
  - `winsock2.h` - Windows Sockets 2 API
  - `ws2tcpip.h` - TCP/IP support for Winsock2
  - `windows.h` - Windows API

- **Project Libraries**:
  - `IPAddress` - Custom IP address handling class

## License

See the LICENSE file for licensing information.

## Contributing

Contributions are welcome. Please ensure code follows the existing style and conventions.

## Author

Created by: donusdonus
Repository: https://github.com/donusdonus/C-TcpClient-Win32