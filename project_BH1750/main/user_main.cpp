/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

#include "esp_log.h"

#include "BH1750_driver.hpp"

#define DEFAULT_BH1750_SDA_GPIO     GPIO_NUM_4
#define DEFAULT_BH1750_SCL_GPIO     GPIO_NUM_5

BH1750_Device my_BH1750(DEFAULT_BH1750_SDA_GPIO,DEFAULT_BH1750_SCL_GPIO);

extern "C" int ets_sprintf(char * buf,const char *fmt, ...);

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

    my_BH1750.init();
    //设置BH1750为快速模式
    my_BH1750.set_mode(BH1750_FAST_MODE);

    while(1)
    {
        run_time_count++;
        ets_sprintf(ShowTextBuff,"Run time %d \tBH1750 %3.3f",run_time_count,my_BH1750.read_data());
        printf("\r%s\r",ShowTextBuff);
        vTaskDelay(20 / portTICK_RATE_MS);
    }
}



