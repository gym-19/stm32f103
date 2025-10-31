#include "eeprom.h"
#include "gpio_iic.h"

void eeprom_init(void)//初始化EEPROM使用的GPIO管脚
{
	IIC_Init();//调用初始化IIC的函数
}

void eeprom_byte_write(u8 daddr, u8 waddr, u8 data)//按字节写
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//发送内部存储空间的地址
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(data);//发送一个字节数据
	IIC_Wait_Ack();//等待ACK
	IIC_Stop();//产生结束信号
}

void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size)//按页写
{
	u8 i = 0;//循环变量
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//发送内部存储空间的地址
	IIC_Wait_Ack();//等待ACK
	for(i = 0; i < size; i++)
	{
		IIC_Send_Byte(buf[i]);//发送一个字节数据
		IIC_Wait_Ack();//等待ACK
	}
	IIC_Stop();//产生结束信号
}

u8 eeprom_curr_read(u8 daddr)//当前地址读
{
	u8 data = 0;//存储读取到的一个字节数据
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 0x01);//发送设备地址 + 读的标志
	IIC_Wait_Ack();//等待ACK
	data = IIC_Recv_Byte(0);//等待接收一个字节数据,并且不产生ACK
	IIC_Stop();//产生结束信号
	
	return data;//把读取到的数据返回
}

u8 eeprom_rand_read(u8 daddr, u8 waddr)//随机地址读
{
	u8 data = 0;//存储读取到的一个字节数据
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//发送内部存储空间的地址
	IIC_Wait_Ack();//等待ACK
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 0x01);//发送设备地址 + 读的标志
	IIC_Wait_Ack();//等待ACK
	data = IIC_Recv_Byte(0);//等待接收一个字节数据,并且不产生ACK
	IIC_Stop();//产生结束信号
	
	return data;//把读取到的数据返回
}

void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size)//顺序读
{
	u8 i = 0;//循环变量
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//发送设备地址 + 写的标志
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//发送内部存储空间的地址
	IIC_Wait_Ack();//等待ACK
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 0x01);//发送设备地址 + 读的标志
	IIC_Wait_Ack();//等待ACK
	for(i = 0; i < size; i++)//循环读取多个字节数据
	{
		if(i == size - 1)//判断是否是最后一次读取数据
			buf[i] = IIC_Recv_Byte(0);//读取最后一个字节数据,并且不产生ACK
		else
			buf[i] = IIC_Recv_Byte(1);//读取之前的数据,并且产生ACK
	}
	IIC_Stop();//产生结束信号
}







