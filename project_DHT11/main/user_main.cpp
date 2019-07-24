/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

#include "dht11_driver.hpp"

//生成 DHT11 的实例化对象
DHT_Sensor my_DHT11(GPIO_NUM_5,DHT11);
/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
extern "C" void app_main(void)
{
    printf("SDK version:%s\n", esp_get_idf_version());
    my_DHT11.DHTInit();

    while(1)
    {
        my_DHT11.DHTRead();
        ESP_LOGI("DHT11","DHT: Temperature*100 =  %d *C, Humidity*100 = %d %% (GPIO%d)",
                (int) (my_DHT11.Sensor_Data.temperature * 100), (int) (my_DHT11.Sensor_Data.humidity * 100), my_DHT11.data_pin);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}



