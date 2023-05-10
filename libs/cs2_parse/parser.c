#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <boolean.h>
#include "scanner.h"
#include "parser.h"


#define PARSER_PARAGRAPH_STRING_LOK        "lok"
#define PARSER_PARAGRAPH_STRING_NUMLOKS    "numloks"
#define PARSER_PARAGRAPH_STRING_LOKOMOTIVE "lokomotive"
#define PARSER_VALUE_STRING_NAME     "name"
#define PARSER_VALUE_STRING_WERT     "wert"
#define PARSER_VALUE_STRING_LOK      "lok"
#define PARSER_VALUE_STRING_UID      "uid"
#define PARSER_VALUE_STRING_ADRESSE  "adresse"
#define PARSER_VALUE_STRING_TYP      "typ"
#define PARSER_VALUE_STRING_MFXUID   "mfxuid"
#define PARSER_VALUE_STRING_SYMBOL   "symbol"
#define PARSER_VALUE_STRING_AV       "av"
#define PARSER_VALUE_STRING_BV       "bv"
#define PARSER_VALUE_STRING_VOLUME   "volume"
#define PARSER_VALUE_STRING_VELOCITY "velocity"
#define PARSER_VALUE_STRING_RICHTUNG "richtung"
#define PARSER_VALUE_STRING_VMAX     "vmax"
#define PARSER_VALUE_STRING_VMIN     "vmin"
#define PARSER_VALUE_STRING_FKT      "fkt"
#define PARSER_VALUE_STRING_DAUER    "dauer"


#define PARSER_TOKEN_KEYWORD_LOK        0x0100
#define PARSER_TOKEN_KEYWORD_NUMLOKS    0x0101
#define PARSER_TOKEN_KEYWORD_LOKOMOTIVE 0x0102
#define PARSER_TOKEN_KEYWORD_NAME       0x0103
#define PARSER_TOKEN_KEYWORD_WERT       0x0104
#define PARSER_TOKEN_KEYWORD_UID        0x0105
#define PARSER_TOKEN_KEYWORD_ADRESSE    0x0106
#define PARSER_TOKEN_KEYWORD_TYP        0x0107
#define PARSER_TOKEN_KEYWORD_MFXUID     0x0108
#define PARSER_TOKEN_KEYWORD_SYMBOL     0x0109
#define PARSER_TOKEN_KEYWORD_AV         0x010a
#define PARSER_TOKEN_KEYWORD_BV         0x010b
#define PARSER_TOKEN_KEYWORD_VOLUME     0x010c
#define PARSER_TOKEN_KEYWORD_VELOCITY   0x010d
#define PARSER_TOKEN_KEYWORD_RICHTUNG   0x010f
#define PARSER_TOKEN_KEYWORD_VMAX       0x0110
#define PARSER_TOKEN_KEYWORD_VMIN       0x0111
#define PARSER_TOKEN_KEYWORD_FKT        0x0112
#define PARSER_TOKEN_KEYWORD_DAUER      0x0113


KeyWordEntry LokNameKeywords[] = {
   { PARSER_PARAGRAPH_STRING_LOK,     PARSER_TOKEN_KEYWORD_LOK },
   { PARSER_PARAGRAPH_STRING_NUMLOKS, PARSER_TOKEN_KEYWORD_NUMLOKS },
   { PARSER_VALUE_STRING_NAME,        PARSER_TOKEN_KEYWORD_NAME },
   { PARSER_VALUE_STRING_WERT,        PARSER_TOKEN_KEYWORD_WERT },
   { NULL, -1 }
};
KeyWordEntry LokInfoKeywords[] = {
   { PARSER_PARAGRAPH_STRING_LOKOMOTIVE, PARSER_TOKEN_KEYWORD_LOKOMOTIVE },
   { PARSER_VALUE_STRING_LOK,            PARSER_TOKEN_KEYWORD_LOK },
   { PARSER_VALUE_STRING_NAME,           PARSER_TOKEN_KEYWORD_NAME },
   { PARSER_VALUE_STRING_WERT,           PARSER_TOKEN_KEYWORD_WERT },
   { PARSER_VALUE_STRING_UID,            PARSER_TOKEN_KEYWORD_UID },
   { PARSER_VALUE_STRING_ADRESSE,        PARSER_TOKEN_KEYWORD_ADRESSE },
   { PARSER_VALUE_STRING_TYP,            PARSER_TOKEN_KEYWORD_TYP },
   { PARSER_VALUE_STRING_MFXUID,         PARSER_TOKEN_KEYWORD_MFXUID },
   { PARSER_VALUE_STRING_SYMBOL,         PARSER_TOKEN_KEYWORD_SYMBOL },
   { PARSER_VALUE_STRING_AV,             PARSER_TOKEN_KEYWORD_AV },
   { PARSER_VALUE_STRING_BV,             PARSER_TOKEN_KEYWORD_BV },
   { PARSER_VALUE_STRING_VOLUME,         PARSER_TOKEN_KEYWORD_VOLUME },
   { PARSER_VALUE_STRING_VELOCITY,       PARSER_TOKEN_KEYWORD_VELOCITY },
   { PARSER_VALUE_STRING_RICHTUNG,       PARSER_TOKEN_KEYWORD_RICHTUNG },
   { PARSER_VALUE_STRING_VMAX,           PARSER_TOKEN_KEYWORD_VMAX },
   { PARSER_VALUE_STRING_VMIN,           PARSER_TOKEN_KEYWORD_VMIN },
   { PARSER_VALUE_STRING_FKT,            PARSER_TOKEN_KEYWORD_FKT },
   { PARSER_VALUE_STRING_DAUER,          PARSER_TOKEN_KEYWORD_DAUER },
   { NULL, -1 }
};


ParserStruct *ParserCreate(void)
{  ParserStruct *NewData;

   NewData = (ParserStruct *)malloc(sizeof(ParserStruct));
   if (NewData != NULL)
   {
      ParserSetScanner(NewData, ScannerCreate());
      if (ParserGetScanner(NewData) == (ScannerStruct *)NULL)
      {
         free(NewData);
         NewData = (ParserStruct *)NULL;
      }
   }
   return(NewData);
}

void ParserDestroy(ParserStruct *Data)
{
   ScannerDestroy(ParserGetScanner(Data));
   free(Data);
}

void ParserInit(ParserStruct *Data, int Type, char *InputLine)
{
   ParserSetVerbose(Data, FALSE);
   ScannerInit(ParserGetScanner(Data), InputLine);
   if (Type == PARSER_TYPE_LOKNAMEN)
      ScannerSetKeyWords(ParserGetScanner(Data), LokNameKeywords);
   else if (Type == PARSER_TYPE_LOKINFO)
      ScannerSetKeyWords(ParserGetScanner(Data), LokInfoKeywords);
}

void PrintError(ParserStruct *Data, char *ErrorString)
{  int i;

   if (ParserGetVerbose(Data))
   {
      fprintf(stderr, "\nERROR: %s\n%s\n",
              ErrorString,
              ScannerGetInputString(ParserGetScanner(Data)));
      for (i = 0; i < ScannerGetPosInInputString(ParserGetScanner(Data)); i++)
         fputc(' ', stderr);
      fputs("^\n\n", stderr);
   }
}

static void DoParagraph(ParserStruct *Data)
{  int Token;

   Token = ScannerScan(ParserGetScanner(Data));
   if ((Token == PARSER_TOKEN_KEYWORD_LOK) ||
       (Token == PARSER_TOKEN_KEYWORD_NUMLOKS) ||
       (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE))
   {
      ParserSetName(Data, ScannerGetValueString(ParserGetScanner(Data)));
      if (ParserGetVerbose(Data))
         printf("Parser: paragraph >%s<\n", ParserGetName(Data));
      if (Token == PARSER_TOKEN_KEYWORD_LOK)
         ParserSetSubType(Data, PARSER_PARAGRAPH_LOK);
      else if (PARSER_TOKEN_KEYWORD_NUMLOKS)
         ParserSetSubType(Data, PARSER_PARAGRAPH_NUMLOKS);
      else if (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE)
         ParserSetSubType(Data, PARSER_PARAGRAPH_LOKOMOTIVE);
      Token = ScannerScan(ParserGetScanner(Data));
      switch (Token)
      {
         case TOKEN_END_PARAGRAPH:
            do {
               Token = ScannerScan(ParserGetScanner(Data));
            } while ((Token != TOKEN_NL) && (Token != TOKEN_EOL));
            break;
         default:
            PrintError(Data, "Unexpected token for paragraph!");
            break;
      }
   }
   else
   {
      if (ParserGetVerbose(Data))
         printf("Parser: found %d\n", Token);
   }
}

static void DoValue(ParserStruct *Data, int StartToken)
{  int Token;

   Token = StartToken;
   while (Token == TOKEN_LEVEL)
   {
      if (Token == TOKEN_LEVEL)
      {
         ParserSetLevel(Data, ParserGetLevel(Data) + 1);
      }
      Token = ScannerScan(ParserGetScanner(Data));
   }
   if ((Token == PARSER_TOKEN_KEYWORD_LOK) ||
       (Token == PARSER_TOKEN_KEYWORD_NAME) ||
       (Token == PARSER_TOKEN_KEYWORD_WERT) ||
       (Token == PARSER_TOKEN_KEYWORD_UID) ||
       (Token == PARSER_TOKEN_KEYWORD_ADRESSE) ||
       (Token == PARSER_TOKEN_KEYWORD_TYP) ||
       (Token == PARSER_TOKEN_KEYWORD_MFXUID) ||
       (Token == PARSER_TOKEN_KEYWORD_SYMBOL) ||
       (Token == PARSER_TOKEN_KEYWORD_AV) ||
       (Token == PARSER_TOKEN_KEYWORD_BV) ||
       (Token == PARSER_TOKEN_KEYWORD_VOLUME) ||
       (Token == PARSER_TOKEN_KEYWORD_VELOCITY) ||
       (Token == PARSER_TOKEN_KEYWORD_RICHTUNG) ||
       (Token == PARSER_TOKEN_KEYWORD_VMAX) ||
       (Token == PARSER_TOKEN_KEYWORD_VMIN) ||
       (Token == PARSER_TOKEN_KEYWORD_FKT) ||
       (Token == PARSER_TOKEN_KEYWORD_DAUER))
   {
      ParserSetName(Data, ScannerGetValueString(ParserGetScanner(Data)));
      if (ParserGetVerbose(Data))
         printf("Parser: value name >%s<\n", ParserGetName(Data));
      if (Token == PARSER_TOKEN_KEYWORD_LOK)
         ParserSetSubType(Data, PARSER_VALUE_LOK);
      else if (Token == PARSER_TOKEN_KEYWORD_NAME)
         ParserSetSubType(Data, PARSER_VALUE_NAME);
      else if (Token == PARSER_TOKEN_KEYWORD_WERT)
         ParserSetSubType(Data, PARSER_VALUE_WERT);
      else if (Token == PARSER_TOKEN_KEYWORD_UID)
         ParserSetSubType(Data, PARSER_VALUE_UID);
      else if (Token == PARSER_TOKEN_KEYWORD_ADRESSE)
         ParserSetSubType(Data, PARSER_VALUE_ADRESSE);
      else if (Token == PARSER_TOKEN_KEYWORD_TYP)
         ParserSetSubType(Data, PARSER_VALUE_TYP);
      else if (Token == PARSER_TOKEN_KEYWORD_MFXUID)
         ParserSetSubType(Data, PARSER_VALUE_MFXUID);
      else if (Token == PARSER_TOKEN_KEYWORD_SYMBOL)
         ParserSetSubType(Data, PARSER_VALUE_SYMBOL);
      else if (Token == PARSER_TOKEN_KEYWORD_AV)
         ParserSetSubType(Data, PARSER_VALUE_AV);
      else if (Token == PARSER_TOKEN_KEYWORD_BV)
         ParserSetSubType(Data, PARSER_VALUE_BV);
      else if (Token == PARSER_TOKEN_KEYWORD_VOLUME)
         ParserSetSubType(Data, PARSER_VALUE_VOLUME);
      else if (Token == PARSER_TOKEN_KEYWORD_VELOCITY)
         ParserSetSubType(Data, PARSER_VALUE_VELOCITY);
      else if (Token == PARSER_TOKEN_KEYWORD_RICHTUNG)
         ParserSetSubType(Data, PARSER_VALUE_RICHTUNG);
      else if (Token == PARSER_TOKEN_KEYWORD_VMAX)
         ParserSetSubType(Data, PARSER_VALUE_VMAX);
      else if (Token == PARSER_TOKEN_KEYWORD_VMIN)
         ParserSetSubType(Data, PARSER_VALUE_VMIN);
      else if (Token == PARSER_TOKEN_KEYWORD_FKT)
         ParserSetSubType(Data, PARSER_VALUE_FKT);
      else if (Token == PARSER_TOKEN_KEYWORD_DAUER)
         ParserSetSubType(Data, PARSER_VALUE_DAUER);
      Token = ScannerScan(ParserGetScanner(Data));
      switch (Token)
      {
         case TOKEN_EQUAL:
            if (ParserGetVerbose(Data))
               puts("Parser: found '='");
            Token = ScannerScanEol(ParserGetScanner(Data));
            ParserSetValue(Data, ScannerGetValueString(ParserGetScanner(Data)));
            if (ParserGetVerbose(Data))
               printf("Parser: value >%s<\n", ParserGetValue(Data));
            Token = ScannerScan(ParserGetScanner(Data));
            break;
         case TOKEN_NL:
	 case TOKEN_EOL:
            if (ParserGetVerbose(Data))
               puts("Parser: found eol");
            ParserSetValue(Data, "");
            break;
         default:
            PrintError(Data, "Unexpected token for value!");
            break;
      }
   }
   else
      PrintError(Data, "Unexpected token for name-value!");
}

int ParserParse(ParserStruct *Data)
{  int Token, Ret;

   Token = ScannerScan(ParserGetScanner(Data));
   Ret = PARSER_ERROR;
   switch (Token)
   {
      case TOKEN_EOL:
         if (ParserGetVerbose(Data))
            puts("Parser: EOF reached");
         ParserSetType(Data, PARSER_EOF);
         Ret = PARSER_EOF;
         break;
      case TOKEN_START_PARAGRAPH:
         if (ParserGetVerbose(Data))
            puts("Parser: Paragraph found");
         ParserSetType(Data, PARSER_PARAGRAPH);
         DoParagraph(Data);
         Ret = PARSER_PARAGRAPH;
         break;
      case TOKEN_LEVEL:
         if (ParserGetVerbose(Data))
            puts("Parser: Value found");
         ParserSetLevel(Data, 0);
         ParserSetType(Data, PARSER_VALUE);
         DoValue(Data, Token);
         Ret = PARSER_VALUE;
         break;
      default:
         if (Token >= PARSER_TOKEN_KEYWORD_LOK)
         {
            if (ParserGetVerbose(Data))
               puts("Parser: Value found");
            ParserSetLevel(Data, 0);
            ParserSetType(Data, PARSER_VALUE);
            DoValue(Data, Token);
            Ret = PARSER_VALUE;
         }
	 else
         {
            if (ParserGetVerbose(Data))
               printf("Parser: token 0x%x\n", Token);
            PrintError(Data, "Unexpected character!");
            Ret = PARSER_ERROR;
         }
         break;
   }
   return(Ret);
}
