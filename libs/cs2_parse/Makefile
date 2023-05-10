TARGET=libcs2_parse.a
OBJS=parser.o scanner.o write_cs2.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp scanner.h $(INCLUDE_PATH)
	cp parser.h $(INCLUDE_PATH)
	cp write_cs2.h $(INCLUDE_PATH)
	cp cs2_types.h $(INCLUDE_PATH)

mvswitch.o: mvswitch.c parser.h scanner.h

parser.o: parser.c parser.h scanner.h

scanner.o: scanner.c scanner.h

write_cs2.o: write_cs2.c write_cs2.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/scanner.h
	rm -f $(INCLUDE_PATH)/parser.h
	rm -f $(INCLUDE_PATH)/write_cs2.h
	rm -f $(INCLUDE_PATH)/cs2_types.h
	rm -f $(LIB_PATH)/$(TARGET)
