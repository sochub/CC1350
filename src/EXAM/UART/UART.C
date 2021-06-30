/********************************** (C) COPYRIGHT *******************************
* File Name          : UART.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 UART��ʼ�����
*******************************************************************************/
#include "UART.H"

#pragma  NOAREGS

/*******************************************************************************
* Function Name  : CH545UART1Init
* Description    : UART1��ʼ������,����Ĭ������ P26 P27
* Input          : None
* Return         : None
*******************************************************************************/
void CH545UART1Init(void)
{
	SCON1 &= ~bU1SM0;                            //ѡ��8λ����ͨѶ
	SCON1 |= bU1SMOD;                            //����ģʽ
	SCON1 |= bU1REN;                             //ʹ�ܽ���
	SBAUD1 = 0 - FREQ_SYS/16/UART1_BUAD;         //����������
	SIF1 = bU1TI;                                //��շ�����ɱ�־
	
#ifdef UART1_INTERRUPT                           //�����ж�ʹ��
	IE_UART1 = 1;
	EA = 1;
	
#endif
}

/*******************************************************************************
* Function Name  : CH545UART1Alter
* Description    : CH545����1����ӳ��,����ӳ�䵽 P16 P17
* Input          : None
* Return         : None
*******************************************************************************/
void CH545UART1Alter(void)
{
	P1_MOD_OC |= (3<<6);                                                   //׼˫��ģʽ
	P1_DIR_PU |= (3<<6);
	PIN_FUNC |= bUART1_PIN_X;                                              //�������Ÿ��ù���
}

/*******************************************************************************
* Function Name  : CH545UART1RcvByte
* Description    : CH545UART1����һ���ֽ�
* Input          : None
* Return         : SBUF
*******************************************************************************/
UINT8 CH545UART1RcvByte(void)
{
	while((SIF1&bU1RI) == 0)
	{
			;    //��ѯ����
	}
	SIF1 = bU1RI;                                                          //��������ж�
	return SBUF1;
}

/*******************************************************************************
* Function Name  : CH545UART1SendByte
* Description    : CH545UART1����һ���ֽ�
* Input          : SendDat��Ҫ���͵�����
* Return         : None
*******************************************************************************/
void CH545UART1SendByte(UINT8 SendDat)
{
	SBUF1 = SendDat;
	while((SIF1&bU1TI) == 0)
	{
			;    //��ѯ�ȴ��������
	}
	SIF1 = bU1TI;                                                          //�����������ж�
}

#ifdef UART1_INTERRUPT
/*******************************************************************************
* Function Name  : UART1Interrupt
* Description    : UART1 �жϷ������
* Input          : None
* Return         : None
*******************************************************************************/
void UART1Interrupt(void) interrupt INT_NO_UART1 using 1                //����1�жϷ������,ʹ�üĴ�����1
{
	UINT8 dat;
	
	if(SIF1&bU1RI)
	{
		SIF1 = bU1RI;                                                     //����������ж�
		dat = SBUF1;
		CH545UART1SendByte(dat);
	}
}

#endif