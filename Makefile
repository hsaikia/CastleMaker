# Compiler
CXX          =  g++
CFLAGS       = -g -Wno-write-strings
INCLUDE	     = -Isrc/processor/modeler
# Compilation (add flags as needed)
CXXFLAGS    +=

#Sources, Objects and Executable
EXECUTABLE=builder
SRC_DIR=src/processor/modeler
OBJ_DIR=debug
OBJECT_NAMES:=main.o grammar.tab.o lex.yy.o model_processor.o model.o component.o util.o objprocessor.o
OBJECTS:=$(addprefix $(OBJ_DIR)/, $(OBJECT_NAMES))
# Linking (add flags as needed)
LDFLAGS     += 
LFLAGS       = 
LIBS         = 

# Name your target executables here
all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDE) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS) $(LFLAGS) $(LIBS)
$(OBJ_DIR)/grammar.tab.o:
	bison -d -Wnone src/processor/yyparser/grammar.ypp
	flex src/processor/yyparser/tokenizer.l
	mv grammar.* lex.* $(OBJ_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $(OBJ_DIR)/grammar.tab.cpp -o $@ 
$(OBJ_DIR)/lex.yy.o :
	$(CXX) $(INCLUDE) -c $(OBJ_DIR)/lex.yy.c -o $(OBJ_DIR)/lex.yy.o
$(OBJ_DIR)/util.o: $(SRC_DIR)/util.cpp
	$(CXX) -c $(SRC_DIR)/util.cpp -o $(OBJ_DIR)/util.o
$(OBJ_DIR)/objprocessor.o: $(SRC_DIR)/objprocessor.cpp
	$(CXX) -c $(SRC_DIR)/objprocessor.cpp -o $(OBJ_DIR)/objprocessor.o
$(OBJ_DIR)/model.o: $(SRC_DIR)/model.cpp
	$(CXX) -c $(SRC_DIR)/model.cpp -o $(OBJ_DIR)/model.o
$(OBJ_DIR)/model_processor.o: $(SRC_DIR)/model_processor.cpp
	$(CXX) -c $(SRC_DIR)/model_processor.cpp -o $(OBJ_DIR)/model_processor.o
$(OBJ_DIR)/component.o: $(SRC_DIR)/component.cpp
	$(CXX) -c $(SRC_DIR)/component.cpp -o $(OBJ_DIR)/component.o
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o
	
clean:
	rm -f $(EXECUTABLE) $(OBJ_DIR)/*.o $(OBJ_DIR)/lex.yy.c $(OBJ_DIR)/grammar.tab.*

