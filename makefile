CC=gcc
deps=lexer.o grammarRead.o parser.o


all: programs

programs: $(deps)
	$(CC) $(deps) -o stage1exe


%.o : %.c
	$(CC) -c $^	