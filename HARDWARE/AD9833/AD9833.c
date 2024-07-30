#include "AD9833.h"
#include "delay.h"
//***************************
//		Pin assign	   	
//			STM32			AD9833
//		GPIOB_Pin_9 		---> FSYNC
//		GPIOB_Pin_3 		---> SCK
//		GPIOB_Pin_5 		---> DAT

//***************************	

	/*端口定义 */ 
	#define PORT_FSYNC	GPIOB
	#define PIN_FSYNC		GPIO_Pin_9

	#define PORT_SCK		GPIOB
	#define PIN_SCK			GPIO_Pin_3

	#define PORT_DATA		GPIOB
	#define PIN_DATA		GPIO_Pin_5
	#define PI 3.14159265358979323846

/**************************************
*   函 数 名: ad9833_init
*   功能说明: ad9833初始化
*   形    参: 无
*   返 回 值: 无
*************************************/
void AD9833_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = PIN_FSYNC|PIN_SCK|PIN_DATA; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**************************************
*   函 数 名: AD9833_Delay
*   功能说明: ad9833延迟
*   形    参: 无
*   返 回 值: 无
*************************************/
void AD9833_Delay(void)
{
    uint16_t i;
    for (i = 0; i < 1; i++);
}
/**************************************
*   函 数 名: ad9833_write_data
*   功能说明: ad9833写入16位数据
*   形    参: txdata：待写入的16位数据
*   返 回 值: 无
*************************************/
void AD9833_Write(u16 txdata)
{
    int i;
    AD9833_SCLK=1;
		AD9833_FSYNC=1;//SCK-FSYNC建立时间最小为5ns
    AD9833_FSYNC=0;//传输开始
    //写16位数据
    for(i=0;i<16;i++)
    {
        if (txdata & 0x8000)
				{AD9833_SDATA=1;}
        else
				{AD9833_SDATA=0;}
//        AD9833_Delay();//数据保持时间3ns
        AD9833_SCLK=0;//时钟周期最小25ns，低电平和高电平至少各持续10ns
//        AD9833_Delay();
        AD9833_SCLK=1;
        txdata<<=1;
    }
    AD9833_FSYNC=1;//转换结束
}
/**************************************
*   函 数 名: AD9833_SetFrequency
*   功能说明: ad9833设置频率寄存器
*   形    参: reg：待写入的频率寄存器 0-0x4000,1-0x8000
              Freq：频率值
*   返 回 值: 无
选择频率寄存器并写好频率
*************************************/
void AD9833_SetFrequency(uint16_t Freq_SFR, double Freq)
{
	uint16_t frequence_LSB,frequence_MSB;
	double   frequence_mid,frequence_DATA;
	uint32_t frequence_hex;
/*********************************计算频率的16进制值***********************************/
	frequence_mid=268435456/25000000;//适合25M晶振，
	//如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz
	frequence_DATA=Freq;
	frequence_DATA=frequence_DATA*frequence_mid;
//	frequence_hex=frequence_DATA;
	frequence_hex=(uint32_t)(frequence_DATA)+(uint32_t)(4430*frequence_mid);  //这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
	//+(uint32_t)(4430*frequence_mid)，为我根据实际情况调整参数，相当于使输入的freq+4430Hz，可以不加。或者freq/0.9，最终输出与希望得到的值相似
	//仍有少许偏差，使用者还可以继续微调参数。
	frequence_LSB=frequence_hex&0xffff; //frequence_hex低16位送给frequence_LSB
	frequence_LSB=frequence_LSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位
	frequence_MSB=(frequence_hex>>14)&0xffff; //frequence_hex高16位送给frequence_HSB
	frequence_MSB=frequence_MSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位

	if(Freq_SFR==0)				  //把数据设置到设置频率寄存器0
	{
		frequence_LSB=frequence_LSB+0x4000;
		frequence_MSB=frequence_MSB+0x4000;
		//使用频率寄存器0输出波形
		}
	if(Freq_SFR==1)				//把数据设置到设置频率寄存器1
	{	
		 frequence_LSB=frequence_LSB+0x8000;
		 frequence_MSB=frequence_MSB+0x8000;
	}
		AD9833_Write(0x2000); //选择数据一次写入，B28位为1,D13
		AD9833_Write(frequence_LSB); //L14，选择频率寄存器1的低14位输入D15+D14
		AD9833_Write(frequence_MSB); //H14 频率寄存器1为高14位输入

	}
/**************************************
*   函 数 名: AD9833_SetPhase
*   功能说明: ad9833设置相位寄存器
*   形    参: Phase_SFR:待写入的相位寄存器   
             Phase：相位值
*   返 回 值: 无
*************************************/
void AD9833_SetPhase(uint16_t Phase_SFR,uint16_t Phase)
{
	uint16_t Phaser;
	uint16_t PhaseLSB;
	PhaseLSB=(uint16_t)(Phase/360*4096);//除以360°；12位
	PhaseLSB=PhaseLSB&0x0FFF;//12位
	if(Phase_SFR==0)				  //把数据设置到设置相位寄存器0
		{
			Phaser=0xC000; //选择数据一次写入，D15+D14+D130,

	    }
		if(Phase_SFR==1)				//把数据设置到设置相位寄存器1
		{	
			Phaser=0xD000; //选择数据一次写入，D15+D14+D131，
    }
		PhaseLSB=Phaser+PhaseLSB;
		AD9833_Write(PhaseLSB);
}
//复位
void AD9833_RESET(void)
{
	AD9833_Write(0x0100);
}
/**************************************
*   函 数 名: AD9833_SetWave
*   功能说明: ad9833设置输出波形
*   形    参: WaveMode：输出波形类型 
              Freq_SFR：输出的频率寄存器类型
              Phase_SFR：输出的相位寄存器类型
*   设置VOUT,选择频率寄存器和相位寄存器，需要与初始化设置一置
*************************************/
void AD9833_SetWave(uint16_t WaveMode,uint16_t Freq_SFR,uint16_t Phase_SFR)
	{
			uint16_t Wave = 0;
			uint16_t WaveOut;
			if(WaveMode==0)				  //三角波
			{
			 Wave=TRI_WAVE;//D15-D8=0;D7=0,D6=0,D5=0,D4=0,D3=X,D2=0,D1=1,D0=0，D13=1；
			if(WaveMode==1)				//正弦波
			{
				Wave=SIN_WAVE;//D15-D8=0;D7=0,D6=0,D5=0,D4=0,D3=X,D2=0,D1=0,D0=0，D13=1
			}
			//Freq_SFR:D11=0-0,1-1;Phase_SFR:0-0,1-1;D11=0,D10=0
			//D15-D14:D13-D12可随意设置，一般设置D13=1；
			//D9=0，D8=0；
			WaveOut = ( Wave+ Freq_SFR+Phase_SFR);
//			delay_ms(1000);
//			delay_ms(1000);
			AD9833_Write(WaveOut);
//			AD9833_Write(0x00C0);
		}
	}
	
/**************************************
*   函 数 名: AD9833_Setup
*   功能说明: ad9833设置输出
*   形    参: Freq_SFR：频率寄存器类型
              Freq    ：频率值
              Phase_SFR：相位寄存器类型
              Phase：相位值
              WaveMode：波形类型
*   返 回 值: 无
*************************************/
void AD9833_Setup(uint16_t  Freq_SFR,double Freq,uint16_t Phase_SFR,uint16_t Phase,uint16_t WaveMode)
{
    uint16_t Fsel,Psel;
    AD9833_RESET(); //复位AD9833
    AD9833_SetFrequency(Freq_SFR,Freq);//选择频率寄存器,并输入频率
    AD9833_SetPhase(Phase_SFR,Phase);//选择相位寄存器，并输入相位
		if(Freq_SFR==0)				  //从频率寄存器0读取数据,D11=0
		{
			Fsel=0x0000;

			}
		if(Freq_SFR==1)				//从频率寄存器1读取数据,D11=1;
		{	
			Fsel=0x0800;
		}
		if(Phase_SFR==0)				  //从频率寄存器0读取数据,D10=0
		{
			Psel=0x0000;

			}
		if(Phase_SFR==1)				//从频率寄存器1读取数据,D10=1;
		{	
			Psel=0x0400;
		}
    
    AD9833_SetWave(WaveMode,Fsel,Psel);
		
}

/**************************************
*   函 数 名: AD9833_Square_Wave
*   功能说明: ad9833设置输出方波

*   返 回 值: 无
*************************************/
void AD9833_Square_Wave(uint16_t  Freq_SFR,double Freq,uint16_t Phase_SFR,uint16_t Phase)                   
{
    AD9833_RESET();
    AD9833_Write(0x2100); // 初始化设置，如果是为了重置设备，之后需要清除RESET位
    // 配置频率和相位（看起来你的设置是正确的）
    AD9833_SetFrequency(Freq_SFR,Freq);
    AD9833_SetPhase(Phase_SFR,Phase);
    // 设置为方波输出，确保RESET位被清除（D13为0），并正确设置D5和D1
    AD9833_Write(0x2028); // 这里假设0x2028是正确设置方波的控制字
}

/**************************************
*   函 数 名: AD9833_Square_Wave
*   功能说明: ad9833设置输出方波

*   返 回 值: 无
*************************************/
void AD9833_None_Wave(uint16_t  Freq_SFR,double Freq,uint16_t Phase_SFR,uint16_t Phase)                   
{
    AD9833_RESET();
    AD9833_Write(0x2100); // 初始化设置，如果是为了重置设备，之后需要清除RESET位
    // 配置频率和相位（看起来你的设置是正确的）
    AD9833_SetFrequency(Freq_SFR,Freq);
    AD9833_SetPhase(Phase_SFR,Phase);
    // 设置为方波输出，确保RESET位被清除（D13为0），并正确设置D5和D1
    AD9833_Write(0x20C0); // 这里假设0x2028是正确设置方波的控制字
}

