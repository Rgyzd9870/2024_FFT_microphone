#ifndef __DISP_H
#define __DISP_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 							   
void clear_point(u16 num);//������ʾ����ǰ��	
void Set_BackGround(void);//���ñ���
void Lcd_DrawNetwork(void);//������
float get_vpp(u16 *buf);//��ȡ���ֵ
void DrawOscillogram(u16* buf);//������ͼ	
#endif 
