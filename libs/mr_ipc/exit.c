#include <stdlib.h>
#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcExit(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      if (MrIpcGetRendered(Data) != (char *)NULL)
         free(MrIpcGetRendered(Data));
      MrIpcSetRendered(Data, (char *)NULL);
      if (MrIpcGetPrivate(Data) != NULL)
         cJSON_Delete((cJSON *)MrIpcGetPrivate(Data));
      MrIpcSetPrivate(Data, NULL);
   }
}
