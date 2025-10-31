#include "adj_res.h"

void adj_res_init(void)//初始化高精度可调电阻使用的管脚
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	ADC_InitTypeDef Adc_Value;//定义了初始化ADC结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	//通过APB2总线使能GPIOC组以及ADC1的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AIN;//选择了模拟输入的模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;//选择了0号管脚
	GPIO_Init(GPIOC,&Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Adc_Value.ADC_ContinuousConvMode = DISABLE;//选择了关闭连续转换模式
	Adc_Value.ADC_DataAlign = ADC_DataAlign_Right;//选择了数据右对齐
	Adc_Value.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//选择了关闭外部触发
	Adc_Value.ADC_Mode = ADC_Mode_Independent;//选择了单独的转换模式
	Adc_Value.ADC_NbrOfChannel = 10;//选择了10号转换通道
	Adc_Value.ADC_ScanConvMode = DISABLE;//选择了关闭扫描模式
	ADC_Init(ADC1, &Adc_Value);//按照上述配置初始化ADC1
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	//配置ADC1的通道10
	
	ADC_Cmd(ADC1, ENABLE);//使能ADC1的转换功能
}

int get_adj_res_value(void)//获取高精度可调电阻经过AD转换以后的数值
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//以软件的方式开启ADC1的转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//以死等的方式等待转换结束
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);//清除ADC1的转换结束的标志
	return ADC_GetConversionValue(ADC1);//获取ADC1转换以后的结果并且返回
}








