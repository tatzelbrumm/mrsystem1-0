TARGET=libmenge.a
OBJS=menge_add.o menge_create.o menge_destroy.o menge_first.o menge_init.o menge_next.o menge_remove.o menge_citer.o menge_iiter.o menge_diter.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp menge.h $(INCLUDE_PATH)

menge_add.o: menge_add.c menge.h

menge_create.o: menge_create.c menge.h

menge_destroy.o: menge_destroy.c menge.h

menge_first.o: menge_first.c menge.h

menge_init.o: menge_init.c menge.h

menge_next.o: menge_next.c menge.h

menge_remove.o: menge_remove.c menge.h

menge_citer.o: menge_citer.c menge.h

menge_iiter.o: menge_iiter.c menge.h

menge_diter.o: menge_diter.c menge.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/menge.h
	rm -f $(LIB_PATH)/$(TARGET)
