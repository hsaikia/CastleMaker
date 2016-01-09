# Compiler
CXX          =  g++
CFLAGS       = -Wno-write-strings
INCLUDE	     = -I./modeler
# Compilation (add flags as needed)
CXXFLAGS    +=

#Sources, Objects and Executable
LIBRARY=libbuilder.a
SRC_DIR=modeler
OBJ_DIR=debug
OBJECTS:=grammar.tab.o lex.yy.o model_processor.o model.o component.o util.o objprocessor.o

# Name your target executables here
all : $(LIBRARY)

$(LIBRARY) : $(OBJECTS)
	ar r $(LIBRARY) $(OBJECTS)
	@mv *.o lex.yy.c grammar.tab.cpp grammar.tab.hpp $(OBJ_DIR)/
grammar.tab.o:
	bison -d -Wnone yyparser/grammar.ypp
	flex yyparser/tokenizer.l
	$(CXX) $(CFLAGS) $(INCLUDE) -c grammar.tab.cpp 
lex.yy.o :
	$(CXX) $(INCLUDE) -c lex.yy.c
util.o: $(SRC_DIR)/util.cpp
	$(CXX) -c $(SRC_DIR)/util.cpp
objprocessor.o: $(SRC_DIR)/objprocessor.cpp
	$(CXX) -c $(SRC_DIR)/objprocessor.cpp
model.o: $(SRC_DIR)/model.cpp
	$(CXX) -c $(SRC_DIR)/model.cpp
model_processor.o: $(SRC_DIR)/model_processor.cpp
	$(CXX) -c $(SRC_DIR)/model_processor.cpp
component.o: $(SRC_DIR)/component.cpp
	$(CXX) -c $(SRC_DIR)/component.cpp
	
clean:
	rm -f $(LIBRARY) $(OBJ_DIR)/*.o $(OBJ_DIR)/lex.yy.c $(OBJ_DIR)/grammar.tab.*

