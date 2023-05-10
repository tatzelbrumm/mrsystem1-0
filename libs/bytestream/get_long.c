/**********************************************************************\
* Dateiname: get_long.c
* Autor    : Michael Bernstein
* Projekt  : bytestream
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: read long from array of char
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <bitmask.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "bytestream.h"

/**********************************************************************\
* Funktionsname: GetIntFromByteArray
*
* Kurzbeschreibung:
* read a long from a array of bytes in big endian
*
* Parameter:
* Data : array of bytes, from which should read
*
\**********************************************************************/
unsigned long GetLongFromByteArray(char *Data)
{
   return ((unsigned long)Data[0] << SHIFT_3_BYTE) |
          ((unsigned long)Data[1] << SHIFT_2_BYTE) |
          ((unsigned long)Data[2] << SHIFT_1_BYTE) |
          ((unsigned long)Data[3] << SHIFT_0_BYTE);
}
