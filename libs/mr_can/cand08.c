/**********************************************************************\
* Dateiname: cand08.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: decode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2DecWriteConfig
*
* Kurzbeschreibung:
* decode function for CAN write config command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* CvIndex  : holds the decoded cv index
* Value    : holds the decoded value
* CtrlRslt : holds the decoded control result
*
\**********************************************************************/
void MrCs2DecWriteConfig(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Value, int *CtrlRslt)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Value = CanMsg->Data[6];
   *CtrlRslt = CanMsg->Data[7];
}
