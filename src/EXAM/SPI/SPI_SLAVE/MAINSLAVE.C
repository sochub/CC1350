/********************************** (C) COPYRIGHT *******************************
* File Name          : MAINSLAVE.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 SPI设备例子演示，连接SPI主机进行数据收发
*******************************************************************************/
#include "CH545.H"
#include "Debug.H"
#include "SPI.H"

/*硬件接口定义*/
/******************************************************************************
使用CH545 硬件SPI接口
         CH545
         P1.4——SCS
         P1.5——MOSI
         P1.6——MISO
         P1.7——SCK
*******************************************************************************/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	UINT8 ret,i=0;
	
	CfgFsys();
	mDelaymS(5);                                                               //修改系统主频，建议稍加延时等待主频稳定
	mInitSTDIO();                                                             //串口0初始化
	printf("EXAM SPI Slave\n");
	SPISlvModeSet();                                                          //SPI从机模式设置
	
	while(1)
	{
#ifndef SPI_INTERRUPT
		ret = CH545SPISlvRead();                                               //主机保持CS=0
		CH545SPISlvWrite(ret^0xFF);                                            //SPI等待主机把数据取走,SPI 主机每次读之前先将CS=0，读完后CS=1
		printf("Read#%02x\n",(UINT16)ret);

#endif
	}
}
