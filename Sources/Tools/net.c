#include "net.h"

//设置网口
int Set_Ethernet(Interface ch , char *ip, char *mask, char *gateway){
	//ip=192.168.1.1
	//MAC=ba:f2:42:cc:5a:69
	//gateway= 192.167.1.1
	//mask=255.255.255.0
	//int (*fun)(int,char*, char*,char*,char*);
	

	if(ch==ETH0){
		char Address[100] = "ifconfig eth0 ";
		char Gateway[100] = "route add default gw ";
		strcat(Address, ip );
		strcat(Address, " netmask ");
		strcat(Address, mask);
		strcat(Gateway, gateway);
		printf("%s\n,%s\n",Address,Gateway);
		system("ifconfig eth0 up ");
		
		system(Address);
		//system("ifconfig eth0 add &mac");
		//system(MaskAddress);
		system(Gateway);
		//system("ifconfig eth1 down ");
		system("ifconfig pppd down ");
		return 1;
		
	}
	else if (ch==ETH1){
		char Address[100] = "ifconfig eth1 ";
		char Gateway[100] = "route add default gw ";
		strcat(Address, ip );
		strcat(Address, " netmask ");
		strcat(Address, mask);
		strcat(Gateway, gateway);
		printf("%s\n,%s\n",Address,Gateway);
		system("ifconfig eth1 up ");
		
		system(Address);
		//system("ifconfig eth0 add &mac");
		//system(MaskAddress);
		system(Gateway);
		//system("ifconfig eth0 down ");
		system("ifconfig pppd down ");
		return 1;
	}
	else{
		return 0;
	}
}
//设置4G
int Set_Wlan_Net(APN ch){
    int i=6,err=0;
        pid_t pid;
        FILE *fp;
        char buf[50];
        if(Dtu4GCheck()==1){
            return 1;
        }
        else{
        pid = fork();
            if(pid<0){
                printf("create fork fail\n");
                return 0;
            }
            if(pid==0){
                //childpid = getpid();
                printf("this is son fork\n");
                if(ch == China_Mobile)
                    system("cd /etc/gosuncn/&&./ppp-on-cmnet.sh &");
                else if(ch == China_Unicom)
                    system("cd /etc/gosuncn/&&./ppp-on-3gnet.sh &");
                else
                    system("cd /etc/gosuncn/&&./ppp-on-ctnet.sh &");
                printf("son fork exit\n");
                while(1);
            }
            else{
                printf("this is father fork--------------->\n");
                /*sleep(20);
                fp = popen("ps -o pid,comm|grep pppd","r");
                err = fscanf(fp,"%s",buf);
                pclose(fp);
                if(err==-1){
                    printf("this is buf %s\n",buf);
                    printf("conncet succeed!\n");
                    return 1;
                }*/
                while(i--){
                    if(Dtu4GCheck()==1) {
                    printf("conncet succeed!\n");
                    err = 1;
                    break;
                    }
                    sleep(1);
                }

                printf("-------------------relief child ");

                    if(err==0){
                        system("cd /etc/gosuncn/&&./disconnect");
                        printf("cd /etc/gosuncn/&&./disconnect\n");
                    }

                    kill(pid, SIGKILL);
                    wait(NULL);


                    return err;
            }
                /*else{
                    int status;
                    printf("this is buf %s\n",buf);
                    system("cd /etc/gosuncn/&&./disconnect");
                    printf("---------------------\n");
                    wait(&status);
                    return 0;
                }*/
            }


}
//关闭所有网口
void Close_Network(Interface ch){
	
    switch (ch)
        {
        case ETH0:
            system("ifconfig eth0 down ");
            break;
        case ETH1:
            system("ifconfig eth1 down ");
            break;
        case PPPD:
            system("/etc/gosuncn/disconnect");
            break;
        }

}

//检查usb是否挂载成功
int USBCheck(){
	int err;
	FILE *fp;
	char buf[50];
	fp = popen("ls /sys/bus/usb/drivers/usb-storage/ |grep '[0-9]-[0-9]*'","r");
	err = fscanf(fp,"%s",buf);
	if (err == -1){
        printf("----------------------------------------->mount usb fail\n");
		return 0;
	}
	else{
        printf("###################################------->buf=%s\n",buf);
		return 1;
	}
}

//检查SDcard是否挂载成功
int SDcardCheck(){
	int err;
	FILE *fp;
	char buf[50];
	fp = popen("ls /sys/block/ |grep mmcblk0","r");
	err = fscanf(fp,"%s",buf);
	if (err == -1){
		return 0;
	}
	else{
		printf("buf=%s",buf);
		return 1;
	}
}

//背光
int BackLight_Dark(){
	//将亮度调到最低，即熄屏效果
	system("cd /sys/class/backlight/backlight&&echo 0 > /sys/class/backlight/backlight/brightness");
	return 0;
}

int BackLight_Light(){
	//点亮屏幕
	system("cd /sys/class/backlight/backlight&&echo 7 > /sys/class/backlight/backlight/brightness");
	return 0;
}


//检测网口状态
int EthernetCheck(Interface ch){
    int err;
    FILE *fp;
    FILE *fp1;
    char buf[100];
    if(ch == ETH0){
        fp = popen("ifconfig |grep eth0","r");
        err = fscanf(fp,"%s",buf);
        printf("%s\n",buf);
        if(err == -1){
            printf("failed \n");
            pclose(fp);
            return 0;
        }
        else {
            fp1 = popen("cat /sys/class/net/eth0/carrier","r");
            err = fscanf(fp1,"%s",buf);
            if(strncmp(buf,"1",1)==0){
                printf("eth0 linking\n");
                pclose(fp);
                pclose(fp1);
                return 1;
            }
            else{
                printf("eth0 failed\n");
                pclose(fp);
                pclose(fp1);
                return 0;
            }
        }


    }
    else {
        fp = popen("ifconfig |grep eth1","r");
        err = fscanf(fp,"%s",buf);
        printf("%s\n",buf);
        if(err == -1){
            printf("failed \n");
            pclose(fp);
            return 0;
        }
        else {
            fp1 = popen("cat /sys/class/net/eth1/carrier","r");
            err = fscanf(fp1,"%s",buf);
            if(strncmp(buf,"1",1)==0){
                printf("eth1 linking\n");
                pclose(fp);
                pclose(fp1);
                return 1;
            }
            else{
                printf("eth1 failed\n");
                pclose(fp);
                pclose(fp1);
                return 0;
            }
        }


    }

}

int Dtu4GCheck()
{

    //FILE *fp;
        FILE *fp1;
        int err;
        char buf[100];
        /*fp = popen("ifconfig |grep ppp0","r");
            err = fscanf(fp,"%s",buf);
            printf("%s\n",buf);
            if(err == -1){
                printf("failed \n");
                pclose(fp);
                return 0;
            }*/
                fp1 = popen("cat /sys/class/net/ppp0/carrier","r");
                err = fscanf(fp1,"%s",buf);
                if(strncmp(buf,"1",1)==0){
                    printf("ppp0 linking\n");
                    //pclose(fp);
                    pclose(fp1);
                    return 1;
                }
                else{
                    printf("ppp0 failed\n");
                    //pclose(fp);
                    pclose(fp1);
                    return 0;
                }

}

//void main(){
//	/*FILE *fp;
//	int err;
//	char buf[20];
//	err = Set_Wlan_Net();
//	printf("###################");*/
//	/*int err;
//	Interface ch = ETH0;
//	char buf[100]="ifconfig eth0 ";
//	char *ip ="192.168.1.150";
//	char *mask = "255.255.255.0";
//	char *gateway = "192.168.1.1";
//	err = Set_Ethernet(ch , ip, NULL,  mask,  gateway);*/
//	//strcat(buf, ip);
//	//strcat(buf, " mask ");
//	//strcat(buf, mask);
//	//strcat(buf, " hello");
//	//printf("%s\n",buf);

//	/*int err;
//	int i;
//	char getbuff[100];
//	pid_t pid ;
//	int buflen;
//	memset(getbuff, 0, 100);
	
//	//printf("%d\n",err);
//	FILE *fp;
//	pid = fork();
//	if(pid ==0){
//		err = system("./t.sh");
//	}
//	else{
//		sleep(20);
//		fp = popen("ps -C t.sh -o pid=","r");
//		if(fp==NULL)
//			printf("popen fail");
//		fscanf(fp,"%s",getbuff);
//		pclose(fp);
//		}*/
//		/*int i;
//		char getbuff[50]={0};
//		char killbuff[50]="kill ";
//		FILE *fp;
//		fp = popen("ps -C t.sh -o pid=","r");
//		if(fp==NULL)
//			printf("popen fail");
//		i = fscanf(fp,"%s",getbuff);
//		printf("%s\n,i=%d\n",getbuff,i);
//		if(i == -1){
//			printf("conncet succeed");
//		}
//		else{
			
//			strcat(killbuff,getbuff);
//			printf("%s\n",killbuff);
//			system(killbuff);
//		}*/
	
//	//sleep(3);
//	//system("etc/gosuncn/disconnect");
//	system("cd /sys/class/backlight/backlight&&echo 0 > /sys/class/backlight/backlight/brightness");

//}


