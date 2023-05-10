/**********************************************************************\
* Dateiname: cand20.c
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
* Funktionsname: MrCs2DecConfigQuery
*
* Kurzbeschreibung:
* decode function for all CAN command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* FileName : holds the decoded filename as 0 terminated C string
*
\**********************************************************************/
void MrCs2DecConfigQuery(MrCs2CanDataType *CanMsg, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      FileName[i] = CanMsg->Data[i];
   FileName[8] = '\0';
}
