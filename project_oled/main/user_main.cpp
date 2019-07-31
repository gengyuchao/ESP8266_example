/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

#include "esp_log.h"

#include "SSD1306I2C.hpp"
 
//生成 OLED 的实例化对象
SSD1306I2C display(0x3c,GPIO_NUM_5,GPIO_NUM_4);// OLED_Address SCL SDA

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
extern "C" void app_main(void)
{
    char ShowTextBuff[50]={0};
    int run_time_count = 0;

    printf("SDK version:%s\n", esp_get_idf_version());
    
    display.init();//oled初始化

    display.clear();//清屏

    sprintf(ShowTextBuff,"Hello ESP8266\n");
    display.display();//开始显示

    while(1)
    {
        run_time_count++;
        display.clear();//清屏
        sprintf(ShowTextBuff,"Run time %d\n",run_time_count);

        display.drawString(0,10,ShowTextBuff);//向缓存中写文字
        display.display();//开始显示
        
        ESP_LOGI("OLED","%s",ShowTextBuff);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}



