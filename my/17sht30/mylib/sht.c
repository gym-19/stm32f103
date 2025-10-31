#include "sht.h"
#include "gpio_iic.h"

void sht_init(void)//初始化SHT30使用的管脚
{
	IIC_Init();
}

void sht_write_mode(void)//设置SHT30传感器的采样频率
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_HIGH >> 8);//发送采样指令的高字节
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_HIGH & 0xFF);//发送采样指令的低字节
	IIC_Wait_Ack();//等待ACK
	IIC_Stop();//产生结束信号
}

void sht_write_read_cmd(void)//发送读取数据的指令
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(FETCH_DATA >> 8);//发送获取数据指令的高字节
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(FETCH_DATA & 0xFF);//发送获取数据指令的低字节
	IIC_Wait_Ack();//等待ACK
}

void sht_read_data(double sht_data[2])//获取采集并转换后的数据
{
	u8 i = 0;//循环变量
	u8 buf[6] = {0};//存储从SHT30读取出的6个字节数据
	u16 temp = 0;//存储温度的高字节 + 低字节
	u16 hum = 0;//存储湿度的高字节 + 低字节
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR | 0x01);//发送设备地址 + 读的标志
	IIC_Wait_Ack();//等待ACK
	for(i = 0; i < 6; i++)//循环读取数据
	{
		if(i == 5)//判断是否是读取的最后一个字节数据
			buf[i] = IIC_Recv_Byte(0);//读取最后一个字节数据并且不产生ACK
		else
			buf[i] = IIC_Recv_Byte(1);//读取之前的数据并且产生ACK
	}
	IIC_Stop();//产生结束信号
	//把温度的高字节和低字节数据拼成一个2byte的数据
	temp = buf[0] & 0xFF;
	temp <<= 8;
	temp |= buf[1];
	//把湿度的高字节和低字节数据拼成一个2byte的数据
	hum = buf[3] & 0xFF;
	hum <<= 8;
	hum |= buf[4];
	//把拼好的数据带入转换公式
	sht_data[0] = (double)(-45 + 175 * (double)(temp) / 65535);
	sht_data[1] = (double)(100 * (double)(hum) / 65535);
}
