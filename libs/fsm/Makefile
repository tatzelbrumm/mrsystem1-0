TARGET=libfsm.a
OBJS=fsm_create.o fsm_destroy.o fsm_init.o fsm_exit.o fsm_do.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp fsm.h $(INCLUDE_PATH)

fsm_create.o: fsm_create.c fsm.h

fsm_destroy.o: fsm_destroy.c fsm.h

fsm_init.o: fsm_init.c fsm.h

fsm_exit.o: fsm_exit.c fsm.h

fsm_do.o: fsm_do.c fsm.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/fsm.h
	rm -f $(LIB_PATH)/$(TARGET)
