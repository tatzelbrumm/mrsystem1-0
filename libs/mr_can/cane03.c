/**********************************************************************\
* Dateiname: cane03.c
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
* Funktionsname: MrCs2EncMfxVerify6
*
* Kurzbeschreibung:
* encode function for CAN mfx verify command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Sid    : sid for receiver
*
\**********************************************************************/
void MrCs2EncMfxVerify6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Sid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_MFX_VERIFY);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncMfxVerify7
*
* Kurzbeschreibung:
* encode function for CAN mfx verify command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Sid    : sid for receiver
* Ask    : ask for receiver
*
\**********************************************************************/
void MrCs2EncMfxVerify7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid, int Ask)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Sid);
   CanMsg->Data[6] = Ask;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_MFX_VERIFY);
   MrCs2SetDlc(CanMsg, 7);
}
