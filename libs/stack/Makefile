TARGET=libstack.a
OBJS=stack_create.o stack_destroy.o stack_init.o stack_is_empty.o stack_pop.o stack_push.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp stack.h $(INCLUDE_PATH)

stack_create.o: stack_create.c stack.h

stack_destroy.o: stack_destroy.c stack.h

stack_init.o: stack_init.c stack.h

stack_is_empty.o: stack_is_empty.c stack.h

stack_pop.o: stack_pop.c stack.h

stack_push.o: stack_push.c stack.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/stack.h
	rm -f $(LIB_PATH)/$(TARGET)
