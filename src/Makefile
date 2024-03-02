COMP = g++
LOADER = g++

ROOT = /home/wiktor/Desktop/engineering_thesis

INC = $(ROOT)/inc
SRC = $(ROOT)/src
OBJ = $(ROOT)/obj
BIN = $(ROOT)/bin
LIB = $(ROOT)/lib

DEBUG = -g
OPT = -O2
OMP = -fopenmp 

datatypes: $(INC)/CM_datatypes.hpp $(SRC)/CM_datatypes.cpp
	$(COMP) -c -I$(INC) CM_datatypes.cpp -o $(OBJ)/CM_datatypes.o

xmlparser: datatypes $(INC)/CM_xmlparser.hpp $(SRC)/CM_xmlparser.cpp
	$(COMP) -c -I$(INC) CM_xmlparser.cpp -o $(OBJ)/CM_xmlparser.o

generation: datatypes $(INC)/CM_generation.hpp $(SRC)/CM_generation.cpp
	$(COMP) -c -I$(INC) CM_generation.cpp -o $(OBJ)/CM_generation.o

omp_generation: generation $(INC)/CM_generationOMP.hpp $(SRC)/CM_generationOMP.cpp
	$(COMP) -c $(OMP) -I$(INC) -g -Wall CM_generationOMP.cpp -o $(OBJ)/CM_generationOMP.o

postproc: datatypes $(INC)/CM_postproc.hpp $(SRC)/CM_postproc.cpp
	$(COMP) -c -I$(INC) CM_postproc.cpp -o $(OBJ)/CM_postproc.o

bmp: $(INC)/BMP/libbmp.h $(SRC)/BMP/EasyBMP.cpp
	$(COMP) -c -I$(INC)/BMP BMP/EasyBMP.cpp -o $(OBJ)/bmp.o

ui:
	$(COMP) -c -I$(INC) $(SRC)/CM_ui.cpp -o $(OBJ)/CM_ui.o

cmlib: datatypes xmlparser generation omp_generation postproc bmp ui
	ar rcs $(LIB)/cmlib.a $(OBJ)/*.o

clean:
	rm $(OBJ)/*.o
	
implementation_omp: $(LIB)/cmlib.a 
	$(LOADER) -I$(INC) $(OMP) $(DEBUG) generator_omp.cpp $(LIB)/cmlib.a -o $(BIN)/generator_omp

clmstructure: $(LIB)/cmlib.a 
	$(LOADER) -I$(INC) $(OMP) clmstructure.cpp $(LIB)/cmlib.a -o $(BIN)/clmstructure

2Drun:
	$(BIN)/generator_omp ../xml/test_config.xml

3Drun:
	$(BIN)/generator_omp ../xml/test_config_3D.xml