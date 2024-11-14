#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main () {
    pid_t childPid;
    
    childPid = fork(); 
    if (childPid == 0){
        childPid = fork();
    }
    if (childPid == 0) {
        printf("Parent process die %d\n", childPid);
        exit(0);

    } 
    else if (childPid > 0) {
        printf("Created a child process %d\n", childPid);
        sleep(2);
        printf("Child continie live process %d\n", childPid);
        sleep(2);
        wait(NULL);
        printf("Child proccess %d killed\n", childPid);
        }
    
    
    return 0;
}
