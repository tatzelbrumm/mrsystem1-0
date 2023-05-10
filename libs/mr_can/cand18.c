/**********************************************************************\
* Dateiname: cand18.c
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
* Funktionsname: MrCs2DecPing0
*
* Kurzbeschreibung:
* decode function for CAN ping command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
*
\**********************************************************************/
void MrCs2DecPing0(MrCs2CanDataType *CanMsg)
{
}

/**********************************************************************\
* Funktionsname: MrCs2DecPing8
*
* Kurzbeschreibung:
* decode function for CAN ping command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* Uid       : holds the decoded uid
* SwVersion : holds the decoded software version
* DeviceId  : holds the decoded device id
*
\**********************************************************************/
void MrCs2DecPing8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                   int *SwVersion, int *DeviceId)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *SwVersion = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *DeviceId = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
