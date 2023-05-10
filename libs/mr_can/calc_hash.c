/**********************************************************************\
* Dateiname: enceth.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode UDP frame from CAN message
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <strings.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2CalcHash
*
* Kurzbeschreibung:
* calculates the hash and set the hash field of CanMsg
*
* Parameter:
* CanMsg : CAN message
* Uid    : UID from which hash should be calculated
*
\**********************************************************************/
void MrCs2CalcHash(MrCs2CanDataType *CanMsg, unsigned long Uid)
{  unsigned CanHash;

   CanHash = (Uid & 0x0000ffff) ^ (Uid >> 16); /* xor uid low and high word */
   CanHash = htons(CanHash);                   /* swap bytes if needed */
   CanHash &= ~MR_CS2_MASK_HASH_MAGIC;         /* mask bits 7-9 */
   CanHash |= MR_CS2_HASH_MAGIC;               /* set bits 8-9 */
   MrCs2SetHash(CanMsg, CanHash);
}
