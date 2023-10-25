#include "bottom.h"

void KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

}
void KEY_Scan(void)
{
	static uint32_t high_cnt[5]={0};
	if(PBin(11)==1){
		high_cnt[0]++;
		if(high_cnt[0]>25){
			air.ui_mode=(air.ui_mode+1)%MAX_UI_COUNT;
		}
	}else{
		if(high_cnt[0]>3&&high_cnt[0]<25){
			air.ui_mode=(air.ui_mode+1)%MAX_UI_COUNT;
		}
		high_cnt[0]=0;
	}
	if(PBin(12)==1){
		high_cnt[1]++;
		if(high_cnt[1]>25){
			
		}
	}else{
		if(high_cnt[1]>3&&high_cnt[1]<25){
				air.mode=(air.mode+1)%MAX_MODE_COUNT;
				if(air.mode==MANUAL_MODE){
					air.humi_sta=0;
					air.fan_sta=0;
					air.violed_sta=0;
				}
				air.need_update=1;
		}
		high_cnt[1]=0;
	}
	if(PBin(13)==1){
		high_cnt[2]++;
		if(high_cnt[2]>25){
			if(air.ui_mode==SET_UI){
				air.violed_offtime++;
			}
		}
	}else{
		if(high_cnt[2]>3&&high_cnt[2]<25){
			if(air.mode==MANUAL_MODE&&air.ui_mode!=SET_UI){
				air.fan_sta=!air.fan_sta;
				air.need_update=1;
			}
			if(air.ui_mode==SET_UI){
				air.violed_offtime++;
			}
		}
		high_cnt[2]=0;
	}
	if(PBin(14)==1){
		high_cnt[3]++;
		if(high_cnt[3]>25){
			if(air.ui_mode==SET_UI){
				if(air.violed_offtime>1){
					air.violed_offtime--;
				}
			}
		}
	}else{
		if(high_cnt[3]>3&&high_cnt[3]<25){
			if(air.mode==MANUAL_MODE&&air.ui_mode!=SET_UI){
				air.humi_sta=!air.humi_sta;
				air.need_update=1;
			}
			if(air.ui_mode==SET_UI){
				if(air.violed_offtime>1){
					air.violed_offtime--;
				}
			}
		}
		high_cnt[3]=0;
	}
	if(PBin(15)==1){
		high_cnt[4]++;
		if(high_cnt[4]>25){
			
		}
	}else{
		if(high_cnt[4]>3&&high_cnt[4]<25){
			air.violed_sta=!air.violed_sta;
			air.need_update=1;
		}
		high_cnt[4]=0;
	}
}

	

