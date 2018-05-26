#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
// Maybe a well defined port?
//#define PORT [30001-60000]
static int customgetLine (char *buff, size_t sz) // custom getline function for reading commands from the terminal.
{
    int ch, extra;

    printf("> ");
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    buff[strlen(buff)-1] = '\0';
    return OK;
}
static void create_socket(int * clientfd)
{
    *clientfd = socket(AF_INET, SOCK_STREAM,0);
}
static void print_error(const char * s)
{
    printf("CLIENT %s\n",s);
}
int main(int argc, char * argv[])
{
    char cont[256];
    while(1)
    {
    int cpid = fork();

    if(cpid == 0)
    {
    struct sockaddr_in server_socket_address;
    struct hostent * hp;
    int clientfd = 0;
    char server_return [512];
    int bytes_read;
    create_socket(&clientfd);
    if(clientfd <0)
    {
        print_error("ERROR: SOCKET CREATION FAILED");
        return -1;
    }
    if((hp = gethostbyname(argv[1])) == NULL)
    {
        print_error("ERROR: CANNOT RESOLVE HOST");
        return -1;
    }
    memset(&server_socket_address, '0',sizeof(server_socket_address));
    server_socket_address.sin_family = AF_INET;
    bcopy((char *) hp->h_addr_list[0], (char *)&server_socket_address.sin_addr.s_addr, hp->h_length);
    server_socket_address.sin_port = htons(atoi(argv[2]));

    char query[256];
    char temp[256];
    if(connect(clientfd, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address)) <0)
    {
        print_error("ERROR: CONNECTION TO THE SERVER FAILED");
        return -1;
    }
    customgetLine(query, sizeof(query));
    
        
        
        if(strcmp("+++", query) == 0)
        {
           write(clientfd, query, sizeof(query));
           printf("EXITING CLIENT\n");
           close(clientfd);
           return 0;
        }
    
        bzero(temp, sizeof(temp));
        temp[0] = strlen(query);
        int i;
        for(i=0; i< strlen(query); i++)
        {
            temp[i+1] = query[i];
        }
        if(write(clientfd, temp, sizeof(temp)) < 0)
        {
            print_error("ERROR: SEND FAILED");
            return -1;
        }
    

        //bzero(server_return, sizeof(server_return));
        if((bytes_read = read(clientfd, server_return, sizeof(server_return))) > 0)
        {
            printf("%s\n", server_return);
        }
        else if(bytes_read <0)
        {
            print_error("ERROR: READ FAILED");
            return -1;
        }

    
    }
    else{
    wait(NULL);
    return 0;}
    }
    
}