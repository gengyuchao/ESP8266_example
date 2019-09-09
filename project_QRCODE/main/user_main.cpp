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

#include <stdio.h>
#include <stdlib.h>
 
//生成 OLED 的实例化对象
SSD1306I2C display(0x3c,GPIO_NUM_5,GPIO_NUM_4);// OLED_Address SCL SDA

#include "dht11_driver.hpp"

//生成 DHT11 的实例化对象
DHT_Sensor my_DHT11(GPIO_NUM_14,DHT11);
void task_DHT11_get(void *p)
{

    my_DHT11.DHTInit();

    while(1)
    {
        my_DHT11.DHTRead();
        // ESP_LOGI("DHT11","DHT: Temperature*100 =  %d *C, Humidity*100 = %d %% (GPIO%d)",
        //         (int) (my_DHT11.Sensor_Data.temperature * 100), (int) (my_DHT11.Sensor_Data.humidity * 100), my_DHT11.data_pin);

        //注意 格式控制符中 有效数字个数包括小数点
        //ets_printf("\rDistance:%07.2fcm ,Temperature = %06.2f *C, Humidity = %04.2f %% ",my_HC_SR04.Distance, (my_DHT11.Sensor_Data.temperature), (my_DHT11.Sensor_Data.humidity));

        vTaskDelay(1000 / portTICK_RATE_MS);
    }

}

extern "C" int ets_sprintf(char * buf,const char *fmt, ...);

#include "strings.h"
#include <vector>
#include "QrCode.hpp"
using namespace qrcodegen;


void Bilinear(uint8_t* inpBmpBuf, int src_x,int src_y,uint8_t* outpBmpBuf,int dstWidth,int dstHeight)
{
    double lx=1,ly=1;//缩小倍数
    if(src_x/dstWidth>1) {lx=src_x/(double)dstWidth;} 
    else{lx=src_x/(double)dstWidth;}
 
    if(src_y/dstHeight>1){ly=src_y/(double)dstHeight;}
    else{ly=src_y/(double)dstHeight;}
 
    int x1, x2, y1, y2, Fq11, Fq12, Fq21, Fq22;
        double x, y, r1, Fr1, Fr2, Fp;
        for (int i = 0; i < dstHeight; ++i){
            for (int j = 0; j < dstWidth; ++j){
                x = lx*j; // 原图像坐标
                y = ly*i;
                // 四个坐标值
                x1 = floor(x); x2 = x1 + 1;// 取整ceil向上，floor向下
                y1 = floor(y); y2 = y1 + 1;
                // 四个坐标对应的灰度值
                Fq11 = inpBmpBuf[x1*src_y+y1];
                Fq12 = inpBmpBuf[x1*src_y+y2];
                Fq21 = inpBmpBuf[x2*src_y+y1];
                Fq22 = inpBmpBuf[x2*src_y+y2];
                // x方向插值和y方向插值
                Fr1 = 0; Fr2 = 0;
 
                Fr1 = (x2 - x) / (x2 - x1)*Fq11 + (x - x1) / (x2 - x1)*Fq21;
                Fr2 = (x2 - x) / (x2 - x1)*Fq12 + (x - x1) / (x2 - x1)*Fq22;
                Fp = (y2 - y) / (y2 - y1)*Fr1 + (y - y1) / (y2 - y1)*Fr2;
                // 新图像灰度值赋值
                if (Fp >= 0 && Fp <= 255){
                    outpBmpBuf[i+j*dstHeight]=round(Fp)>124; // 新图像
                    //num_buf_big[j][i]=round(Fp)>80; // 新图像
                }
            }
        }
}

#define SHOW_SIZE 60

void Show_QRcode_Picture(std::string message)
{

    // Manual operation
    std::vector<QrSegment> segs =
        QrSegment::makeSegments(message.c_str());
    QrCode qr1 = QrCode::encodeSegments(
        segs, QrCode::Ecc::HIGH, 5, 10, 2, false);

    //创建二维码画布(放大)
    // uint8_t* QrCode_Image=(uint8_t*)malloc(qr1.getSize()*qr1.getSize());
    // uint8_t* QrCode_Image_BIG=(uint8_t*)malloc(SHOW_SIZE*SHOW_SIZE+1);

    // for (int y = 0; y < qr1.getSize(); y++) {
    //     for (int x = 0; x < qr1.getSize(); x++) {
    //         if(qr1.getModule(x, y)==0)
    //             QrCode_Image[x+y*qr1.getSize()]=0;//display.setPixel(x,y);
    //         else
    //             QrCode_Image[x+y*qr1.getSize()]=240;//display.clearPixel(x,y);
    //     }
    // }

    // Bilinear(QrCode_Image, qr1.getSize(),qr1.getSize(),QrCode_Image_BIG,SHOW_SIZE,SHOW_SIZE);

    // for (int y = 0; y < SHOW_SIZE; y++) {
    //     for (int x = 0; x < SHOW_SIZE; x++) {
    //         if(QrCode_Image_BIG[x+y*SHOW_SIZE]==true)
    //             display.setPixel(x,y);
    //         else
    //             display.clearPixel(x,y);
    //     }
    // }
    // free(QrCode_Image);
    // free(QrCode_Image_BIG);


    for (int y = 0; y < qr1.getSize(); y++) {
        for (int x = 0; x < qr1.getSize(); x++) {
            if(qr1.getModule(x, y)==0)
                display.setPixel(x,y);
            else
                display.clearPixel(x,y);
        }
    }



    display.display();
    
}



/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
extern "C" void app_main(void)
{
    char ShowTextBuff[100]={0};
    int run_time_count = 0;

    printf("SDK version:%s\n", esp_get_idf_version());
    
    display.init();//oled初始化
    display.flipScreenVertically();


    display.clear();//清屏

    sprintf(ShowTextBuff,"Hello ESP8266\n");
    display.display();//开始显示

    std::string Hello="Hello ESP8266";

    std::string message="";
    char char_message[60];
    

    xTaskCreate( task_DHT11_get, "task_DHT11_get", 1024, NULL, 1, NULL );

    vTaskDelay(1000 / portTICK_RATE_MS);

    while(1)
    {
        run_time_count++;
        display.clear();//清屏
        
        ESP_LOGI("DHT11","DHT: Temperature*100 =  %d *C, Humidity*100 = %d %% (GPIO%d)",
         (int) (my_DHT11.Sensor_Data.temperature * 100), (int) (my_DHT11.Sensor_Data.humidity * 100), my_DHT11.data_pin);

        sprintf(char_message,"DHT: Temperature*100 =  %d *C, Humidity*100 = %d %% time %d",(int) (my_DHT11.Sensor_Data.temperature * 100), (int) (my_DHT11.Sensor_Data.humidity * 100),run_time_count);
        message=Hello+char_message;
        Show_QRcode_Picture(message);


        display.display();//开始显示
        
        ESP_LOGI("OLED","%s",ShowTextBuff);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}



