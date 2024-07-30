
#ifndef _AD9833_H
#define _AD9833_H

#include "sys.h"
//AD9833 GPIO
#define AD9833_SDATA  PBout(5)  //SDATA
#define AD9833_SCLK   PBout(3)  //SCLK
#define AD9833_FSYNC  PBout(9)  //FSYNC

/* WaveMode */
#define AD9833_OUT_SINUS    ((0 << 5) | (0 << 1) | (0 << 3))//正弦波D5+D1
#define AD9833_OUT_TRIANGLE ((0 << 5) | (1 << 1) | (0 << 3))//D5加D1
#define AD9833_OUT_MSB      ((1 << 5) | (0 << 1) | (1 << 3))
#define AD9833_OUT_MSB2     ((1 << 5) | (0 << 1) | (0 << 3))

/* Registers */

#define AD9833_REG_CMD      (0 << 14)
#define AD9833_REG_FREQ0    (1 << 14)
#define AD9833_REG_FREQ1    (2 << 14)
#define AD9833_REG_PHASE0   (6 << 13)
#define AD9833_REG_PHASE1   (7 << 13)

/* Command Control Bits */

#define AD9833_B28          (1 << 13)
#define AD9833_HLB          (1 << 12)
#define AD9833_FSEL0        (0 << 11)
#define AD9833_FSEL1        (1 << 11)
#define AD9833_PSEL0        (0 << 10)
#define AD9833_PSEL1        (1 << 10)
//#define AD9833_PIN_SW       (1 << 9)
//#define AD9833_RESET        (1 << 8)
#define AD9833_CLEAR_RESET  (0 << 8)
#define AD9833_SLEEP1       (1 << 7)
#define AD9833_SLEEP12      (1 << 6)
#define AD9833_OPBITEN      (1 << 5)
//#define AD9833_SIGN_PIB     (1 << 4)
#define AD9833_DIV2         (1 << 3)
#define AD9833_MODE         (1 << 1)

#define TRI_WAVE 	0x2002  		//输出三角波D7=0,D6=0,D5=0,D4=0,D3=X,D2=0,D1=1,D0=0
#define SIN_WAVE 	0x2000		    //输出正弦波D7=0,D6=0,D5=0,D4=0,D3=X,D2=0,D1=0,D0=0
#define SIN_MSB  	0x2028		    //输出方波  D7=0,D6=0,D5=1,D4=0,D3=1,D2=0,D1=0,D0=0
#define SIN_MSB2  	0x2020		    //输出方波  D7=0,D6=0,D5=1,D4=0,D3=X,D2=0,D1=0,D0=0
void AD9833_Init(void);
void AD9833_Delay(void);
void AD9833_Write(u16 txdata);
void AD9833_SetFrequency(uint16_t Freq_SFR, double Freq);
void AD9833_SetPhase(uint16_t Phase_SFR,uint16_t Phase);
void AD9833_RESET(void);
void AD9833_SetWave(uint16_t WaveMode,uint16_t Freq_SFR,uint16_t Phase_SFR);
void AD9833_Setup(uint16_t  Freq_SFR,double Freq,uint16_t Phase_SFR,uint16_t Phase,uint16_t WaveMode);
void AD9833_Square_Wave(uint16_t  Freq_SFR,double Freq,uint16_t Phase_SFR,uint16_t Phase);
#endif









