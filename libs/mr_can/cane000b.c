/**********************************************************************\
* Dateiname: cane000b.c
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
* Funktionsname: MrCs2EncSysStatus6
*
* Kurzbeschreibung:
* encode function for CAN system command status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : Uid of reciver
* Channel : channel
*
\**********************************************************************/
void MrCs2EncSysStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncSysStatus7
*
* Kurzbeschreibung:
* encode function for CAN system command status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : Uid of reciver
* Channel : channel
* Set     : flag for set
*
\**********************************************************************/
void MrCs2EncSysStatus7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Set)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   CanMsg->Data[6] = Set;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 7);
}

/**********************************************************************\
* Funktionsname: MrCs2EncSysStatus8
*
* Kurzbeschreibung:
* encode function for CAN system command status message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : Uid of reciver
* Channel : channel
* Value   : value
*
\**********************************************************************/
void MrCs2EncSysStatus8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Value)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), Value);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 8);
}
