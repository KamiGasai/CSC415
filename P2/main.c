//
//  main.c
//  Project2
//
//  Created by PEITONG SHI on 17/09/2017.
//  Copyright © 2017 PEITONG SHI. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>



int main(int argc, const char * argv[]) {
    pid_t pid;
    do {
        printf("1. Hello World\n2. List files\n3. Exit\nPlease select:\n"); //menu
        char a[100];
        scanf("%s", a);
        int selection = atoi(a);
        
        
        switch (selection) {
            case 1:
                pid = fork();
                if (pid < 0) { printf("Error occurs!");}
                else if (pid==0) {
                    printf("Hello World!\n");
                    exit(0);
                    
                }
                else { wait(NULL);} //parent
                break;
            case 2:
                pid = fork();
                if (pid < 0) { printf("Error occurs!");}
                else if (pid==0) {
                    execlp("/bin/ls", "ls", NULL);
                    exit(0);
                }
                else { wait(NULL);} //parent
                break;
                
            case 3:
                exit(0);
                break;
            default: //For error input
                printf("Error input, please re input the correct number\n");
                break;
        }
    } while (1); //loop forever;

    
    
    
       return 0;
}
