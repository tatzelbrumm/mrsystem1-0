/**********************************************************************\
* Dateiname: main.c
* Autor    : Michael Bernstein
* Projekt  : client_log
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
#include "log.h"

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
   printf("mrlog V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] -a <addr> -p <port> ) | -?\n", name);
   puts("-a - ip address of drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-f - dont fork to go in background");
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
* 2 : invalid command line parameters
* 3 : program can not go to background
* 4 : main loop terminates
*
\**********************************************************************/
int main(int argc, char *argv[])
{  int c, Port;
   BOOL Verbose, DoFork;
   LogStruct *Log;
   pid_t ChildPid;
   char *IpAddr;
   time_t Now;

   /* 1) get comand line parameter */
   Verbose = FALSE;
   DoFork = TRUE;
   Port = -1;
   IpAddr = (char *)NULL;
   do {
      c = getopt(argc, argv, "a:p:fs:v?");
      switch (c)
      {
         case 'a':
            /* Note: memory is given to drehscheibe as new owner! */
            IpAddr = strdup(optarg);
            break;
         case 'p':
            Port = atoi(optarg);
            break;
         case 'f':
            DoFork = FALSE;
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
            Log = LogCreate();
            if (Log != (LogStruct *)NULL)
            {
               LogInit(Log, Verbose, IpAddr, Port);
               LogRun(Log);
               LogExit(Log);
               LogDestroy(Log);
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
         Log = LogCreate();
         if (Log != (LogStruct *)NULL)
         {
            LogInit(Log, Verbose, IpAddr, Port);
            LogRun(Log);
            LogExit(Log);
            LogDestroy(Log);
            /* if we are here, something went wrong in main loop */
            /* or we got a signal to stop */
         }
         return(4);
      }
   }
}
