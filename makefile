CC=gcc
deps=lexer.o stack.o grammarRead.o ntree.o ast.o symbolTable.o typeChecker.o intermediateCode.o parser.o codeGeneration.o compiler.o


all: programs

programs: $(deps)
	$(CC) $(deps) -o compiler


%.o : %.c
	$(CC) -c $^	