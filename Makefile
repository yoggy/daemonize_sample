TARGET = all 

FILES = main_daemonize

OBJECTS = $(addsuffix .o,$(FILES))

CFLAGS   = -Wall -g
INCLUDES = 
LIBS     = 

all: daemonize_sample

daemonize_sample: $(OBJECTS)
	gcc $(LIBS) $^ -o $@

$(OBJECTS): %.o : %.c 
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

test: daemonize_sample
	./daemonize_sample start
	sleep 3
	./daemonize_sample stop

clean:
	rm -rf $(OBJECTS)
	rm -rf daemonize_sample
