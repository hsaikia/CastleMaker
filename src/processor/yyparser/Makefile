OBJ_DIR=../debug

default:
	bison -d grammar.ypp
	flex tokenizer.l
	g++ -c grammar.tab.cpp lex.yy.c -Wno-write-strings
	mv grammar.tab.o $(OBJ_DIR)
clean:
	rm -rf *.output *.tab.* lex.yy.c
