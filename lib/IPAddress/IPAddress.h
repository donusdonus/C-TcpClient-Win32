
#ifndef __LIB__IPADDRESS__H
#define __LIB__IPADDRESS__H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

class IPAddress
{
public:
    IPAddress();
    ~IPAddress();

    IPAddress(uint32_t address)
    {
        _addr = address;
    }

    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
    {
        _addr = (first_octet << 24) | (second_octet << 16) | (third_octet << 8) | fourth_octet ;
    }

    const char *toString()
    {
        sprintf(IPString, "%d.%d.%d.%d", this[3], this[2], this[1], this[0]);
        return &IPString[0];
    }

    bool operator==(IPAddress addr)
    {
        return (addr == _addr);
    }

    void operator=(uint32_t addr)
    {
        _addr = addr ;
    }

    bool operator!=(IPAddress addr)
    {
        return (addr != _addr);
    };

    uint8_t operator[](int index)
    {
        return *((uint8_t *)(&_addr) + index % 4);
    }

private:
    uint32_t _addr;
    char IPString[16];
};
#endif // __LIB__IPADDRESS__H
