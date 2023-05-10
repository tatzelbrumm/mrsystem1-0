/**********************************************************************\
* Dateiname: cand0007.c
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
* Funktionsname: MrCs2DecSysMfxFastRead
*
* Kurzbeschreibung:
* decode function for CAN system command mfx fast read message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* MfxSid : holds the decoded mfx sid
*
\**********************************************************************/
void MrCs2DecSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *MfxSid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *MfxSid = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
