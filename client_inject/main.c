/**********************************************************************\
* Dateiname: main.c
* Autor    : Michael Bernstein
* Projekt  : client_inject
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: main entry point and help of this program
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "inject.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SOFTWARE_VERSION "1.01"

/**********************************************************************\
* Funktionsname: usage
*
* Kurzbeschreibung: this function prints a short help about all
*                   available command line parameter
*
* Parameter:
* name : name of this program which is argv[0] of main()
*
\**********************************************************************/
static void usage(char *name)
{
   printf("mrinject V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] -a <addr> -p <port> [-v] ) | -?\n", name);
   puts("-a - ip address of drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-v - verbose");
   puts("-? - this help");
}

/**********************************************************************\
* Funktionsname: main
*
* Kurzbeschreibung: this function is the entry point of this program
*
* Parameter:
* argc : number of command line parameters
* argv : command line parameters
*
* Rueckgabewert:
* 0 : success (parent if programm will fork)
* 2 : can not create inject object
*
\**********************************************************************/
int main(int argc, char *argv[])
{  int c, Port;
   BOOL Verbose;
   InjectStruct *Inject;
   char *IpAddr;
   time_t Now;

   /* 1) get comand line parameter */
   Verbose = FALSE;
   Port = -1;
   IpAddr = (char *)NULL;
   do {
      c = getopt(argc, argv, "a:p:v?");
      switch (c)
      {
         case 'a':
            /* Note: memory is given to drehscheibe as new owner! */
            IpAddr = strdup(optarg);
            break;
         case 'p':
            Port = atoi(optarg);
            break;
         case 'v':
            Verbose = TRUE;
            break;
         case '?':
            usage(argv[0]);
            return(1);
         default:
            break;
      }
   } while (c != -1);

   /* 2) check parameter */
   if ((Port == -1) || (IpAddr == (char *)NULL))
   {
      usage(argv[0]);
      return(1);
   }
   else
   {
      /* 3) run */
      Now = time(NULL);
      if (Verbose)
         printf("start with no fork at %s\n", asctime(localtime(&Now)));
      Inject = InjectCreate();
      if (Inject != (InjectStruct *)NULL)
      {
         InjectInit(Inject, Verbose, IpAddr, Port);
         InjectRun(Inject);
         InjectExit(Inject);
         InjectDestroy(Inject);
         return(0);
      }
      else
      {
         if (Verbose)
            puts("can not create inject module");
         return(2);
      }
   }
}
