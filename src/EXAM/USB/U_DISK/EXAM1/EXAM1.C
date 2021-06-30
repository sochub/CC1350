/********************************** (C) COPYRIGHT *******************************
* File Name          : EXAM1.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/03
* Description        :
 CH545 C���Ե�U���ļ��ֽڶ�дʾ�������ļ�ָ��ƫ�ƣ��޸��ļ����ԣ�ɾ���ļ��Ȳ���
 ֧��: FAT12/FAT16/FAT32
 ע����� CH545UFI.LIB/USBHOST.C/DEBUG.C
 ע��ʹ��CH545UFI.LIBע��(�������ڴ�ģʽ���ó�:small)
*******************************************************************************/
#include "absacc.h"
#include "DEBUG.H"
#include "CH545UFI.C"
#include "USBHOST.H"  

#pragma NOAREGS

UINT8X buf[100];	

/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ������״̬,�����������ʾ������벢ͣ��
* Input          : iError����������
* Return         : None
*******************************************************************************/
void mStopIfError(UINT8 iError)
{
	if(iError == ERR_SUCCESS)
	{
		return;    /* �����ɹ� */
	}
	printf("Error: %02X\n", (UINT16)iError);  /* ��ʾ���� */
	/* ���������,Ӧ�÷����������Լ�CH554DiskStatus״̬,�������CH549DiskReady��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
		 ���������Ĵ�������:
		 1������һ��CH549DiskReady,�ɹ����������,����Open,Read/Write��
		 2�����CH549DiskReady���ɹ�,��ôǿ�н���ͷ��ʼ����(�ȴ�U�����ӣ�CH554DiskReady��) */
	while(1)
	{
	}
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	UINT8 s,c,i;
	UINT16 TotalCount;
	
	CfgFsys();
	mDelaymS(20);                                                              //�޸���Ƶ���Լ���ʱ�ȴ���Ƶ�ȶ�
	mInitSTDIO();                                                            //��ʼ������0Ϊ���ü����ͨ�����ڼ����ʾ����
	printf("start ...\n");
	InitUSB_Host();
	CH549LibInit();                                                          //��ʼ��CH549�������֧��U���ļ�
	printf("start ...:%x\n",(UINT16)CH549GetVer());
	FoundNewDev = 0;
	while(1)
	{
		s = ERR_SUCCESS;
		if(UIF_DETECT)                                                   // �����USB��������ж�����
		{
			UIF_DETECT = 0;                                                 // �������жϱ�־
			s = AnalyzeRootHub();                                          // ����ROOT-HUB״̬
			if(s == ERR_USB_CONNECT)
			{
				FoundNewDev = 1;
			}
		}
		if(FoundNewDev || s == ERR_USB_CONNECT)                         // ���µ�USB�豸����
		{
			FoundNewDev = 0;
			mDelaymS(200);                                               // ����USB�豸�ղ�����δ�ȶ�,�ʵȴ�USB�豸���ٺ���,������ζ���
			s = InitRootDevice();                                         // ��ʼ��USB�豸
			if (s == ERR_SUCCESS)
			{
				// U�̲������̣�USB���߸�λ��U�����ӡ���ȡ�豸������������USB��ַ����ѡ�Ļ�ȡ������������֮�󵽴�˴�����CH549�ӳ���������ɺ�������
				CH549DiskStatus = DISK_USB_ADDR;
				for(i = 0;i != 10;i ++)
				{
					printf("Wait DiskReady\n");
					s = CH549DiskReady();                                 //�ȴ�U��׼����
					if(s == ERR_SUCCESS)
					{
						break;
					}
					else
					{
						printf("%02x\n",(UINT16)s);
					}
					mDelaymS(50);
				}
				if(CH549DiskStatus >= DISK_MOUNTED)
				{
					//�����ļ���ʾ
					printf("Create\n");
					strcpy(mCmdParam.Create.mPathName, "/NEWFILE.TXT");          /* ���ļ���,�ڸ�Ŀ¼��,�����ļ��� */
					s = CH549FileCreate();                                        /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
					mStopIfError(s);
					printf("ByteWrite\n");
					//ʵ��Ӧ���ж�д���ݳ��ȺͶ��建���������Ƿ������������ڻ�������������Ҫ���д��
					i = sprintf(buf,"Note: \xd\xa������������ֽ�Ϊ��λ����U���ļ���д,549����ʾ���ܡ�\xd\xa");  /*��ʾ */
					for(c=0; c<10; c++)
					{
						mCmdParam.ByteWrite.mByteCount = i;                          /* ָ������д����ֽ��� */
						mCmdParam.ByteWrite.mByteBuffer = buf;                       /* ָ�򻺳��� */
						s = CH549ByteWrite();                                       /* ���ֽ�Ϊ��λ���ļ�д������ */
						mStopIfError(s);
						printf("�ɹ�д�� %02X��\n",(UINT16)c);
					}

					printf("Close\n");
					mCmdParam.Close.mUpdateLen = 1;                                  /* �Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ����� */
					i = CH549FileClose();
					mStopIfError(i);
				}
			}
		}
		mDelaymS(100);  // ģ�ⵥƬ����������
		SetUsbSpeed(1);  // Ĭ��Ϊȫ��
	}
}