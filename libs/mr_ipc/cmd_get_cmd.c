#include <stddef.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

MrIpcCommandValue MrIpcCmdGetCmd(MrIpcCmdType *Data)
{  int ReturnValue;

   ReturnValue = MrIpcCmdNull;
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      if (cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_CMD)->valuestring != NULL)
      {
         if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                        MR_IPC_JSON_OBJECT_CMD)->valuestring,
                    MR_IPC_JSON_CMD_NULL) == 0)
            ReturnValue = MrIpcCmdNull;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                        MR_IPC_JSON_OBJECT_CMD)->valuestring,
                    MR_IPC_JSON_CMD_RUN) == 0)
            ReturnValue = MrIpcCmdRun;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_LOCOMOTIVE_DIR) == 0)
            ReturnValue = MrIpcCmdLocomotiveDirection;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_LOCOMOTIVE_SPEED) == 0)
            ReturnValue = MrIpcCmdLocomotiveSpeed;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_LOCOMOTIVE_FKT) == 0)
            ReturnValue = MrIpcCmdLocomotiveFunction;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_ACC_SWITCH) == 0)
            ReturnValue = MrIpcCmdAccSwitch;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_MEMBER) == 0)
            ReturnValue = MrIpcCmdMember;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_REQUEST_LOC_NAME) == 0)
            ReturnValue = MrIpcCmdRequestLocName;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_REQUEST_LOC_INFO) == 0)
            ReturnValue = MrIpcCmdRequestLocInfo;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_REQUEST_MEMBER) == 0)
            ReturnValue = MrIpcCmdRequestMember;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_CFG_HEADER) == 0)
            ReturnValue = MrIpcCmdCfgHeader;
         else if (strcmp(cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data),
                                             MR_IPC_JSON_OBJECT_CMD)->valuestring,
                         MR_IPC_JSON_CMD_CFG_DATA) == 0)
            ReturnValue = MrIpcCmdCfgData;
      }
   }
   return ReturnValue;
}
