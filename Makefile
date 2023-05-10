PROJECTS=client_cs2eth client_inject client_log client_logms2 client_ms1 client_ms2 client_zentrale drehscheibe libs
CFLAGS=
INCLUDE_PATH=include
LIB_PATH=lib
LDFLAGS=

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: $(PROJECTS)

all: $(PROJECTS)

client_cs2eth: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_inject: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_log: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_logms2: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_ms1: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_ms2: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

client_zentrale: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

drehscheibe: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

libs:
	if [ ! -d include ] ; then mkdir include ; fi
	if [ ! -d lib ] ; then mkdir lib ; fi
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

install:
	make -C client_cs2eth install
	make -C client_inject install
	make -C client_log install
	make -C client_logms2 install
	make -C client_ms1 install
	make -C client_ms2 install
	make -C client_zentrale install
	make -C drehscheibe install

clean:
	make -C client_cs2eth clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_inject clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_log clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms1 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms2 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_logms2 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_zentrale clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C drehscheibe clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C libs clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
