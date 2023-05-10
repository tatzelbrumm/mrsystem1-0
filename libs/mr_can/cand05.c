/**********************************************************************\
* Dateiname: cand05.c
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
* Funktionsname: MrCs2DecDirection4
*
* Kurzbeschreibung:
* decode function for CAN loc direction command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : holds the decoded loc id
*
\**********************************************************************/
void MrCs2DecDirection4(MrCs2CanDataType *CanMsg, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
}

/**********************************************************************\
* Funktionsname: MrCs2DecDirection5
*
* Kurzbeschreibung:
* decode function for CAN loc direction command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* LocId     : holds the decoded loc id
* Direction : holds the decoded direction
*
\**********************************************************************/
void MrCs2DecDirection5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Direction)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Direction = CanMsg->Data[4];
}
