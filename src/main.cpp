
#include <stdio.h>
#include "TcpClient_Win32.h"


int main()
{
    printf("Hello World");

    TcpClient_Win32 tcpclient;

    tcpclient.connect("www.google.com",80);

    return 0;
}

        