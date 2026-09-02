#include"feeddog.h"

int feeddog(){
    int fd=-1;
    if((fd = open("wd",O_RDWR|O_NONBLOCK))<0){
        printf("feed dog failed\n");
        return 0;
    }
    else if(fd>0){
        write(fd,"wang",128);
        close(fd);
        return 1;
    }   
}

/*void main(){
    int r;
    //int fd= open("wd",O_RDWR|O_NONBLOCK);
    while(1){
        sleep(6);
        printf("feed dog--------");
        //write(fd,"wang",128);
        feeddog();
        printf("%d\n",r);
        
    }
    
}*/