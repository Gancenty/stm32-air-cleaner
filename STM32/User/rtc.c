#include "rtc.h"

void RTC_config(void){
		//让电源和后备寄存器使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
    //后备寄存器访问使能
    PWR_BackupAccessCmd(ENABLE);
    //初始化后备寄存器区域
    BKP_DeInit();
    //启用外部时钟
    RCC_LSEConfig(RCC_LSE_ON);
    //等待外部时钟启用成功
    while(!RCC_GetFlagStatus(RCC_FLAG_LSERDY));
    //配置RTC的时钟源伟外部时钟LSE
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    //启用RTC
    RCC_RTCCLKCmd(ENABLE);
    //等待时钟同步
    RTC_WaitForSynchro();
    //等待上一个命令完成
    RTC_WaitForLastTask();
    //时钟计算公式:fTR_CLK=fRTCLK/(PRL[19:0]+1)
    //PRL[19:0]=32767,即预设的分频系数
    //fRTCLK=1Hz,即1秒
    RTC_SetPrescaler(32767);
    //等待上一个命令完成
    RTC_WaitForLastTask();
    //启用秒中断
    RTC_ITConfig(RTC_IT_SEC,ENABLE);
    //等待上一个命令完成
    RTC_WaitForLastTask();

}
void RTC_set(u32 count)
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
	 PWR_BackupAccessCmd(ENABLE);
	 RTC_SetCounter(count);
	 RTC_WaitForLastTask();
}
void RTC_Config(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;//中断初始化结构体
	 NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;//中断请求名称
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//配置通道是否开启(ENABLE:1为开启)
	 NVIC_Init(&NVIC_InitStructure);//中断初始化
   if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        //重新配置RTC
        RTC_config();
        //配置完成后，向后备寄存器中写特殊字符0xA5A5
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        //若后备寄存器没有掉电，则无需重新配置RTC
        //这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            //这是上电复位
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            //这是外部RST管脚复位
        }
        //清除RCC中复位标志
        RCC_ClearFlag();

        //虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
        //但是每次上电后，还是要使能RTCCLK
        RCC_RTCCLKCmd(ENABLE);
        //等待RTC时钟与APB1时钟同步
        RTC_WaitForSynchro();

        //使能秒中断
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        //等待操作完成
        RTC_WaitForLastTask();
    }
	
}
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC)==SET)//判断执行的是否是秒中断
	{	
		air.time.timestamp=RTC_GetCounter();
		RTC_UpdateTime();	
		RTC_WaitForLastTask();
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
}
//                       1  2  3  4  5  6  7  8  9 10 11 12
const u8 month_table[]={31,28,31,30,31,30,31,31,30,31,30,31};
const u8 week_table[]= {0,  3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

u8 Is_Leap_Year(u16 _year)
{
    if(((_year%4==0)&&(_year%100!=0))||((_year%400==0)&&(_year%100==0))) return 1;
    else return 0;
}
void RTC_UpdateTime(void)
{
	RTC_GetCalendar(air.time.timestamp,&air.time);
}
void RTC_GetCalendar(u32 timecount,TIME *time)
{
    static u16 daycount=0;
    u32 daynum;  //day
    u32 temp=0;
		temp=3600*time->timezone;
		timecount=timecount+temp;
		daynum=timecount/86400;
    //Calendar year
    if(daycount!=daynum)  //>1
    {
        daycount=daynum;
        temp=1970;
        while(daynum>365)
        {
            if(Is_Leap_Year(temp))
            {
                if(daynum>=366)
                    daynum=daynum-366;
                else
                    break;
            }
            else
                daynum=daynum-365;
            temp++;
        }
        time->year=temp;  // set year
        temp=0;  //reset temp
        
        // Calendar month
        while(daynum>=28)
        {
            if(Is_Leap_Year(time->year) && temp==1)
            {
                if(daynum>=29) daynum=daynum-29;
                else break;
            }
            else
            {
                if(daynum>=month_table[temp])
                    daynum=daynum-month_table[temp];
                else
                    break;
            }
            temp++;
        }
        time->month=temp+1;  //set month
        time->day=daynum+1;  //set day
    }
		temp=timecount%86400; 
		time->hour=temp/3600;
    time->minute=(temp%3600)/60;  //set mintue
    time->second=(temp%3600)%60;  //set second
		time->weekday=RTC_GetWeek(time->year,time->month,time->day);
}

u8 RTC_SetCalendar(u16 _year,u8 _month,u8 _day,u8 _hour,u8 _min,u8 _second)
{
    u16 i;
    u32 seccount=0;  //second total
    if(_year<1970 || _year>2099) return 1;  //valid year
    
    //calculate year
    for(i=1970;i<_year;i++)
    {
        if(Is_Leap_Year(i))
            seccount=seccount+3600*24*366; //3600*24*366=31622400
        else
            seccount=seccount+3600*24*365; //3600*24*365=31536000
    }
    //calculate month
    _month=_month-1;
    for(i=0;i<_month;i++)
    {
        seccount=seccount+(u32)month_table[i]*3600*24;
        if(Is_Leap_Year(_year) && i==1)  //February,add 1 day
        {
            seccount=seccount+3600*24;
        }
    }
    
    //calculate day
    seccount=seccount+(u32)(_day-1)*3600*24;
    seccount+=(u32)_hour*3600;
    seccount+=(u32)_min*60;
    seccount+=_second;
    
    RTC_set(seccount);
    return 0;
}


u8 RTC_GetWeek(u16 _year,u8 _month,u8 _day)
{
    u16 temp;
    u8 yearH,yearL;
    yearH=_year/100;  //   2016/100=20
    yearL=_year%100;  //   2016%100=16
    
    if(yearH>19) yearL+=100;
    temp=yearL+yearL/4;
    temp=temp+_day+week_table[_month-1];
    if(yearL%4==0 && _month<3)temp=temp-1;
  
    return temp%7;
}

