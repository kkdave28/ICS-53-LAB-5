#include <signal.h>
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
void sigint(int a)
{
    printf("^C caught\n");
    exit(EXIT_SUCCESS);
}
int main()
{
    signal(SIGINT, sigint);
    for (;;) {}
}