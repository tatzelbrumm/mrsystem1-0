/**********************************************************************\
* Dateiname: cane20.c
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
* Funktionsname: MrCs2EncConfigQuery0
*
* Kurzbeschreibung:
* encode function for CAN config querry command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
*
\**********************************************************************/
void MrCs2EncConfigQuery0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CONFIG_QUERY);
   MrCs2SetDlc(CanMsg, 0);
}

/**********************************************************************\
* Funktionsname: MrCs2EncConfigQuery
*
* Kurzbeschreibung:
* encode function for CAN config querry command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* FileName : filename to request
*
\**********************************************************************/
void MrCs2EncConfigQuery(MrCs2CanDataType *CanMsg, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = '\0';
   for (i = 0; i < strlen(FileName); i++)
      CanMsg->Data[i] = FileName[i];
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CONFIG_QUERY);
   MrCs2SetDlc(CanMsg, 8);
}
