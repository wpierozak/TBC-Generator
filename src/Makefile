COMP = g++
LOADER = g++

ROOT = /home/wiktor/Desktop/engineering_thesis

INC = $(ROOT)/inc
SRC = $(ROOT)/src
OBJ = $(ROOT)/obj
BIN = $(ROOT)/bin

OMP = -fopenmp 

datatypes: $(INC)/CM_datatypes.hpp $(SRC)/CM_datatypes.cpp
	$(COMP) -c CM_datatypes.cpp -I $(INC) -o $(OBJ)/CM_datatypes.o

xmlparser: datatypes $(INC)/CM_xmlparser.hpp $(SRC)/CM_xmlparser.cpp
	$(COMP) -c CM_xmlparser.cpp -I $(INC) -o $(OBJ)/CM_xmlparser.o

generation: datatypes $(INC)/CM_generation.hpp $(SRC)/CM_generation.cpp
	$(COMP) -c CM_generation.cpp -I $(INC) -o $(OBJ)/CM_generation.o

omp_generation: generation $(INC)/CM_generationOMP.hpp $(SRC)/CM_generationOMP.cpp
	$(COMP) -c CM_generationOMP.cpp -I $(INC) -o $(OBJ)/CM_generationOMP.o

postproc: datatypes $(INC)/CM_postproc.hpp $(SRC)/CM_postproc.cpp
	$(COMP) -c CM_postproc.cpp -I $(INC) -o $(OBJ)/CM_postproc.o