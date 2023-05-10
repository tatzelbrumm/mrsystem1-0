LIBS=bitmask bytestream bool compare cs2_parse dliste fsm liste menge mr_can mr_ipc stack

.PHONY: $(LIBS)

all: $(LIBS)

bitmask:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

bytestream:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

bool:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

compare:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

cs2_parse:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

dliste: bool compare
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

fsm:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

liste: bool compare
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

menge: bool compare dliste
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_can: bitmask bytestream
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_ipc:
	#if [ ! -f ../archive/cJSONFiles.zp ] ; then wget -P ../archive http://sourceforge.net/projects/cjson/files/latest/download/cJSONFiles.zip ; fi
	if [ ! -d mr_ipc/cJSON ] ; then unzip ../archive/cJSONFiles.zip -d mr_ipc ; fi
	if [ -d mr_ipc/__MACOSX ] ; then rm -r mr_ipc/__MACOSX ; fi
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

stack: bool compare liste
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

clean:
	make -C bitmask clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C bytestream clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C bool clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C compare clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C cs2_parse clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C dliste clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C fsm clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C liste clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C menge clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C mr_can clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	#if [ -f ../archive/cJSONFiles.zp ] ; then rm ../archive/cJSONFiles.zip ; fi
	make -C mr_ipc clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C stack clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
