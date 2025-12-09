
#include <stdio.h>
#include "TcpClient_Win32.h"

WSADATA WSA = {0};

TcpClient_Win32 tcpclient;
char input[120];
int inputBuffer = 0;
int bufferRecived = 0;

int main()
{
    /* Important!!!! Enable Socket Application */
    WSAStartup(MAKEWORD(2,2),&WSA);

    printf("Software Started\r\n\r\n");
    
    tcpclient.connect("127.0.0.1",19910);

    while(1)
    {
       printf("Send Data : ");
       fgets(input,sizeof(input),stdin);
       inputBuffer = strlen(input);
       printf("User Input Buffer : %d Bytes\n",inputBuffer);
       tcpclient.write((uint8_t*)&input[0],inputBuffer);

       Sleep(5000);

        bufferRecived = tcpclient.available();
        if(bufferRecived > 0)
        {
            uint8_t data[bufferRecived];
            memset(&data[0],0x00,bufferRecived);
            tcpclient.readBytes(&data[0],bufferRecived);
            printf("Recived %d Bytes : ",bufferRecived);

            for(auto i = 0 ; i < bufferRecived ; i++)
                printf("%c",data[i]);

            printf("\r\n");
        }

    }

    /* Important!!!! Disable Socket Application */
    WSACleanup();

    return 0;
}

        