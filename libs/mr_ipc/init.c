#include <stddef.h>
#include "mr_ipc.h"

void MrIpcInit(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetRendered(Data, (char *)NULL);
      MrIpcSetPrivate(Data, (void *)NULL);
   }
}
