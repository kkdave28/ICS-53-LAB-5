#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define LISTENQ 1024
struct 
{
    char name [128];
    char email[128];
} nmep[3];

static void fill_table()
{
    strcpy(nmep[0].name, "Ian G Harris");
    strcpy(nmep[0].email, "harris@ics.uci.edu");
    strcpy(nmep[1].name, "Joe Smith");
    strcpy(nmep[1].email, "joe@cnn.com");
    strcpy(nmep[2].name, "Jane Smith");
    strcpy(nmep[2].email, "jane@slashdot.org");
    
}
static char * lookup(char * email)
{
    int i;
    for(i=0; i< 3; i++)
    {
        if(strcmp(email, nmep[i].email) == 0)
        {
            return nmep[i].name;
        }
    }
    return "unknown";
}
static void create_socket(int * clientfd)
{
    *clientfd = socket(AF_INET, SOCK_STREAM,0);
}
static void print_error(const char * s)
{
    printf("SERVER %s\n",s);
}
int main(int argc, char *argv[])
{
    struct sockaddr_in server_socket_address;
    struct hostent * hp;
    char client_msg[512];   
    int serverfd = 0;
    int connection_fd, clen,bytes_read;
    int opt_val=1;
    create_socket(&serverfd);
    if(serverfd <0)
    {
        print_error("ERROR: SOCKET CREATION FAILED");
        return -1;
    }
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (const void *)&opt_val, sizeof(int))<0)
    {
        print_error("ERROR: PRE-BINDING FAILED");
        return -1;
    }
    memset(&server_socket_address, '0',sizeof(server_socket_address));
    server_socket_address.sin_family = AF_INET;
    server_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_socket_address.sin_port = htons(atoi(argv[1]));
    if(bind(serverfd, (struct sockaddr*)&server_socket_address, sizeof(server_socket_address))<0)
    {
        print_error("ERROR: BINDING FAILED");
        return -1;
    }
    if(listen(serverfd, LISTENQ)<0)
    {
        print_error("ERROR: LISTEN BINDING FAILED");
        return -1;
    }
    fill_table();
    clen = sizeof(server_socket_address);
    if((connection_fd = accept(serverfd, (struct sockaddr*)&server_socket_address, &clen)) <0)
    {
        print_error("ERROR: ACCEPT");
        return -1;
    }
    printf("Address Server Started:\n");
    while(1)
    {
    
    bzero(client_msg, sizeof(client_msg));
    bytes_read = read(connection_fd, client_msg, sizeof(client_msg));
    if(strcmp(client_msg, "+++") == 0)
    {
        close(connection_fd);
        clen = sizeof(server_socket_address);
        if((connection_fd = accept(serverfd, (struct sockaddr*)&server_socket_address, (socklen_t*)&clen)) <0)
        {
            print_error("ERROR: ACCEPT");
            return -1;
        }

     }  
    else
    {
        if(bytes_read<0)
        {
            print_error("ERROR: READ FAILED");
            return -1;
        }

        if(write(connection_fd, lookup((client_msg+1)), sizeof(client_msg)) < 0)
        {
            print_error("ERROR: SEND FAILED");
            return -1;
        }
        //close(connection_fd);
    }
    }
    return 0;
}