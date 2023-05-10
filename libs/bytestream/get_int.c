/**********************************************************************\
* Dateiname: get_int.c
* Autor    : Michael Bernstein
* Projekt  : bytestream
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: read int from array of char
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
* read a int from a array of bytes in big endian
*
* Parameter:
* Data : array of bytes, from which should read
*
\**********************************************************************/
unsigned int GetIntFromByteArray(char *Data)
{
   return ((unsigned int)Data[0] << SHIFT_1_BYTE) |
          ((unsigned int)Data[1] << SHIFT_0_BYTE);
}
