HEADERS = terminal.h headers.h
OBJECTS = terminal.o

default: terminal

%.o: %.c $(HEADERS)
	    gcc -c $< -o $@

program: $(OBJECTS)
	    gcc $(OBJECTS) -o $@

clean:
	    -rm -f $(OBJECTS)
	        -rm -f program
