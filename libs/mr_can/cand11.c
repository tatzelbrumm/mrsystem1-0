/**********************************************************************\
* Dateiname: cand11.c
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
* Funktionsname: MrCs2DecS88Event4
*
* Kurzbeschreibung:
* decode function for CAN S88 event command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Device  : holds the decoded device
* Contact : holds the decoded contact
*
\**********************************************************************/
void MrCs2DecS88Event4(MrCs2CanDataType *CanMsg, int *Device, int *Contact)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
}

/**********************************************************************\
* Funktionsname: MrCs2DecS88Event5
*
* Kurzbeschreibung:
* decode function for CAN S88 event command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Device  : holds the decoded device
* Contact : holds the decoded contact
* Param   : holds the decoded parameter
*
\**********************************************************************/
void MrCs2DecS88Event5(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *Param)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *Param = CanMsg->Data[4];
}

/**********************************************************************\
* Funktionsname: MrCs2DecS88Event8
*
* Kurzbeschreibung:
* decode function for CAN S88 event command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* Device   : holds the decoded device
* Contact  : holds the decoded contact
* StateOld : holds the decoded old state
* StateNew : holds the decoded new state
* Time     : holds the decoded time
*
\**********************************************************************/
void MrCs2DecS88Event8(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *StateOld, int *StateNew, int *Time)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *StateOld = CanMsg->Data[4];
   *StateNew = CanMsg->Data[5];
   *Time = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
