#include "set_sys_time.h"

void Set_Sys_Time(char * time ){
    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];
    char second[3];
    char format_time[21]={0};
    int ret;
    pid_t pid,pid1;

    sscanf(time,"%4s%2s%2s%2s%2s%2s",year,month,day,hour,minute,second);
    strcpy(format_time, year);
    strcat(format_time,"-");
    strcat(format_time,month);
    strcat(format_time,"-");
    strcat(format_time,day);
    strcat(format_time," ");
    strcat(format_time,hour);
    strcat(format_time,":");
    strcat(format_time,minute);
    strcat(format_time,":");
    strcat(format_time,second);

    pid = vfork();
    if(pid <0){
        perror("create faild\n");
    }
    else if(pid >0){
        
        //printf("this is grandfather fork\n");
        //ret = execl("/sbin/hwclock", "hwclock","--systohc",NULL);
        //ret = execl("/bin/date", "date", "-s", format_time, NULL);
    }
    else {
        pid1 = vfork();
            if(pid1>0)
            {   
                sleep(2);
                //printf("this is father fork\n");
                ret = execl("/sbin/hwclock", "hwclock","--systohc",NULL);
            }
        else {
            //printf("this is son fork\n");
            ret = execl("/bin/date", "date", "-s", format_time, NULL);
            
        }
    }
   


//printf("len = %d,%s\n",strlen(format_time),format_time);
/*ret = execl("/bin/date", "date", "-s", format_time, NULL);
printf("%d\n",ret);
//execl("/sbin/hwclock", "hwclock", NULL);
sleep(1);
ret = execl("/sbin/hwclock", "hwclock","--systohc",NULL);
printf("%d\n",4);
*/




//printf("year= %s\nmonth= %s\nday= %s\nhour= %s\nminute = %s\nsecond= %s\n",year,month,day,hour,minute,second);

}

/*void main(){
    Set_Sys_Time("19991019235820");
    printf("----------------------------\n");
    //char *s = "2021-12-03 16:00:00";
}
*/
