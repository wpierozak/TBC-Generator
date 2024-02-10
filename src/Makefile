COMP = g++
LOADER = g++

ROOT = /home/wiktor/Desktop/engineering_thesis

INC = $(ROOT)/inc
SRC = $(ROOT)/src
OBJ = $(ROOT)/obj
BIN = $(ROOT)/bin

datatypes: $(INC)/CM_datatypes.hpp $(SRC)/CM_datatypes.cpp
	$(COMP) -c CM_datatypes.cpp -I $(INC) -o $(OBJ)/CM_datatypes.o

xmlparser: datatypes $(INC)/CM_xmlparser.hpp $(SRC)/CM_xmlparser.cpp
	$(COMP) -c CM_xmlparser.cpp -I $(INC) -o $(OBJ)/CM_xmlparser.o
