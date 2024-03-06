#include "dac.h"
#include "sys.h"	 
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DAC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
 #define RELOAD_VAL(hz) (u16)(84000000UL /256/hz) //������װֵ
 #define RELOAD_VAL1(hz)(u16)(84000000UL /256/hz1) //������װֵ
// #define RELOAD_VAL2(hz)(u16)(84000000UL /256/hz2) //������װֵ
u16 Sine12bit[256] = { //���Ҳ����
 2048, 2098, 2148, 2198, 2248, 2298, 2348, 2398, 2447, 2496,
 2545, 2594, 2642, 2690, 2737, 2785, 2831, 2877, 2923, 2968,
 3013, 3057, 3100, 3143, 3185, 3227, 3267, 3307, 3347, 3385,
 3423, 3460, 3496, 3531, 3565, 3598, 3631, 3662, 3692, 3722,
 3750, 3778, 3804, 3829, 3854, 3877, 3899, 3920, 3940, 3958,
 3976, 3992, 4007, 4021, 4034, 4046, 4056, 4065, 4073, 4080,
 4086, 4090, 4093, 4095, 4095, 4095, 4093, 4090, 4086, 4080,
 4073, 4065, 4056, 4046, 4034, 4021, 4007, 3992, 3976, 3958,
 3940, 3920, 3899, 3877, 3854, 3829, 3804, 3778, 3750, 3722,
 3692, 3662, 3631, 3598, 3565, 3531, 3496, 3460, 3423, 3385,
 3347, 3307, 3267, 3227, 3185, 3143, 3100, 3057, 3013, 2968,
 2923, 2877, 2831, 2785, 2737, 2690, 2642, 2594, 2545, 2496,
 2447, 2398, 2348, 2298, 2248, 2198, 2148, 2098, 2047, 1997,
 1947, 1897, 1847, 1797, 1747, 1697, 1648, 1599, 1550, 1501,
 1453, 1405, 1358, 1310, 1264, 1218, 1172, 1127, 1082, 1038,
 995, 952, 910, 868, 828, 788, 748, 710, 672, 635,
 599, 564, 530, 497, 464, 433, 403, 373, 345, 317,
 291, 266, 241, 218, 196, 175, 155, 137, 119, 103,
 88, 74, 61, 49, 39, 30, 22, 15, 9, 5,
 2, 0, 0, 0, 2, 5, 9, 15, 22, 30,
 39, 49, 61, 74, 88, 103, 119, 137, 155, 175,
 196, 218, 241, 266, 291, 317, 345, 373, 403, 433,
 464, 497, 530, 564, 599, 635, 672, 710, 748, 788,
 828, 868, 910, 952, 995, 1038, 1082, 1127, 1172, 1218,
 1264, 1310, 1358, 1405, 1453, 1501, 1550, 1599, 1648, 1697,
 1747, 1797, 1847, 1897, 1947, 1997 };
u16 Sine12bit1[256] = { //���Ҳ����
 2048, 2098, 2148, 2198, 2248, 2298, 2348, 2398, 2447, 2496,
 2545, 2594, 2642, 2690, 2737, 2785, 2831, 2877, 2923, 2968,
 3013, 3057, 3100, 3143, 3185, 3227, 3267, 3307, 3347, 3385,
 3423, 3460, 3496, 3531, 3565, 3598, 3631, 3662, 3692, 3722,
 3750, 3778, 3804, 3829, 3854, 3877, 3899, 3920, 3940, 3958,
 3976, 3992, 4007, 4021, 4034, 4046, 4056, 4065, 4073, 4080,
 4086, 4090, 4093, 4095, 4095, 4095, 4093, 4090, 4086, 4080,
 4073, 4065, 4056, 4046, 4034, 4021, 4007, 3992, 3976, 3958,
 3940, 3920, 3899, 3877, 3854, 3829, 3804, 3778, 3750, 3722,
 3692, 3662, 3631, 3598, 3565, 3531, 3496, 3460, 3423, 3385,
 3347, 3307, 3267, 3227, 3185, 3143, 3100, 3057, 3013, 2968,
 2923, 2877, 2831, 2785, 2737, 2690, 2642, 2594, 2545, 2496,
 2447, 2398, 2348, 2298, 2248, 2198, 2148, 2098, 2047, 1997,
 1947, 1897, 1847, 1797, 1747, 1697, 1648, 1599, 1550, 1501,
 1453, 1405, 1358, 1310, 1264, 1218, 1172, 1127, 1082, 1038,
 995, 952, 910, 868, 828, 788, 748, 710, 672, 635,
 599, 564, 530, 497, 464, 433, 403, 373, 345, 317,
 291, 266, 241, 218, 196, 175, 155, 137, 119, 103,
 88, 74, 61, 49, 39, 30, 22, 15, 9, 5,
 2, 0, 0, 0, 2, 5, 9, 15, 22, 30,
 39, 49, 61, 74, 88, 103, 119, 137, 155, 175,
 196, 218, 241, 266, 291, 317, 345, 373, 403, 433,
 464, 497, 530, 564, 599, 635, 672, 710, 748, 788,
 828, 868, 910, 952, 995, 1038, 1082, 1127, 1172, 1218,
 1264, 1310, 1358, 1405, 1453, 1501, 1550, 1599, 1648, 1697,
 1747, 1797, 1847, 1897, 1947, 1997 };
////uint32_t DualSine12bit[32];
void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_AHB1PeriphClockCmd(DAC_CH1_GPIO_CLK|DAC_CH2_GPIO_CLK, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(DAC_CLK, ENABLE);
  
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin =  DAC_CH1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DAC_CH1_GPIO_PORT, &GPIO_InitStructure);
	
		  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_TRIGGER;						//ʹ��TIM2��Ϊ����Դ
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//��ʹ��DAC�������
	//���ǲ��������ʵ��û���õ��������ó�����ֵ�������ṹ���Ա����Ϊ�գ�
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_Init(DAC_CH1_CHANNEL, &DAC_InitStructure);
	
  DAC_InitStructure.DAC_Trigger = DAC_TRIGGER1;						//ʹ��TIM2��Ϊ����Դ
  /* ����DAC ͨ��2 */
  /* ����DAC ͨ��1��2 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
	/* ʹ�� DAC��DMA���� */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}


/**
  * @brief  ����TIM
  * @param  ��
  * @retval ��
  */
 void DAC_TIM_Config(u32 hz,u32 hz1)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ84M */
  RCC_APB1PeriphClockCmd(DAC_TIM_CLK|DAC_TIM_CLK1, ENABLE);
	
  /* TIM2������ʱ������ */
 // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = RELOAD_VAL(hz);       									//��ʱ���� 20  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       							//Ԥ��Ƶ������Ƶ 84M / (0+1) = 84M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(DAC_TIM, &TIM_TimeBaseStructure);
	
  TIM_TimeBaseStructure.TIM_Period = RELOAD_VAL1(hz);       									//��ʱ���� 20  
	TIM_TimeBaseInit(DAC_TIM1, &TIM_TimeBaseStructure);
  /* ����TIM2����Դ */
  TIM_SelectOutputTrigger(DAC_TIM, TIM_TRGOSource_Update);
  TIM_SelectOutputTrigger(DAC_TIM1, TIM_TRGOSource_Update);
	/* ʹ��TIM2 */
  TIM_Cmd(DAC_TIM, ENABLE);
  TIM_Cmd(DAC_TIM1, ENABLE);
}

/**
  * @brief  ����DMA
  * @param  ��
  * @retval ��
  */
 void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* DAC1ʹ��DMA1 ͨ��7 ������5ʱ�� */
	RCC_AHB1PeriphClockCmd(DAC_DMA_CLK, ENABLE);
	
	/* ����DMA2 */
  DMA_InitStructure.DMA_Channel = DAC_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR1RD_Address;					//�������ݵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Sine12bit;			//�ڴ����ݵ�ַ DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									//���ݴ��䷽���ڴ�������
  DMA_InitStructure.DMA_BufferSize = sizeof(Sine12bit);																	//�����СΪ32�ֽ�	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//������������Ϊ��λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//�ڴ���������Ϊ��λ	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DAC_DMA_STREAM, &DMA_InitStructure);
	
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR2RD_Address;					//�������ݵ�ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Sine12bit1;			//�ڴ����ݵ�ַ DualSine12bit
	
	DMA_Init(DAC_DMA_STREAM1, &DMA_InitStructure);
  /* ʹ�� DMA_Stream */
  DMA_Cmd(DAC_DMA_STREAM, ENABLE); 
	DMA_Cmd(DAC_DMA_STREAM1, ENABLE); 

}
/*************************************************************
Function  :  set_Sine12bit
Description : �������������
Input  :   MAX(�����ֵ) MIN(�����ֵ)
return  :  none
*************************************************************/ 
 void Set_Sine12bit(float MAX,float MIN)
{
   int i;
   float  jiaodu=0;
   float MID=(MAX+MIN)/2.0f;                        //�м��ֵ
   if(MAX>3.3f) MAX=3.3f;
   else if(MAX<=MIN) MIN=0;
   for(i=0;i<256;i++)
         {
             jiaodu=i*0.0247369f;      //��i =127ʱ,��ʾΪ180��,����sin()�ǻ�����,������Ҫת��
             Sine12bit[i]=  ((float)sin(jiaodu)*(MAX-MID)+MID)*1241.212f;     //1241.212�Ǳ���,����4096/3.3  
             Sine12bit1[i]=  ((float)sin(jiaodu)*(MAX-MID)+MID)*1241.212f;     //1241.212�Ǳ���,����4096/3.3  					 
         }  
}

////void  Set_Period(u32 value)
////{
////     TIM_ARRPreloadConfig(TIM2,DISABLE);
////     TIM2->ARR = RELOAD_VAL(value);       //����Ԥװ��ֵ 
////     TIM_ARRPreloadConfig(TIM2,ENABLE);
////}





/**
  * @brief  DAC��ʼ������
  * @param  ��
  * @retval ��
  */
void DAC_Mode_Init(void)
{
//	uint32_t Idx = 0;  

	DAC_Config();
	DAC_TIM_Config(1000,1000);
	Set_Sine12bit(0,3);
	/* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
//  for (Idx = 0; Idx < 32; Idx++)
//  {
//    DualSine12bit[Idx] = (Sine12bit1[Idx] << 16) + (Sine12bit[Idx]);
//  }
	
	DAC_DMA_Config();
}









