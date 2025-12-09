
#include <stdio.h>
#include "TcpClient_Win32.h"

WSADATA WSA = {0};

TcpClient_Win32 tcpclient;

int main()
{
    /* Enable Socket Application */
    WSAStartup(MAKEWORD(2,2),&WSA);

    tcpclient.connect("192.168.122.120",80);

    char text[50];
    int len = 0;

    len = sprintf(text,"HELLO WORLD!!!. I'm Client\n");

    tcpclient.write((uint8_t*)&text[0],len);
    Sleep(10000);


    tcpclient.available();

    /* Disable Socket Application */
    WSACleanup();

    return 0;
}

        