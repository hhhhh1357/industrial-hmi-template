#include "relayer.h"

int OpenRelayer(){   //打开继电器，在界面加载时调用

    relayer_fd = open(RELAYER_DEVICENAME, O_RDWR);  
    if(relayer_fd<0)
        printf("open failed\n");
    else
        printf("open success\n");
    return relayer_fd;
}
void CloseRelayer(){//关闭继电器，在界面关闭时调用
    close(relayer_fd);
}
void Single_On(RELAYER_CHANNEL ch){//打开单个通道，单击打开通道按钮时调用
    int err  = ioctl(relayer_fd, _IO(RELAYER_IOC_MAGIC, ch));
    printf("Relayer Single_On, %d\n", err);
}
void Single_Off(RELAYER_CHANNEL ch){//关闭单个通道，单击关闭通道按钮时调用
    int err  = ioctl(relayer_fd, _IO(RELAYER_IOC_MAGIC, ch+4));
    printf("Relayer Single_Off, %d\n", err);
}


void All_On(){ //打开所有通道，单击打开所有通道按钮时调用
    RELAYER_CHANNEL ch;
    for(ch=CH1; ch<=CH4;ch++){
        Single_On(ch);
        printf("Channel %d Switch On Completed !\n",ch);
    }
    printf("All Channel Switch On Completed !");
}
void All_Off(){//关闭所有通道，单击关闭所有通道按钮时调用
    RELAYER_CHANNEL ch;
    for(ch=CH1; ch<=CH4;ch++){
        Single_Off(ch+4);
        printf("Channel %d Switch Off Completed !\n",ch);
    }
    printf("All Channel Switch Off Completed !");
}

//Example:
void main(){
    OpenRelayer();  //

    Single_On(CH1);   //

    CloseRelayer();
}