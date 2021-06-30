/********************************** (C) COPYRIGHT *******************************
* File Name          : FLASH.H
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/05
* Description        : CH545 Flash
*******************************************************************************/
#ifndef	__DATAFLASH_H__
#define __DATAFLASH_H__

#include "Debug.H"
/*********************** Flash�ռ仮�� *****************************
* 0xF400 ���� 0xFFFD  Code Flash(BOOT����3K) 
* 0xF000 ���� 0xF3FF  Data Flash��1K��
* 0x0000 ���� 0xEFFF  Code Flash���û�������60K��
******************************************************************/
/* Code Flash��Data Flash���� */
UINT8 FlashErasePage(UINT16 Addr);
UINT8 FlashProgByte(UINT16 Addr, UINT8 Data);
UINT8 FlashProgPage(UINT16 Addr, PUINT8X Buf, UINT8 len);
UINT8 FlashReadBuf(UINT16 Addr, PUINT8 buf, UINT16 len);

/* OTP����ReadOnly������ */
UINT8  FlashProgOTPbyte(UINT8 Addr, UINT8 Data);
UINT32 FlashReadOTPword(UINT8 Addr);

#endif