#include <stdlib.h>
#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcDestroy(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      if (MrIpcGetRendered(Data) != (char *)NULL)
         free(MrIpcGetRendered(Data));
      if (MrIpcGetPrivate(Data) != NULL)
         cJSON_Delete((cJSON *)MrIpcGetPrivate(Data));
      free(Data);
   }
}
