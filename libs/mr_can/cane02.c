/**********************************************************************\
* Dateiname: cane02.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2EncBind
*
* Kurzbeschreibung:
* encode function for all CAN command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Sid    : sid for receiver
*
\**********************************************************************/
void MrCs2EncBind(MrCs2CanDataType *CanMsg, unsigned long Uid,
                  unsigned int Sid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Sid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_BIND);
   MrCs2SetDlc(CanMsg, 6);
}
