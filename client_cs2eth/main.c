/**********************************************************************\
* Dateiname: main.c
* Autor    : Michael Bernstein
* Projekt  : client_cs2eth
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: main entry point and help of this program
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <boolean.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "cs2eth.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SOFTWARE_VERSION "1.03"

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
   printf("mrcs2eth V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] -a <addr> -p <port> -b <baddr>)", name);
#ifdef TRACE
   printf(" -t");
#endif
   puts(" | -?\n");
   puts("-a - ip address of drehscheibe");
   puts("-b - send udp as broadcast");
   puts("-p - port of drehscheibe");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
#ifdef TRACE
   puts("-t - trace can messages from udp");
#endif
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
* 2 : invalid command line parameters
* 3 : program can not go to background
* 4 : main loop terminates
*
\**********************************************************************/
int main(int argc, char *argv[])
{  int c, Port;
   BOOL Verbose, DoFork, DoTrace, SendBc;
   Cs2ethStruct *Cs2eth;
   pid_t ChildPid;
   char *IpAddr, *BcAddr;
   time_t Now;

   /* 1) get comand line parameter */
   Verbose = FALSE;
   DoFork = TRUE;
   SendBc = FALSE;
#ifdef TRACE
   DoTrace = FALSE;
#endif
   Port = -1;
   IpAddr = (char *)NULL;
   BcAddr = (char *)NULL;
   do {
#ifdef TRACE
      c = getopt(argc, argv, "a:b:p:ftv?");
#else
      c = getopt(argc, argv, "a:b:p:fv?");
#endif
      switch (c)
      {
         case 'a':
            /* Note: memory is given to drehscheibe as new owner! */
            IpAddr = strdup(optarg);
            break;
         case 'b':
            SendBc = TRUE;
            BcAddr = strdup(optarg);
            break;
         case 'p':
            Port = atoi(optarg);
            break;
         case 'f':
            DoFork = FALSE;
            break;
#ifdef TRACE
         case 't':
            DoTrace = TRUE;
            break;
#endif
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
      return(2);
   }
   else
   {
      /* 3) start or put into background */
      if (DoFork)
      {
         ChildPid = fork();
         if (ChildPid == -1)
         {
            /* error in fork */
            if (Verbose)
               puts("ERROR: can not go to backgound");
            return(3);
         }
         else if (ChildPid == 0)
         {
            /* we are child */
            if (Verbose)
               puts("child running");
            Cs2eth = Cs2ethCreate();
            if (Cs2eth != (Cs2ethStruct *)NULL)
            {
               Cs2ethInit(Cs2eth, Verbose, IpAddr, Port, SendBc, BcAddr
#ifdef TRACE
                          , DoTrace
#endif
                          );
               Cs2ethRun(Cs2eth);
               Cs2ethExit(Cs2eth);
               Cs2ethDestroy(Cs2eth);
               /* if we are here, something went wrong in main loop */
               /* or we got a signal to stop */
            }
            return(4);
         }
         else
         {
            /* we are parent */
            if (Verbose)
               puts("parent terminates");
            signal(SIGCHLD, SIG_IGN); /* ignore if child terminates to avoid a zombie */
            return(0);
         }
      }
      else
      {
         Now = time(NULL);
         if (Verbose)
            printf("start with no fork at %s\n", asctime(localtime(&Now)));
         Cs2eth = Cs2ethCreate();
         if (Cs2eth != (Cs2ethStruct *)NULL)
         {
            Cs2ethInit(Cs2eth, Verbose, IpAddr, Port, SendBc, BcAddr
#ifdef TRACE
                       , DoTrace
#endif
                      );
            Cs2ethRun(Cs2eth);
            Cs2ethExit(Cs2eth);
            Cs2ethDestroy(Cs2eth);
            /* if we are here, something went wrong in main loop */
            /* or we got a signal to stop */
         }
         return(4);
      }
   }
}
