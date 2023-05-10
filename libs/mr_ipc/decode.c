#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcDecode(MrIpcCmdType *Data)
{
   if ((Data != (MrIpcCmdType *)NULL) &&
       (MrIpcGetRendered(Data) != (char *)NULL))
   {
      MrIpcSetPrivate(Data, cJSON_Parse(MrIpcGetRendered(Data)));
   }
}
