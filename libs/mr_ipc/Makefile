TARGET=libmr_ipc.a
OBJS=create.o destroy.o init.o exit.o connect.o server.o accept.o send.o receive.o decode.o encode_can.o decode_can.o cJSON/cJSON.o \
    cmd_get_cmd.o cmd_set_null.o cmd_set_run.o cmd_get_run.o \
    cmd_set_locomotive_dir.o cmd_set_locomotive_speed.o cmd_set_locomotive_fkt.o \
    cmd_get_locomotive_dir.o cmd_get_locomotive_speed.o cmd_get_locomotive_fkt.o \
    cmd_set_acc_pos.o cmd_get_acc_pos.o cmd_set_member.o cmd_get_member.o \
    cmd_set_get_member.o cmd_set_get_locname.o cmd_get_get_locname.o \
    cmd_set_get_locinfo.o cmd_get_get_locinfo.o \
    cmd_set_cfg_header.o cmd_get_cfg_header.o cmd_set_cfg_data.o cmd_get_cfg_data.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp mr_ipc.h $(INCLUDE_PATH)

create.o: create.c mr_ipc.h

destroy.o: destroy.c mr_ipc.h

init.o: init.c mr_ipc.h

exit.o: exit.c mr_ipc.h

connect.o: connect.c mr_ipc.h

server.o: server.c mr_ipc.h

accept.o: accept.c mr_ipc.h

send.o: send.c mr_ipc.h

receive.o: receive.c mr_ipc.h

decode.o: decode.c mr_ipc.h

encode_can.o: encode_can.c mr_ipc.h

decode_can.o: decode_can.c mr_ipc.h

cmd_set_null.o: cmd_set_null.c mr_ipc.h cJSON/cJSON.h

cmd_set_locomotive_dir.o: cmd_set_locomotive_dir.c mr_ipc.h cJSON/cJSON.h

cmd_set_locomotive_speed.o: cmd_set_locomotive_speed.c mr_ipc.h cJSON/cJSON.h

cmd_set_locomotive_fkt.o: cmd_set_locomotive_fkt.c mr_ipc.h cJSON/cJSON.h

cmd_set_acc_pos.o: cmd_set_acc_pos.c mr_ipc.h cJSON/cJSON.h

cmd_set_get_member.o: cmd_set_get_member.c mr_ipc.h cJSON/cJSON.h

cmd_set_get_locname.o: cmd_set_get_locname.c mr_ipc.h cJSON/cJSON.h

cmd_set_get_locinfo.o: cmd_set_get_locinfo.c mr_ipc.h cJSON/cJSON.h

cmd_set_member.o: cmd_set_member.c mr_ipc.h cJSON/cJSON.h

cmd_set_cfg_header.o: cmd_set_cfg_header.c mr_ipc.h cJSON/cJSON.h

cmd_set_cfg_data.o: cmd_set_cfg_data.c mr_ipc.h cJSON/cJSON.h

cmd_get_cmd.o: cmd_get_cmd.c mr_ipc.h cJSON/cJSON.h

cmd_get_locomotive_dir.o: cmd_get_locomotive_dir.c mr_ipc.h cJSON/cJSON.h

cmd_get_locomotive_speed.o: cmd_get_locomotive_speed.c mr_ipc.h cJSON/cJSON.h

cmd_get_locomotive_fkt.o: cmd_get_locomotive_fkt.c mr_ipc.h cJSON/cJSON.h

cmd_get_acc_pos.o: cmd_get_acc_pos.c mr_ipc.h cJSON/cJSON.h

cmd_get_member.o: cmd_get_member.c mr_ipc.h cJSON/cJSON.h

cmd_get_get_locname.o: cmd_get_get_locname.c mr_ipc.h cJSON/cJSON.h

cmd_get_get_locinfo.o: cmd_get_get_locinfo.c mr_ipc.h cJSON/cJSON.h

cmd_get_cfg_header.o: cmd_get_cfg_header.c mr_ipc.h cJSON/cJSON.h

cmd_get_cfg_data.o: cmd_get_cfg_data.c mr_ipc.h cJSON/cJSON.h

cJSON/cJSON.o: cJSON/cJSON.c cJSON/cJSON.h
	cd cJSON ; $(CC) $(CFLAGS) -c cJSON.c

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/mr_ipc.h
	rm -f $(LIB_PATH)/$(TARGET)
