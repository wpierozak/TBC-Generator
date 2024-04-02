COMP = g++
LOADER = g++

DEBUG = -g
OPT = -O2
OMP = -fopenmp 
COMP_FLAGS = -c $(DEBUG)

ROOT = /home/wiktor/Desktop/engineering_thesis

INC = -I$(ROOT)/inc
INC_DIR = $(ROOT)inc
SRC_DIR = $(ROOT)/src
OBJ_DIR = $(ROOT)/obj
BIN_DIR = $(ROOT)/bin
LIB_DIR = $(ROOT)/lib

TEST_SRC_DIR = $(SRC_DIR)/tests
TEST_BIN_DIR = $(BIN_DIR)/tests

$(OBJ_DIR)/CM_datatypes.o: $(SRC_DIR)/CM_datatypes.cpp
	$(COMP) $(COMP_FLAGS) $(INC) CM_datatypes.cpp -o $(OBJ_DIR)/CM_datatypes.o

$(OBJ_DIR)/CM_xmlparser.o: $(SRC_DIR)/CM_xmlparser.cpp
	$(COMP) $(COMP_FLAGS) $(INC) CM_xmlparser.cpp -o $(OBJ_DIR)/CM_xmlparser.o

$(OBJ_DIR)/CM_generation.o: $(SRC_DIR)/CM_generation.cpp
	$(COMP) $(COMP_FLAGS) $(INC) CM_generation.cpp -o $(OBJ_DIR)/CM_generation.o

$(OBJ_DIR)/CM_postproc.o: $(SRC_DIR)/CM_postproc.cpp
	$(COMP) $(COMP_FLAGS) $(INC) CM_postproc.cpp -o $(OBJ_DIR)/CM_postproc.o

$(OBJ_DIR)/CM_bmp.o: $(SRC_DIR)/BMP/EasyBMP.cpp
	$(COMP) $(COMP_FLAGS) $(INC)/BMP BMP/EasyBMP.cpp -o $(OBJ_DIR)/CM_bmp.o

$(OBJ_DIR)/CM_setup.o: $(SRC_DIR)/CM_setup.cpp
	$(COMP) $(COMP_FLAGS) $(INC) $(SRC_DIR)/CM_setup.cpp -o $(OBJ_DIR)/CM_setup.o

$(OBJ_DIR)/CM_ui.o:
	$(COMP) $(COMP_FLAGS) $(INC) $(SRC_DIR)/CM_ui.cpp -o $(OBJ_DIR)/CM_ui.o

$(OBJ_DIR)/CM_run.o: $(SRC_DIR)/CM_run.cpp
	$(COMP) $(COMP_FLAGS) $(INC) $(OMP) $(SRC_DIR)/CM_run.cpp	-o $(OBJ_DIR)/CM_run.o

$(OBJ_DIR)/CM_shapefun.o: $(SRC_DIR)/CM_shapefun.cpp
	$(COMP) $(COMP_FLAGS) $(INC) $(SRC_DIR)/CM_shapefun.cpp -o $(OBJ_DIR)/CM_shapefun.o

$(LIB_DIR)/cmlib.a:  $(OBJ_DIR)/CM_generation.o $(OBJ_DIR)/CM_bmp.o $(OBJ_DIR)/CM_datatypes.o $(OBJ_DIR)/CM_postproc.o $(OBJ_DIR)/CM_setup.o $(OBJ_DIR)/CM_xmlparser.o $(OBJ_DIR)/CM_run.o $(OBJ_DIR)/CM_shapefun.o
	ar rcs $(LIB_DIR)/cmlib.a $(OBJ_DIR)/*.o

single_column_test: $(LIB_DIR)/cmlib.a
	$(LOADER) $(INC) $(OMP) $(DEBUG) $(TEST_SRC_DIR)/single_column_test.cpp $(LIB_DIR)/cmlib.a -o $(TEST_BIN_DIR)/single_column_test

u_tasks_creation: $(LIB_DIR)/cmlib.a
	$(LOADER) $(INC) $(DEBUG) $(TEST_SRC_DIR)/task_creation.cpp $(LIB_DIR)/cmlib.a -o $(TEST_BIN_DIR)/u_tasks_creation

tests_run:
	$(TEST_BIN_DIR)/single_column_test

clean:
	rm $(OBJ)/*
	rm $(BIN)/*
