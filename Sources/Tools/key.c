#include "key.h"

int key_fd=-1;

int OpenKey(){
    key_fd = open(KEY_DEVICENAME, O_RDWR);
    if(key_fd<0)
        printf("open failed\n");		
    else       
        printf("open success\n"); 
    return key_fd;
}
int Single_Press(KEY key){
    int data, err;
    err = ioctl(key_fd,_IOR(KEY_IOC_MAGIC, key, int),&data);
    printf("%d\n",data);
    return data;
}
void All_Press(){
    for(KEY key=KEY1;key<=KEY4; key++){
        Single_Press(key);
        printf("Key %d Pressed !\n", key);
    }
    printf("All Keys Pressed !\n");
}
void CloseKey(){
    close(key_fd);
}
/*
//Example
void main(){
    OpenKey();

    Single_Press(KEY1);

    CloseKey();
}
*/
