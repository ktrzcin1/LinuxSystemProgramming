#include <stdlib.h> //system()
#include <stdio.h>

int main(){

    char buff[80];
    long long int buff2;
    char *ret = NULL;
    while(1)
    {   
        printf("Enter command: ");
        fflush(stdout);
        ret = fgets(buff, sizeof(buff), stdin);
        if(ret == NULL)
            return 1;

        system(buff);    
    }
}