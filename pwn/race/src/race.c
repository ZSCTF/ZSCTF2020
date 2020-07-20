#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
static long *list;

void Init(){
    setbuf(stdout, 0);
    setbuf(stdin,0);
    setbuf(stderr,0);

    list = malloc(0xE0);
}

void Choice(){
    puts("----------------");
    puts("Dove's task list");
    puts("----------------");
    puts("  1.Add tasks   ");
    puts("----------------");
    puts(" 2.Delete tasks ");
    puts("----------------");
    puts("    3.gugugu    ");
    puts("----------------");
}

int Input(){
    char buf[0x10];
    read(0,buf,8);
    return atoi(buf);
}

void Add(){
    puts("index:");
    int index=Input();
    if(index>27||index<0)
        exit(0);
    list[index]=malloc(0xa0);
    puts("content:");
    read(0,list[index],0xa0);
}

void Delete(){
    puts("index:");
    int index=Input();
    if(index>27||index<0||!list[index])
        exit(0);
    free(list[index]);
    list[index]=0;
}

void *run(long *task){
    puts("Think about your reason");
    sleep(3);
    read(0,task,0xa0);
    printf("Because %s so you gugugu",task);
}


void Gu(){
    pthread_t tidp;
    puts("index:");
    int index=Input();
    if(index>27||index<0||list[index])
        exit(0);
    pthread_create(&tidp, NULL, run, list[index]);
    sleep(1);
}


int main() {
    int i;
    Init();

    printf("---------------------------------\n\n");
    printf("Welcome to zsctf2020. --zhakul\n\n");
    printf("---------------------------------\n\n");
    while(1){
        Choice();
        i=Input();
        switch(i){
            case 1:
                Add();
                break;
            case 2:
                Delete();
                break;
            case 3:
                Gu();
                break;
            default:
                puts("Input error");
        }
    }
    return 0;
}
