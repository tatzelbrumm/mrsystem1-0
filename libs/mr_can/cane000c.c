/**********************************************************************\
* Dateiname: cane000c.c
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
* Funktionsname: MrCs2EncSysSystemId5
*
* Kurzbeschreibung:
* encode function for CAN system command system id message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
*
\**********************************************************************/
void MrCs2EncSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long Uid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncSysSystemId7
*
* Kurzbeschreibung:
* encode function for CAN system command system id message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
* Id     : id
*
\**********************************************************************/
void MrCs2EncSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Id)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[5]), Id);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID);
   MrCs2SetDlc(CanMsg, 7);
}
