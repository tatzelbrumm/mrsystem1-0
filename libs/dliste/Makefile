TARGET=libdliste.a
OBJS=dliste_ahead.o dliste_append.o dliste_create.o dliste_del_akt.o dliste_del_first.o dliste_del_last.o dliste_delete.o dliste_destroy.o dliste_finde.o dliste_first.o dliste_init.o dliste_insert.o dliste_is_empty.o dliste_last.o dliste_next.o dliste_prev.o dliste_citer.o dliste_diter.o dliste_iiter.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp dliste.h $(INCLUDE_PATH)

dliste_ahead.o: dliste_ahead.c dliste.h

dliste_append.o: dliste_append.c dliste.h

dliste_create.o: dliste_create.c dliste.h

dliste_del_akt.o: dliste_del_akt.c dliste.h

dliste_del_first.o: dliste_del_first.c dliste.h

dliste_del_last.o: dliste_del_last.c dliste.h

dliste_delete.o: dliste_delete.c dliste.h

dliste_destroy.o: dliste_destroy.c dliste.h

dliste_finde.o: dliste_finde.c dliste.h

dliste_first.o: dliste_first.c dliste.h

dliste_init.o: dliste_init.c dliste.h

dliste_insert.o: dliste_insert.c dliste.h

dliste_is_empty.o: dliste_is_empty.c dliste.h

dliste_last.o: dliste_last.c dliste.h

dliste_next.o: dliste_next.c dliste.h

dliste_prev.o: dliste_prev.c dliste.h

dliste_citer.o: dliste_citer.c dliste.h

dliste_diter.o: dliste_diter.c dliste.h

dliste_iiter.o: dliste_iiter.c dliste.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/dliste.h
	rm -f $(LIB_PATH)/$(TARGET)
