CC=gcc
deps=lexer.o grammarRead.o ntree.o ast.o symbolTable.o typeChecker.o intermediateCode.o parser.o


all: programs

programs: $(deps)
	$(CC) $(deps) -o stage1exe


%.o : %.c
	$(CC) -c $^	