/**********************************************************************\
* Dateiname: cand30.c
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
* Funktionsname: MrCs2DecAutomatic6
*
* Kurzbeschreibung:
* decode function for CAN automatic command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* DeviceId : holds the decoded device id
* Func     : holds the decoded function
* Status   : holds the decoded status
* Param    : holds the decoded parameter
*
\**********************************************************************/
void MrCs2DecAutomatic6(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        int *Status, int *Param)
{
   *DeviceId = GetIntFromByteArray((char *)CanMsg->Data);
   *Func = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *Status = CanMsg->Data[4];
   *Param = CanMsg->Data[5];
}

/**********************************************************************\
* Funktionsname: MrCs2DecAutomatic8
*
* Kurzbeschreibung:
* decode function for CAN automatic command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* DeviceId : holds the decoded device id
* Func     : holds the decoded function
* LocId    : holds the decoded loc id
*
\**********************************************************************/
void MrCs2DecAutomatic8(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        unsigned long *LocId)
{
   *DeviceId = GetIntFromByteArray((char *)CanMsg->Data);
   *Func = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *LocId = GetLongFromByteArray((char *)&(CanMsg->Data[4]));
}
