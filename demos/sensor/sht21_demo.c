
#include "uds/uds_i2c.h"
#include "uds/uds.h"
#define SHT20_READ_ADDR   0X81
#define SHT20_WRITE_ADDR  0x80
#define SHT20_MEASURE_TEMP_CMD      0xE3
#define SHT20_MEASURE_RH_CMD        0xE5
#define SHT20_SOFT_RESET_CMD        0xFE




//******************************************************************************
// fn : SHT20_Convert
//
// brief : 将原始的AD值转成相应的温度值与湿度值
//
// param : value-> ad值
//         isTemp-> >0 温度值，=0 湿度值
//
// return : 直观的温度值
float SHT20_Convert(uint16_t value,uint8_t isTemp)
{
  float tmp = 0.0;
  if(isTemp)
  {
    tmp = -46.85 + (175.72* value)/(1 << 16);
  }
  else
  {
    tmp = -6 + (125.0 *value)/(1<<16);
  }
  return tmp;
}

void demo_sht21_iic(void)
{
    uint8_t cmd;
    s32_t ret;
    uint16_t data_raw_temp,data_raw_rh;
    volatile float temp =0 ;
    uint8_t pDATA[3] = {0,0,0};
    uint16_t slave_add;

    dal_i2c_iotype iotype;
    dal_frequence fre;
    
    i2c_init_t i2c1;
    i2c1.i2c_idx = DAL_I2C_ID1;
    i2c1.mode = DAL_I2C_MODE_MASTER;
    i2c1.freq_khz = DAL_FRE_100KHZ;//10khz		
    i2c1.s_add_size = DAL_I2C_7BIT_ADD;
    
    uds_driv_t device;
    uds_driv_init();
    uds_i2c_dev_install("I2C1", (void *)&i2c1);
    
    device = uds_dev_open("I2C1",2);
    if(!device)
        while(1); 
        
    //iotype   = I2C_IO_MASTER_ORIGIN;
    //uds_dev_ioctl(device,I2C_SET_IOTYPE,(void *)&slave_add,sizeof(uint16_t));
    


    iotype = I2C_IO_MASTER_ORIGIN;
    los_dev_ioctl(device,I2C_SET_IOTYPE,(void *)&iotype,sizeof(uint16_t)); //set master read/write mode.


    slave_add = SHT20_WRITE_ADDR;
    los_dev_ioctl(device,I2C_SET_SLAVE_WRITE_ADD,(void *)&slave_add,sizeof(uint16_t));//set write address 

    slave_add = SHT20_READ_ADDR;
    los_dev_ioctl(device,I2C_SET_SLAVE_READ_ADD,(void *)&slave_add,sizeof(uint16_t));

    fre = DAL_FRE_400KHZ;//
    los_dev_ioctl(device,I2C_SET_FREQUENCY,(void *)&fre,sizeof(uint16_t)); //set master read/write mode.

    iotype = I2C_IO_MASTER_ORIGIN;
    los_dev_ioctl(device,I2C_SET_IOTYPE,(void *)&iotype,sizeof(uint16_t)); //set master read/write mode.


    slave_add = SHT20_WRITE_ADDR;
    los_dev_ioctl(device,I2C_SET_SLAVE_WRITE_ADD,(void *)&slave_add,sizeof(uint16_t));//set write address 

    slave_add = SHT20_READ_ADDR;
    los_dev_ioctl(device,I2C_SET_SLAVE_READ_ADD,(void *)&slave_add,sizeof(uint16_t));

    while (1)
    {
        
        cmd = SHT20_MEASURE_TEMP_CMD;
        ret = los_dev_write(device,0,&cmd,1,10000);//write cmd to register.
        if(ret == -1)
            while(1);       
        ret = los_dev_read(device,0,pDATA,3,10000);
        data_raw_temp = pDATA[0];
        data_raw_temp <<= 8;
        data_raw_temp += (pDATA[1] & 0xfc);
        temp = SHT20_Convert(data_raw_temp,1);
        printf("temp = %.1f \r\n",temp);

        temp = 0;

        cmd = SHT20_MEASURE_RH_CMD;
        ret = uds_dev_write(device,0,&cmd,1,10000);
        if(ret == -1)
            while(1);
        ret = uds_dev_read(device,0,pDATA,3,10000);
        if(ret == -1)
            while(1);
        data_raw_rh = pDATA[0];
        data_raw_rh <<= 8;
        data_raw_rh += (pDATA[1] & 0xf0);
        temp = SHT20_Convert(data_raw_rh,0);
        printf("rh = %.1f \r\n",temp);
        temp = 0;
  }

}
