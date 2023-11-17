#include <sys/wait.h>  
#include <stdlib.h>  
#include <stdio.h>
#include <unistd.h>  
  
int main(void)  
{  
    pid_t pids[10];  
    int i;  
  
    for (i = 9; i >= 0; --i) {  
        pids[i] = fork();  
        if (pids[i] == 0) {  
            printf("CHILD %d\n", i);  
            sleep(i+1); 
            printf("CHILD %d DONE\n", i); 
            _exit(0);  
        }  
    }  
  
    for (i = 9; i >= 0; --i) {  
        printf("PARENT %d\n", i);  
        waitpid(pids[i], NULL, 0);  
    }  
  
    return 0;  
}  