/**
 * Copyright (c) 2017 China Mobile IOT.
 * All rights reserved.
**/
#include <nbiot.h>
#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "platform.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "SHT20.h"
#include "adxl345.h"
#include "BH1750.h"
#include "BMP180.h"
#include "ad7124.h"

nbiot_value_t temp;   /* 温度 */
nbiot_value_t humi;   /* 湿度 */
nbiot_value_t illumi; //光照
nbiot_value_t press;  //压力
nbiot_value_t altitude; //海拔
nbiot_value_t acceleX;//三轴加速x
nbiot_value_t acceleY;//三轴加速y
nbiot_value_t acceleZ;//三轴加速z
nbiot_value_t LED;
nbiot_value_t voltage0;
nbiot_value_t voltage1;
nbiot_value_t voltage2;
nbiot_value_t voltage3;




extern float result_lx;//光照
extern _bmp180 bmp180;//压力/海拔
extern  ADXL345_INFO adxlInfo;//三轴加速
void AD7124_8_INIT(void);
void AD7124_8_Reset(void);
void Delay(vu32 nCount);
void LED_TEST(void);
void AD7124_8_INIT1(void);
//uint32_t AD7124_8_Read(void);//??????,??uint32_t ????
//uint8_t AD7124_8_Read_Byte(void);
//AD7124_INFO voltage_Getvalue(void);

void write_callback( uint16_t       objid,
                     uint16_t       instid,
                     uint16_t       resid,
                     nbiot_value_t *data )
{
    printf( "write /%d/%d/%d：%d\r\n",
                  objid,
                  instid,
                  resid,data->value.as_bool );
    if(objid==3311&&instid==0&&resid==5850)
			 Led1_Set(data->value.as_bool);
		/*
		else if(objid==3301&&instid==0&&resid==5700){		    
		      illumi.flag |= NBIOT_UPDATED;
          BH1750_test();				 
				  illumi.value.as_float=result_lx;
		}else if(objid==3303&&instid==0&&resid==5700){
			      SHT20_INFO sht20={0};
		        temp.flag |= NBIOT_UPDATED;						 
            sht20=SHT20_GetValue();
				    temp.value.as_float=(int64_t)sht20.tempreture;
		
		}else if(objid==3304&&instid==0&&resid==5700){
			      SHT20_INFO sht20={0};
			      humi.flag |= NBIOT_UPDATED;					 
            sht20=SHT20_GetValue();
				    humi.value.as_float=(int64_t)sht20.humidity;
			
    }
		*/
}

void read_callback( uint16_t       objid,
                     uint16_t       instid,
                     uint16_t       resid,
                     nbiot_value_t *data )
{
				printf( "read /%d/%d/%d\r\n",
                  objid,
                  instid,
                  resid);
	
        if(objid==3301&&instid==0&&resid==5700){		    
                  BH1750_test();				 
				  illumi.value.as_float=result_lx;
		}else if(objid==3303&&instid==0&&resid==5700){
			      SHT20_INFO sht20={0};					 
                  sht20=SHT20_GetValue();
				  temp.value.as_float=(double)sht20.tempreture;
		
		}else if(objid==3304&&instid==0&&resid==5700){
			      SHT20_INFO sht20={0};				 
                  sht20=SHT20_GetValue();
				  humi.value.as_float=(double)sht20.humidity;
					
    }else if(objid==3316&&instid==0&&resid==5700){
							AD7124_INFO ad7124;
			        ad7124=voltage_Getvalue();
			    voltage0.value.as_float=(double)ad7124.voltagevalue0;
		}else if(objid==3316&&instid==1&&resid==5700){
							AD7124_INFO ad7124;
			        ad7124=voltage_Getvalue();
			    voltage1.value.as_float=(double)ad7124.voltagevalue1;
		}else if(objid==3316&&instid==2&&resid==5700){
							AD7124_INFO ad7124;
			        ad7124=voltage_Getvalue();
			    voltage2.value.as_float=(double)ad7124.voltagevalue2;
		}else if(objid==3316&&instid==3&&resid==5700){
							AD7124_INFO ad7124;
			        ad7124=voltage_Getvalue();
			    voltage3.value.as_float=(double)ad7124.voltagevalue3;
		}
	
}

void execute_callback( uint16_t       objid,
                       uint16_t       instid,
                       uint16_t       resid,
                       nbiot_value_t *data,
                       const void    *buff,
                       size_t         size )
{
    printf( "execute /%d/%d/%d\r\n",
                  objid,
                  instid,
                  resid );
}

#ifdef NOTIFY_ACK
void notify_ack_callback( uint16_t       objid,
                          uint16_t       instid,
                          uint16_t       resid,
                          nbiot_value_t *data,
                          bool           ack )
{
    printf( "notify ack /%x/%x/%x(%s)\r\n",
                  objid,
                  instid,
                  resid,
                  ack ? "true":"false" );
}
#endif

char uri[] = "coap://183.230.40.40:5683";
nbiot_device_t *dev = NULL;


time_t last_time=0;
time_t cur_time=0;


void res_update(time_t interval)
{
	     SHT20_INFO sht20;
	     AD7124_INFO ad7124;
       if(cur_time>=last_time+interval){ 
            cur_time=0;
            last_time=0;				 
             
				    temp.flag |= NBIOT_UPDATED;		
            humi.flag |= NBIOT_UPDATED;					 
            sht20=SHT20_GetValue();
				    temp.value.as_float=sht20.tempreture;
				    humi.value.as_float=sht20.humidity;
       
				    illumi.flag |= NBIOT_UPDATED;
            BH1750_test();				 
				    illumi.value.as_float=result_lx;
				    
				    voltage0.flag|= NBIOT_UPDATED;
				    voltage1.flag|= NBIOT_UPDATED;
				    voltage2.flag|= NBIOT_UPDATED;
				    voltage3.flag|= NBIOT_UPDATED;
				    ad7124=voltage_Getvalue();
				    voltage0.value.as_float=ad7124.voltagevalue0;
				    voltage1.value.as_float=ad7124.voltagevalue1;
				 	  voltage2.value.as_float=ad7124.voltagevalue2;
				    voltage3.value.as_float=ad7124.voltagevalue3;
	 
				 /*
				    press.flag |= NBIOT_UPDATED;		
            altitude.flag |= NBIOT_UPDATED;	
				    BMP180_test();
				    press.value.as_float=bmp180.p;
				    altitude.value.as_float=bmp180.altitude;
				    
				    acceleX.flag |= NBIOT_UPDATED;		
            acceleY.flag |= NBIOT_UPDATED;
            acceleZ.flag |= NBIOT_UPDATED;							
            ADXL345_GetValue();
						acceleX.value.as_float=adxlInfo.incidence_Xf;
						acceleY.value.as_float=adxlInfo.incidence_Yf;
						acceleZ.value.as_float=adxlInfo.incidence_Zf;
         */
			}else if(cur_time==0&&last_time==0){
			
			    cur_time=nbiot_time();
			    last_time=cur_time;
			
			}else{
			
			   cur_time=nbiot_time();
			   
			} 	 

}	
int main( int argc, char *argv[] )
{
     int life_time = 1000;
	   int ret;
	   uint32_t i;
     nbiot_init_environment( argc, argv );
//	   uint32_t Data;
//     float data_temp,Vol;
//     float VolM[8];
//     uint8_t DATA_STATUS;
	
			AD7124_8_Reset();//??AD7190????
			Delay(100);
			AD7124_8_INIT1();
//	    while(1)
//				{ 
//          while(AD7124_8_DOUT);//????????
//          Data=AD7124_8_Read();//Data????
//          DATA_STATUS=AD7124_8_Read_Byte()&0x0F;//??????
//          data_temp=Data;
//          VolM[DATA_STATUS]=(data_temp-0x800000)/0x800000*2.5;//??8??????,AIN0-AIN1???1,AIN2-AIN3???2,????
//					printf("%x %d %f\n",Data,Data,VolM[DATA_STATUS]);
//				}   
 
	    
	

 #ifdef NOTIFY_ACK
        ret = nbiot_device_create( &dev,
                                   life_time,
                                   write_callback,
                                   read_callback,
                                   execute_callback,
                                   notify_ack_callback );
#else
        ret = nbiot_device_create( &dev,
                                   life_time,
                                   write_callback,
                                   read_callback,
                                   execute_callback );
#endif
       
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(/3200/0/5750) failed, code = %d.\r\n", ret );
        }
				LED.type = NBIOT_BOOLEAN;
        LED.flag = NBIOT_READABLE|NBIOT_WRITABLE;
        ret = nbiot_resource_add( dev,
                                  3311,
                                  0,
                                  5850,
                                  &LED );//灯控制
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(LED) failed, code = %d.\r\n", ret );
        }
				
        temp.type = NBIOT_FLOAT;
        temp.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3303,
                                  0,
                                  5700,
                                  &temp );//温度
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(temp) failed, code = %d.\r\n", ret );
        }
				
				
        humi.type = NBIOT_FLOAT;
        humi.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3304,
                                  0,
                                  5700,
                                  &humi );//湿度
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(humi) failed, code = %d.\r\n", ret );
        }

        illumi.type = NBIOT_FLOAT;
        illumi.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3301,
                                  0,
                                  5700,
                                  &illumi );//光照
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(illumi) failed, code = %d.\r\n", ret );
        }
				
        voltage0.type = NBIOT_FLOAT;
        voltage0.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3316,
                                  0,
                                  5700,
                                  &voltage0 );//??
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(press) failed, code = %d.\r\n", ret );
        }
				voltage1.type = NBIOT_FLOAT;
        voltage1.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3316,
                                  1,
                                  5700,
                                  &voltage1 );//??
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(press) failed, code = %d.\r\n", ret );
        }
				voltage2.type = NBIOT_FLOAT;
        voltage2.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3316,
                                  2,
                                  5700,
                                  &voltage2 );//??
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(press) failed, code = %d.\r\n", ret );
        }
				voltage3.type = NBIOT_FLOAT;
        voltage3.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3316,
                                  3,
                                  5700,
                                  &voltage3 );//??
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(press) failed, code = %d.\r\n", ret );
        }
				/*
        altitude.type = NBIOT_FLOAT;
        altitude.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3202,
                                  4,
                                  5600,
                                  &altitude );//海拔
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(temp) failed, code = %d.\r\n", ret );
        }
				
        acceleX.type = NBIOT_FLOAT;
        acceleX.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3202,
                                  5,
                                  5600,
                                  &acceleX );//x轴
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(acceleX) failed, code = %d.\r\n", ret );
        }
				acceleY.type = NBIOT_FLOAT;
        acceleY.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3202,
                                  6,
                                  5600,
                                  &acceleY );//y轴
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(acceleY) failed, code = %d.\r\n", ret );
        }
				acceleZ.type = NBIOT_FLOAT;
        acceleZ.flag = NBIOT_READABLE;
        ret = nbiot_resource_add( dev,
                                  3202,
                                  7,
                                  5600,
                                  &acceleZ );//z轴
        if ( ret )
        {
            nbiot_device_destroy( dev );
            printf( "device add resource(acceleZ) failed, code = %d.\r\n", ret );
        }
				*/
	      nbiot_object_add(dev);
        ret = nbiot_device_connect(dev,100);

        if ( ret )
        {
            printf( "connect OneNET failed.\r\n" );
			      nbiot_reset();
        }else{
					 // Led4_Set(LED_ON);
	         printf( "connect OneNET success.\r\n" );
				 
				}
    do
    {
             ret = nbiot_device_step( dev, 1);
             if ( ret )
             {
               printf( "device step error, code = %d.\r\n", ret );
				       Led4_Set(LED_OFF);
               printf( "connect server failed.\r\n" );
					     nbiot_reset();
             }else{ 
               res_update(60);	
             }					 
			      
    } while(1);
    nbiot_clear_environment();
  //  getchar();
    printf( "press enter key to exit..." );

    return 0;
}
