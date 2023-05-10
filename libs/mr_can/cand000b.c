/**********************************************************************\
* Dateiname: cand000b.c
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
* Funktionsname: MrCs2DecSysStatus6
*
* Kurzbeschreibung:
* decode function for CAN system command system status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : holds the decoded uid
* Channel : holds the decoded channel
*
\**********************************************************************/
void MrCs2DecSysStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
}

/**********************************************************************\
* Funktionsname: MrCs2DecSysStatus7
*
* Kurzbeschreibung:
* decode function for CAN system command system status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : holds the decoded uid
* Channel : holds the decoded channel
* Set     : holds the decoded set flag
*
\**********************************************************************/
void MrCs2DecSysStatus7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Set)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
   *Set = CanMsg->Data[6];
}

/**********************************************************************\
* Funktionsname: MrCs2DecSysStatus8
*
* Kurzbeschreibung:
* decode function for CAN system command system status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : holds the decoded uid
* Channel : holds the decoded channel
* Value   : holds the decoded value
*
\**********************************************************************/
void MrCs2DecSysStatus8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Value)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
   *Value = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
