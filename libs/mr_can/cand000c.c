/**********************************************************************\
* Dateiname: cand000c.c
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
* Funktionsname: MrCs2DecSysSystemId5
*
* Kurzbeschreibung:
* decode function for CAN system command system id message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
*
\**********************************************************************/
void MrCs2DecSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
}

/**********************************************************************\
* Funktionsname: MrCs2DecSysSystemId7
*
* Kurzbeschreibung:
* decode function for CAN system command system id message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Id     : holds the decoded id
*
\**********************************************************************/
void MrCs2DecSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Id)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Id = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
