/**********************************************************************\
* Dateiname: cand06.c
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
* Funktionsname: MrCs2DecFunction5
*
* Kurzbeschreibung:
* decode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* Function : holds the decoded function
*
\**********************************************************************/
void MrCs2DecFunction5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Function = CanMsg->Data[4];
}

/**********************************************************************\
* Funktionsname: MrCs2DecFunction6
*
* Kurzbeschreibung:
* decode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* Function : holds the decoded function
* Value    : holds the decoded value
*
\**********************************************************************/
void MrCs2DecFunction6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Function = CanMsg->Data[4];
   *Value = CanMsg->Data[5];
}

/**********************************************************************\
* Funktionsname: MrCs2DecFunction8
*
* Kurzbeschreibung:
* decode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* Function : holds the decoded function
* Value    : holds the decoded value
* FktVal   : holds the decoded function value
*
\**********************************************************************/
void MrCs2DecFunction8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value, int *FktVal)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Function = CanMsg->Data[4];
   *Value = CanMsg->Data[5];
   *FktVal = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
