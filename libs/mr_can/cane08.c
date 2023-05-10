/**********************************************************************\
* Dateiname: cane08.c
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
* Funktionsname: MrCs2EncWriteConfig
*
* Kurzbeschreibung:
* encode function for CAN write config command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* CvIndex  : cv index for receiver
* Value    : value for receiver
* CtrlRslt : ctrl result for receiver
*
\**********************************************************************/
void MrCs2EncWriteConfig(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Value, int CtrlRslt)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), CvIndex);
   CanMsg->Data[6] = Value;
   CanMsg->Data[7] = CtrlRslt;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_WRITE_CONFIG);
   MrCs2SetDlc(CanMsg, 8);
}
