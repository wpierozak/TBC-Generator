# CLMGENERATOR - Makefile #

COMP = g++ -c
LOADER = g++ -fopenmp

DEBUG = -g
OPT = -O2
OMP = -fopenmp 
COMP_FLAGS = -c $(DEBUG)

# Directories #

ROOT = /home/wiktor/Desktop/engineering_thesis
INC_DIR = $(ROOT)/inc
SRC_DIR = $(ROOT)/src
OBJ_DIR = $(ROOT)/obj
BIN_DIR = $(ROOT)/bin
LIB_DIR = $(ROOT)/lib

INC = -I$(INC_DIR)

# CM Library #

CM_COMP_FLAGS = $(DEBUG)
OBJ_CM_DIR = $(OBJ_DIR)/cm
CM_DEPENDENCIES =

$(OBJ_CM_DIR)/CM_xmlparser.o: $(SRC_DIR)/CM_xmlparser.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) CM_xmlparser.cpp -o $(OBJ_CM_DIR)/CM_xmlparser.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_xmlparser.o

$(OBJ_CM_DIR)/CM_generation.o: $(SRC_DIR)/CM_generation.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) CM_generation.cpp -o $(OBJ_CM_DIR)/CM_generation.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_generation.o

$(OBJ_CM_DIR)/CM_postproc.o: $(SRC_DIR)/CM_postproc.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) CM_postproc.cpp -o $(OBJ_CM_DIR)/CM_postproc.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_postproc.o

$(OBJ_CM_DIR)/CM_bmp.o: $(SRC_DIR)/BMP/EasyBMP.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC)/BMP BMP/EasyBMP.cpp -o $(OBJ_CM_DIR)/CM_bmp.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_bmp.o

$(OBJ_CM_DIR)/CM_setup.o: $(SRC_DIR)/CM_setup.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_setup.cpp -o $(OBJ_CM_DIR)/CM_setup.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_setup.o

$(OBJ_CM_DIR)/CM_basictypes.o: $(SRC_DIR)/CM_basictypes.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_basictypes.cpp -o $(OBJ_CM_DIR)/CM_basictypes.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_basictypes.o

$(OBJ_CM_DIR)/CM_mstypes.o: $(SRC_DIR)/CM_mstypes.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_mstypes.cpp -o $(OBJ_CM_DIR)/CM_mstypes.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_mstypes.o

$(OBJ_CM_DIR)/CM_domain.o: $(SRC_DIR)/CM_domain.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_domain.cpp -o $(OBJ_CM_DIR)/CM_domain.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_domain.o

$(OBJ_CM_DIR)/CM_config.o: $(SRC_DIR)/CM_config.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_config.cpp -o $(OBJ_CM_DIR)/CM_config.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_config.o

$(OBJ_CM_DIR)/CM_run.o: $(SRC_DIR)/CM_run.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(OMP) $(SRC_DIR)/CM_run.cpp	-o $(OBJ_CM_DIR)/CM_run.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_run.o

$(OBJ_CM_DIR)/CM_shapefun.o: $(SRC_DIR)/CM_shapefun.cpp
	$(COMP) $(CM_COMP_FLAGS) $(INC) $(SRC_DIR)/CM_shapefun.cpp -o $(OBJ_CM_DIR)/CM_shapefun.o
CM_DEPENDENCIES += $(OBJ_CM_DIR)/CM_shapefun.o

$(LIB_DIR)/cmlib.a: $(CM_DEPENDENCIES)
	ar rcs $(LIB_DIR)/cmlib.a $(OBJ_CM_DIR)/*.o

# Tests #
SRC_TEST_DIR = $(SRC_DIR)/tests
OBJ_TEST_DIR = $(OBJ_DIR)/tests
BIN_TEST_DIR = $(BIN_DIR)/tests

TEST_COMP_FLAGS = 

$(OBJ_TEST_DIR)/single_column.o: $(SRC_TEST_DIR)/single_column.cpp
	$(COMP) $(TEST_COMP_FLAGS) $(INC) $(OMP) $(SRC_TEST_DIR)/single_column.cpp -o $(OBJ_TEST_DIR)/single_column.o

$(BIN_TEST_DIR)/single_column: $(OBJ_TEST_DIR)/single_column.o
	$(LOADER) $(OBJ_TEST_DIR)/single_column.o $(LIB_DIR)/cmlib.a -o $(BIN_TEST_DIR)/single_column

single_column: $(LIB_DIR)/cmlib.a $(BIN_TEST_DIR)/single_column