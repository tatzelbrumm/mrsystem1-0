/**********************************************************************\
* Dateiname: cand03.c
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
* Funktionsname: MrCs2DecMfxVerify6
*
* Kurzbeschreibung:
* decode function for CAN mfx verify command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Sid    : holds the decoded sid
*
\**********************************************************************/
void MrCs2DecMfxVerify6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        unsigned int *Sid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Sid = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

/**********************************************************************\
* Funktionsname: MrCs2DecMfxVerify7
*
* Kurzbeschreibung:
* decode function for CAN mfx verify command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Sid    : holds the decoded sid
* Ask    : holds the decoded ask
*
\**********************************************************************/
void MrCs2DecMfxVerify7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        unsigned int *Sid, int *Ask)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Sid = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Ask = CanMsg->Data[6];
}
