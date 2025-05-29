CC=g++
PROGNAME=bmpisha
CPPFLAGS=-O0 -g3 -Wall --std=c++17

OBJS=main.o bitmap.o

$(PROGNAME) : $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(PROGNAME)

all: $(PROGNAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(PROGNAME)

re: fclean all

main.o : main.cpp bitmap.h
	$(CC) $(CPPFLAGS) -c main.cpp

bitmap.o : bitmap.cpp bitmap.h
	$(CC) $(CPPFLAGS) -c bitmap.cpp

.PHONY: all clean fclean re
