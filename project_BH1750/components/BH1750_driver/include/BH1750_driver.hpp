/*
  This example code is created by gengyuchao used for Latest ESP8266 
  SDK based on FreeRTOS, esp-idf style.  (update:2019-08-01).
  Unless required by applicable law or agreed to in writing, this
  software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef __BH1750_DEVICE_HPP_
#define __BH1750_DEVICE_HPP_

    #include "esp_system.h"
    #include <string.h>
    #include "esp_log.h"    

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "driver/i2c.h"

    enum BH1750_MODE{
        BH1750_FAST_MODE,      //快速模式
        BH1750_ACCURATE_MODE   //精确模式
    };


    class IIC_Device
    {

        private:

        gpio_num_t sda_io_num; //I2C_MASTER_SDA_GPIO
        gpio_num_t scl_io_num; //I2C_MASTER_SCL_GPIO

        public:
        IIC_Device(gpio_num_t sda_io, gpio_num_t scl_io)
        :sda_io_num(sda_io),scl_io_num(scl_io)
        {
            gpio_init(sda_io_num,scl_io_num);
        }

        esp_err_t gpio_init(gpio_num_t sda_io, gpio_num_t scl_io);

        protected:
        void    IIC_Start(void);
        void    IIC_Stop(void);
        uint8_t IIC_Wait_Ask(void);        
        void    IIC_WriteByte(uint8_t data);
        uint8_t IIC_ReadByte(void);
        void    SendACK(uint8_t ack);

    };


    class BH1750_Device : public IIC_Device
    {

    private:
        uint8_t SlaveAddress = 0x46; //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                                    //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A

        uint8_t BUF[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //接收数据缓存区
        
        uint8_t BH1750_RecvByte();
        void BH1750_SendByte(uint8_t data);
        
        void Single_Write_BH1750(uint8_t REG_Address);
        void Multiple_Read_BH1750(void);

    public:

        float data = 0;

        BH1750_MODE currect_mode = BH1750_FAST_MODE;

        BH1750_Device(gpio_num_t sda_io_num, gpio_num_t scl_io_num) : IIC_Device(sda_io_num, scl_io_num)
        {
            init();
        }

        void init();
        float read_data();
        void set_mode(BH1750_MODE mode);
        esp_err_t delay_ms(uint32_t time);
    };

#endif


