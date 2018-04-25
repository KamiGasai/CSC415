//
//  main.c
//  ASCIICount
//
//  Created by PEITONG SHI on 01/10/2017.
//  Copyright © 2017 PEITONG SHI. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#define THREAD 8  //numbers of thread = 8


int r[127];

void *runner(void *param);
struct Section {
    int p_index;    //index of pthread
    int start;      //where to start
    int end;        //where to end
    int *arr;
    int **record;
    
};



int main(int argc, const char * argv[]) {
    char file_string[65536]; //define the max size of the file that can be read by program
    
    //terminate if argument is incorrect
    if (argc !=2 ) {
        printf("Sorry, please input the correct txt file name!\n");
        return -1;
    }
    
    
    char* buff;
    FILE* f;
    f = fopen(argv[1],"r"); //read the file only
    
    fread(file_string, 65536, 1, f); //memory block allocated = file_string, //each element size = 65536
                                          //element = 1, input file = f;
    int sep_point = 0;
    buff = strtok(file_string, "");   //seperate the string by "nextline"
    int bufferLength=(int)strlen(buff);
    printf("buffer length: %d \n", bufferLength);
    
    

    if (strlen(buff)%8 != 0){
         sep_point = (int)(strlen(buff) / 8)+1;  //if there is any reminder, in order to make sure every
    } else {                                        //character to be count, separate point position +1
         sep_point=(int) (strlen(buff)/8);    //if no reminder, seperate by 8
    }
    
    struct Section s [THREAD];
    
    
    
    for (int i = 0; i < THREAD; i++) {  //to define each THREAD's work
        s[i].p_index = i;
        s[i].start = i * (sep_point);
        s[i].end = ((i+1)*sep_point)-1;
        s[i].arr = malloc(sizeof(int)*sep_point*512);
        s[i].record = malloc(sizeof(int*) * 128);

        
        for (int j = s[i].start; j <= s[i].end; j++) {
            s[i].arr[j] = buff[j];
       //     printf("%d",s[i].arr[i]);
         //   printf("\n");
        }
        
    }
   

    pthread_t tid[THREAD];      //thread identifiers for child threads
    pthread_attr_t attr;        //thread attributes
    
    pthread_attr_init(&attr);
    
    for (int i = 0; i<THREAD; i++) {        //create child thread
        pthread_create(&tid[i], &attr, runner, &s[i]);
    }
    int result[128];
    
    for (int i = 0; i<THREAD;i++) {
        pthread_join(tid[i], NULL);
    }
    
    for (int i = 0; i<THREAD;i++) {
        for (int j = s[i].start; j <= s[i].end;j++){
            int tmp = s[i].record[j][s[i].arr[j]];
            if(tmp!=0) {
              //  printf("In THREAD %d, %d occurrence for %d\n", i,tmp, (int)buff[j]);
                for (int i = 0; i < tmp;i++) {
                   // printf("success\n");
                    result[buff[j]]++;
                    //printf("result[%d] now is %d\n", buff[j], result[buff[j]]);
                }
            }
        }
    }
    
    
    for (int i = 0; i < 128; i++) {      //Ouputting result
       printf("%d", result[i]);
	if (i<=32 || i>=127){
		printf(" occurences for 0x%02x", i);
	} else {
		printf(" occurences for %c", (char) i);
	}
        printf("\n");
    }
    
    
    for (int i = 0; i < THREAD; i++) {
        free(s[i].arr);
        free(s[i].record);
    }
    return 0;
    
    
}

void *runner(void *param) {
    
    int index, start, end;
    struct Section *s;
    s = param;
    int* arr;
    int **record;
    
    index = s->p_index;
    start = s->start;
    end = s->end;
    arr = s->arr;
    record = s->record;

    for (int i = 0; i<128 ; i++) {         //memory allocate
        record[i] = malloc(sizeof(int));
    }
    
    for (int i = start; i<=end; i++) {  //record[i][j] that in i thread, j from 0 - 127 represent 0*0 -- 0*7f
        if (arr[i] ==0) {                //if record[0][1] = 1, means there are 1 count of 0*01
        }else{                           //in the 0(first) thread.
        record[i][arr[i]]++;
        }
    }
    
    
    pthread_exit(0);
}
