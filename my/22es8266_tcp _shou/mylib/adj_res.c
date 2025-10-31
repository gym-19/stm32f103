#include "adj_res.h"


//初始化
void adj_res_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	ADC_InitTypeDef Adc_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	//通过APB2总线使能时钟
	
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,  &Gpio_Value);//初始化gpioc管脚
	
	Adc_Value.ADC_ContinuousConvMode = DISABLE;//关闭连续转换模式
	Adc_Value.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	Adc_Value.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//关闭外部触发
	Adc_Value.ADC_Mode = ADC_Mode_Independent;//单独转换模式
	Adc_Value.ADC_NbrOfChannel = 10;//十号通道
	Adc_Value.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &Adc_Value);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	//adc1通道10的配制
	
	ADC_Cmd(ADC1, ENABLE);//使能adc1的转换功能
	
	
	
}

int  get_adj_res_value(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//以软件方式开启转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//死等转换结束
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	
	return ADC_GetConversionValue(ADC1);//获取转换结果并返回
	
}


