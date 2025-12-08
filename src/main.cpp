
#include <stdio.h>
#include "TcpClient_Win32.h"


int main()
{
    printf("Hello World");

    TcpClient_Win32 tcpclient;

    tcpclient.connect("127.0.0.1",1991);

    char text[50];
    int len = 0;

    len = sprintf(text,"HELLO WORLD!!!. I'm Client\n");

    tcpclient.write((uint8_t*)&text[0],len);
    Sleep(10000);


    tcpclient.available();

    return 0;
}

        