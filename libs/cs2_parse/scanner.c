#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"


ScannerStruct *ScannerCreate(void)
{  ScannerStruct *NewData;

   NewData = (ScannerStruct *)malloc(sizeof(ScannerStruct));
   return(NewData);
}

void ScannerDestroy(ScannerStruct *Data)
{
   free(Data);
}

void ScannerInit(ScannerStruct *Data, char *InputLine)
{
   ScannerSetInputString(Data, InputLine);
   ScannerSetPosInInputString(Data, 0);
   ScannerSetKeyWords(Data, NULL);
}

static char NextChar(ScannerStruct *Data)
{
   if (ScannerGetPosInInputString(Data) <= strlen(ScannerGetInputString(Data)))
   {
      ScannerSetPosInInputString(Data, ScannerGetPosInInputString(Data) + 1);
      return(ScannerGetInputString(Data)[ScannerGetPosInInputString(Data) - 1]);
   }
   else
   {
      return('\0');
   }
}

void UngetLastChar(ScannerStruct *Data)
{
   ScannerSetPosInInputString(Data, ScannerGetPosInInputString(Data) - 1);
}

int ScannerScan(ScannerStruct *Data)
{  char Token[80], Value[80], ActualChar;
   int PosInToken, Ret, i;

   PosInToken = 0;
   /* skip space and tab (whitespace) */
   do {
      ActualChar = NextChar(Data);
   } while ((ActualChar == ' ') || (ActualChar == '\t'));
   /* check, which character we found */
   if (ActualChar == '\0')
      Ret = TOKEN_EOL;
   else if ((ActualChar == '\n') || (ActualChar == '\r'))
   {
      do {
         ActualChar = NextChar(Data);
      } while ((ActualChar == '\n') || (ActualChar == '\r'));
      UngetLastChar(Data);
      Ret = TOKEN_NL;
   }
   else if (ActualChar == '[')
      Ret = TOKEN_START_PARAGRAPH;
   else if (ActualChar == ']')
      Ret = TOKEN_END_PARAGRAPH;
   else if (ActualChar == '.')
      Ret = TOKEN_LEVEL;
   else if (ActualChar == '=')
      Ret = TOKEN_EQUAL;
   else if (isalpha(ActualChar))
   {
      /* evaluate keyword */
      do {
         Value[PosInToken] = ActualChar;
         Token[PosInToken++] = tolower(ActualChar);
         ActualChar = NextChar(Data);
      } while (isalnum(ActualChar));
      Token[PosInToken++] = '\0';
      UngetLastChar(Data);
      ScannerSetValueString(Data, Value);
      if (ScannerGetKeyWords(Data) != NULL)
      {
         Ret = TOKEN_VALUE;
         i = 0;
         while (ScannerKeyWordsGetKeyWord(ScannerGetKeyWordsIdx(Data, i)) != NULL)
         {
            if (strcmp(Token, ScannerKeyWordsGetKeyWord(ScannerGetKeyWordsIdx(Data, i))) == 0)
            {
               Ret = ScannerKeyWordsGetToken(ScannerGetKeyWordsIdx(Data, i));
            }
            i++;
         }
      }
      else
      {
         Ret = TOKEN_VALUE;
      }
   }
   else if (isdigit(ActualChar))
   {  char *EndPtr;

      do {
         Token[PosInToken++] = ActualChar;
         ActualChar = NextChar(Data);
      } while (isalnum(ActualChar));
      Token[PosInToken++] = '\0';
      UngetLastChar(Data);
      ScannerSetValueInt(Data, strtol(Token, &EndPtr, 0));
      Ret = TOKEN_INTEGER;
   }
   else
   {
      Ret = TOKEN_ERROR;
   }
   return(Ret);
}

int ScannerScanEol(ScannerStruct *Data)
{  char Token[80], ActualChar;
   int PosInToken;

   PosInToken = 0;
   ActualChar = NextChar(Data);
   while ((ActualChar != '\n') && (ActualChar != '\r'))
   {
      Token[PosInToken++] = ActualChar;
      ActualChar = NextChar(Data);
   }
   Token[PosInToken++] = '\0';
   ScannerSetValueString(Data, Token);
   UngetLastChar(Data);
   return(TOKEN_VALUE);
}
