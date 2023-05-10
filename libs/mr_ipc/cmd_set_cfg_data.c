#include <stddef.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetCfgData(MrIpcCmdType *Data, char *Buf)
{  cJSON *root;
   char BufAsString[9]; /* 8 data bytes + null */

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_CFG_DATA));
   memcpy(BufAsString, Buf, 8);
   BufAsString[8] = '\0';
   cJSON_AddStringToObject(root, MR_IPC_JSON_OBJECT_CFG_DATA, BufAsString);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
