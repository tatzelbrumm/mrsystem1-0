/**********************************************************************\
* Dateiname: cane07.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2EncReadConfig6
*
* Kurzbeschreibung:
* encode function for CAN read config command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* LocId   : loc if for receiver
* CvIndex : cv index for receiver
*
\**********************************************************************/
void MrCs2EncReadConfig6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), CvIndex);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_READ_CONFIG);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncReadConfig7
*
* Kurzbeschreibung:
* encode function for CAN read config command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* LocId   : loc id for receiver
* CvIndex : cv index for receiver
* Param   : param for receiver
*
\**********************************************************************/
void MrCs2EncReadConfig7(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Param)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), CvIndex);
   CanMsg->Data[6] = Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_READ_CONFIG);
   MrCs2SetDlc(CanMsg, 7);
}
