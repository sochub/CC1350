/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 Time ��ʼ������ʱ������������ֵ��T2��׽���ܵ�                        
*******************************************************************************/
#include "Timer.H"
#include "Debug.H"
#pragma  NOAREGS

/* Global Variable */ 
sbit SCK_0 = P4^0;                                                             //Timer�жϼ��ӣ�����P40�������
sbit SCK_1 = P4^1;                                                             //��׽�ź�CAP1���ӣ�����P41�������
sbit SCK_2 = P4^2;                                                             //��׽�ź�CAP2���ӣ�����P42�������
UINT16 Cap2[2] = {0};
UINT16 Cap1[2] = {0};

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void) 
{
	mDelaymS(50);
	CfgFsys();                                                                //CH545ʱ��ѡ������       
	mInitSTDIO();                                                             //����0��ʼ��
	CH545UART0Alter();
	printf("CHIP_ID:%02x\n",(UINT16)CHIP_ID); 
	printf("EXAM Time\n");
	SCK_0 = 0;
	SCK_1 = 0;
	SCK_2 = 0;
	
#if Timer0_INTERRUPT
	printf("T0 Test ...\n");
	mTimer0Clk12DivFsys();                                                     //T0��ʱ��ʱ������ FREQ_SYS/12
	mTimer_x_ModInit(0,1);                                                     //T0 ��ʱ��ģʽ���� ģʽ1 16λ��ʱ��
	mTimer_x_SetData(0,2000);                                                  //T0��ʱ����ֵ 24MHZ 1MS�ж�
	TR0 = 1;                                                                   //T0��ʱ������
	ET0 = 1;                                                                   //T0��ʱ���жϿ���
	EA  = 1;
	
#endif	
	
#if Timer1_INTERRUPT
	printf("T1 Test ...\n");
	mTimer1Clk12DivFsys();                                                     //T1��ʱ��ʱ������ FREQ_SYS/12
	mTimer_x_ModInit(1,1);                                                     //T1 ��ʱ��ģʽ���� ģʽ1 16λ��ʱ��
	mTimer_x_SetData(1,2000);                                                  //T1��ʱ����ֵ 24MHZ 1MS�ж�
	TR1 = 1;                                                                   //T1��ʱ������
	ET1 = 1;                                                                   //T1��ʱ���жϿ���
	EA  = 1;
	
#endif	

#if Timer2_INTERRUPT
	printf("T2 Test ...\n");
	mTimer2Clk12DivFsys();                                                     //T2��ʱ��ʱ������ FREQ_SYS/12
	mTimer_x_ModInit(2,0);                                                     //T2 ��ʱ��ģʽ����
	mTimer_x_SetData(2,4000);                                                 //T2��ʱ����ֵ FREQ_SYS=24MHz,2ms�ж�
	TR2 = 1;                                                                   //T2��ʱ������
	ET2 = 1;                                                                   //T2��ʱ���жϿ���
	EA  = 1;	
	
#endif	

#if T2_CAP
	printf("T2_CAP Test ...\n");
	P1_MOD_OC &= ~(1<<1);                                                      //CAP2 ��������
	P1_DIR_PU &= ~(1<<1);
	P1_MOD_OC &= ~(1<<0);                                                      //CAP1 ��������
	P1_DIR_PU &= ~(1<<0);
	mTimer2Clk12DivFsys();                                                     //T2��ʱ��ʱ������ FREQ_SYS/12
	mTimer_x_SetData(2,0);                                                     //T2 ��ʱ��ģʽ���ò�׽ģʽ
	CAP2Init(1);                                                               //T2 CAP2(P11)���ã������ز�׽
	CAP1Init(1);                                                               //T2 CAP1(P10)���ã������ز�׽
	TR2 = 1;                                                                   //T2��ʱ������
	ET2 = 1;                                                                   //T2��ʱ���жϿ���
	EA  = 1;
		
#endif

	while(1){ }
}

#if Timer0_INTERRUPT
/*******************************************************************************
* Function Name  : Timer0Interrupt
* Description    : Timer0 �жϷ��������ʱ1ms
* Input          : None
* Return         : None
*******************************************************************************/
void Timer0Interrupt(void) interrupt INT_NO_TMR0                       //Timer0�жϷ������
{  
	SCK_0 = ~SCK_0; ;
	mTimer_x_SetData(0,2000);
}

#endif


#if Timer1_INTERRUPT
/*******************************************************************************
* Function Name  : Timer1Interrupt
* Description    : Timer1 �жϷ��������ʱ1ms
* Input          : None
* Return         : None
*******************************************************************************/
void Timer1Interrupt(void) interrupt INT_NO_TMR1                       //Timer1�жϷ������
{  
	SCK_0 = ~SCK_0;
	mTimer_x_SetData(1,2000);
}
#endif

/*******************************************************************************
* Function Name  : Timer2Interrupt
* Description    : Timer2 �жϷ������
* Input          : None
* Return         : None
*******************************************************************************/	
void Timer2Interrupt(void) interrupt INT_NO_TMR2                       //Timer2�жϷ������
{
#if Timer2_INTERRUPT
	static UINT16 tmr2=0;
 
	if(TF2)
	{
		TF2 = 0;                                                             //��ն�ʱ��2����ж�,���ֶ���
		tmr2++;
		
		if(tmr2 == 10)                                                       //��ʱ20ms                                                    
		{
			tmr2 = 0;
			SCK_0 = ~SCK_0;
		}
	}
	
#endif

#if  T2_CAP
	if(EXF2)                                                                //T2ex��ƽ�仯�ж��жϱ�־
	{
		Cap2[0] = RCAP2;                                                    //T2EX
		printf("CAP2 %04x\n",Cap2[0]-Cap2[1]);
		SCK_2 = ~SCK_2;
		Cap2[1] = Cap2[0];
		EXF2 = 0;                                                           //���T2ex��׽�жϱ�־
	}
	if(CAP1F)                                                               //T2��ƽ��׽�жϱ�־
	{
		Cap1[0] = T2CAP1;                                                    //T2;
		printf("CAP1 %04x\n",Cap1[0]-Cap1[1]);
		SCK_1 = ~SCK_1;
		Cap1[1] = Cap1[0];
		CAP1F = 0;                                                          //���T2��׽�жϱ�־
	}
	
#endif
}