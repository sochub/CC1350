/********************************** (C) COPYRIGHT *******************************
* File Name          : I2C.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : VGA�л�ʱEDID������س���
*******************************************************************************/
#include "I2C.H"	 															/* �����������ͷ�ļ� */

#pragma  NOAREGS
/*******************************************************************************
* Function Name  : I2CM_Init
* Description    : I2C������ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void I2CM_Init(void)
{
	I2CM_CK_SE = 60;															    /* 100KHZ */
//	I2CM_CTRL |= bI2CM_IE;													/* �ж�ʹ�� */
	I2CM_CTRL |= bI2CM_EN;														/* ����ʹ��EN */
}

/*******************************************************************************
* Function Name  : I2CS_Init
* Description    : I2C�ӻ���ʼ��
* Input          : None
* Return         : None
*******************************************************************************/
void I2CS_Init(UINT8 addr, UINT8* pbuf )
{		
	I2CS0_DEV_A = addr;															  /* �����豸��ַ */
	I2CS1_DEV_A = addr;
	I2CS2_DEV_A = addr;
	I2CS3_DEV_A = addr;
	
	I2CS0_CTRL 	= bI2CS_IE_STASTO | bI2CS_DMA_EN;		  /* ��ʼ�ж�,����DMA */
	I2CS1_CTRL 	= bI2CS_IE_STASTO	| bI2CS_DMA_EN;
	I2CS2_CTRL 	= bI2CS_IE_STASTO	| bI2CS_DMA_EN;
	I2CS3_CTRL 	= bI2CS_IE_STASTO	| bI2CS_DMA_EN;
	I2CS0_CTRL |= bI2CS_EN;														/* ʹ��I2C�ӻ� */
	I2CS1_CTRL |= bI2CS_EN;
	I2CS2_CTRL |= bI2CS_EN;
	I2CS3_CTRL |= bI2CS_EN;

	I2CS0_DMA_H = (UINT16)pbuf>> 8; 
	I2CS0_DMA_L = (UINT16)pbuf;  
	I2CS1_DMA_H = (UINT16)pbuf>> 8; 
	I2CS1_DMA_L = (UINT16)pbuf;    
	I2CS2_DMA_H = (UINT16)pbuf>> 8; 
	I2CS2_DMA_L = (UINT16)pbuf; 
	I2CS3_DMA_H = (UINT16)pbuf>> 8; 
	I2CS3_DMA_L = (UINT16)pbuf;  
	
	IE_PWM_I2C = 1;																     /* �����ж� */
	E_DIS = 0;
	EA = 1;
}
	
/*******************************************************************************
* Function Name  : I2CM_CMD
* Description    : I2C������������
* Input          : None
* Return         : None
*******************************************************************************/
void I2CM_CMD(UINT8 cmd)
{
	I2CM_CTRL &= 0xfc;
	I2CM_CTRL |= cmd;
}

/*******************************************************************************
* Function Name  : I2CM_WaitACK
* Description    : I2C�����ȴ��ӻ�Ӧ��ACK
* Input          : None
* Return         : None
*******************************************************************************/
void I2CM_WaitACK(void)
{
	UINT16 timecount;
	
	/* �ȴ������������ */
	timecount = 0x00;
	while(!(I2CM_STAT & 0X80))
	{
		timecount++;
		if(timecount > 5000)
		{
			break;
		}
		mDelayuS(1);	
	}			
	I2CM_STAT |= 0x80;															/* ���жϱ�־ */
	
	timecount = 0x00;
	while(I2CM_CTRL & 0X10)													/* ��ѯ�豸ACK */
	{
		timecount++;
		if(timecount > 5000)
		{
			break;
		}
		mDelayuS(1);	
	}	
}

/*******************************************************************************
* Function Name  : I2CM_RecvData
* Description    : I2C��������1���ֽ�����
* Input          : None
* Return         : None
*******************************************************************************/
UINT8 I2CM_RecvData(void)
{
	UINT16 timecount;
		
	I2CM_CMD(I2CM_CMD_RX_ACK);  												/* ����һ�����ݲ��ظ�ACK */
	
	/* ����������� */
	while(!(I2CM_STAT & 0X80))												
	{
		timecount++;
		if(timecount > 5000)
		{
			return(0xFF);
		}
		mDelayuS(1);	
	}	
	
	I2CM_STAT |= 0x80;  														/* ���жϱ�־ */
	return((UINT8)I2CM_DATA);
}

/*******************************************************************************
* Function Name  : I2C_Interrupt
* Description    : I2C�жϷ�����
* Input          : None
* Return         : None
*******************************************************************************/
void I2C_Interrupt(void) interrupt 11
{
	IE_PWM_I2C = 0;																/* �����ж� */
	
	if(I2CX_INT & 0x20)
	{
		/* I2C�ж� */
		switch(I2CX_INT & 0xc0)
		{
			case 0x00:   
				I2CS0_STAT |= 0xF0;
				break;
				
			case 0x40:
				I2CS1_STAT |= 0xF0;
				break;

			case 0x80:
				I2CS2_STAT |= 0xF0;
				break;

			case 0xc0:
				I2CS3_STAT |= 0xF0;
				break;
		}
	}
	
	IE_PWM_I2C = 1;                              /* �����ж� */
}

