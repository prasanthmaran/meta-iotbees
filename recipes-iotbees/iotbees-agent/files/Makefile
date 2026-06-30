CC ?= gcc
CFLAGS += -Wall -O2
LDLIBS += -lmodbus -lpaho-mqtt3c

iotbees-agent: iotbees-agent.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f iotbees-agent

.PHONY: clean
