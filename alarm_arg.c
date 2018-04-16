#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void
help (void)
{
    printf("Help message.\n");
}

void 
alarm_handler(int num)
{
    printf("Handler of %d signal.\n", num);
    alarm(5);
}


int
main(int argc, char** argv){

    if(argc != 2){
        help();
        return 1;
    }

    int time = 0;
    if (sscanf(argv[1], "%i", &time) != 1)
    {
        printf("Error parsing arguments.\n");
        return 1;
    }
    printf("Argument: %d\n", time);

    //Handle alarm signal.
    signal(SIGALRM, alarm_handler);
    alarm(time);
    while(1){
        printf("Sleeping...10\n");
        sleep(10);
    }
}