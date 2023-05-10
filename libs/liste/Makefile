TARGET=libliste.a
OBJS=liste_ahead.o liste_append.o liste_create.o liste_del_first.o liste_delete.o liste_destroy.o liste_finde.o liste_first.o liste_is_empty.o liste_init.o liste_insert.o liste_next.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp liste.h $(INCLUDE_PATH)

liste_ahead.o: liste_ahead.c liste.h

liste_append.o: liste_append.c liste.h

liste_create.o: liste_create.c liste.h

liste_del_first.o: liste_del_first.c liste.h

liste_delete.o: liste_delete.c liste.h

liste_destroy.o: liste_destroy.c liste.h

liste_finde.o: liste_finde.c liste.h

liste_first.o: liste_first.c liste.h

liste_is_empty.o: liste_is_empty.c liste.h

liste_init.o: liste_init.c liste.h

liste_insert.o: liste_insert.c liste.h

liste_next.o: liste_next.c liste.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/liste.h
	rm -f $(LIB_PATH)/$(TARGET)
