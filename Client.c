#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>


void* pth(void* arg);

typedef struct
{
    int len;
    int id;
    short mode;
    time_t time;
}data;

typedef struct
{
    int pth;
    int sock;
} pth_sock;



int main(int argc, char** argv)
{
    int sock_conn = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_conn == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6657);

    if(bind(sock_conn, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_addr.sin_port = htons(argv[2]);

    if(connect(sock_conn, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in)))
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }


    pthread_t p1, p2;
    pth_sock s1, s2;

    s1.pth = 1;
    s1.sock = sock_conn;
    s2.pth = 2;
    s2.sock = sock_conn;

    pthread_create(p1, NULL, pth, (void*)&s1);
    pthread_create(p2, NULL, pth, (void*)&s2);




    close(sock_conn);

    return 0;
}


void* pth(void* arg)
{
    arg = (pth_sock*)arg;


    pthread_exit(NULL);
}
