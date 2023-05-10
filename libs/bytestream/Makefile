TARGET=libbytestream.a
OBJS=get_long.o get_int.o set_long.o set_int.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp bytestream.h $(INCLUDE_PATH)

get_long.o: get_long.c bytestream.h

get_int.o: get_int.c bytestream.h

set_long.o: get_long.c bytestream.h

set_int.o: get_int.c bytestream.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/bytestream.h
	rm -f $(LIB_PATH)/$(TARGET)
