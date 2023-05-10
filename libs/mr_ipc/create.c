#include <stdlib.h>
#include "mr_ipc.h"

MrIpcCmdType *RpiIpcCreate(void)
{  MrIpcCmdType *NewIpcCmd;

   NewIpcCmd = (MrIpcCmdType *)malloc(sizeof(MrIpcCmdType));
   if (NewIpcCmd != (MrIpcCmdType *)NULL)
   {
      MrIpcSetRendered(NewIpcCmd, (char *)NULL);
      MrIpcSetPrivate(NewIpcCmd, (void *)NULL);
   }
   return NewIpcCmd;
}
