PROJECT_NAME = libkusnik


SRC_DIR   = src
OBJ_DIR   = objects
BIN_DIR   = bin
MAIN_DIR  = main
LIB_DIR   = lib
INC_DIR   = include

ifeq ($(CUDA),1)
CC        = nvcc
CPP       = c++
LIB_PATHS = -L/usr/local/cuda/lib64 
OPTFLAGS  = -O2 --compiler-options '-fPIC' -DCUDA
LIBS      = -lcuda -lcudart -lpng -liqa

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CU_FILES  = $(wildcard $(SRC_DIR)/*.cu)

H_FILES   = $(wildcard $(SRC_DIR)/*.h)

CUH_FILES = $(wildcard $(SRC_DIR)/*.cuh)
CUO_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CU_FILES:.cu=.cu.o)))
CU_SRC_FILES = $(wildcard $(SRC_DIR)/*.cu)

else

CC        = g++
AR        = ar -cqs
RANLIB    = ranlib

CFLAGS    = -Wall -pedantic -ansi -fopenmp -fPIC
OPTFLAGS  = -O2 

LIBS      = -lm -lpng -liqa 

BIN_FILES = $(addprefix $(BIN_DIR)/, $(notdir $(MAIN_FILES:.cpp=)))

endif

#common settings
AR        = ar -cqs
RANLIB    = ranlib

FLAGS     = $(CFLAGS) $(OPTFLAGS)


OBJS = $(OBJ_FILES) $(CUO_FILES)
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:.cpp=.o)))
MAIN_FILES = $(wildcard $(MAIN_DIR)/*.cpp)
MAIN_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(MAIN_FILES:.cpp=.o)))
INSTALL_DIR_LIB    = /usr/local/lib
INSTALL_DIR_HEADER = /usr/local/include

#--------------------------------------------------------------------------------------
default: $(OBJS)
	@mkdir -p $(BIN_DIR)

#--------------------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.cpp $(OBJ_FILES) 
	@echo "Compiling [$(notdir $<)]"
	$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@
	@echo "Linking objects to exec file [$(BIN_DIR)/$*]"

ifeq ($(CUDA),1)
	#@cp $(LIB_DIR)/$(PROJECT_NAME).so $(BIN_DIR)/.
	$(CC) -o $(BIN_DIR)/$* $(CUO_FILES) $@ $(FLAGS) $(LIBS) $(LIB_PATHS) -I$(INC_DIR) -L$(LIB_DIR) -lkusnik
else
	$(CC) -o $(BIN_DIR)/$* $(OBJ_FILES) $@ $(FLAGS) $(LIBS) $(LIB_PATHS) -I$(INC_DIR)
endif


$(OBJ_DIR)/%.cu.o: $(SRC_DIR)/%.cu
ifeq ($(CUDA),1)
		@mkdir -p $(OBJ_DIR)
		@echo "Compiling [$(notdir $<)]"
		$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@
endif

#--------------------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@echo "Compiling [$(notdir $<)]"
	$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@ 

#--------------------------------------------------------------------------------------
main: $(MAIN_OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(BIN_DIR)

#--------------------------------------------------------------------------------------
lib: $(OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "Creating static library $(OBJ_DIR)/$(PROJECT_NAME).a" 
ifeq ($(CUDA),1)
	@$(AR) $(LIB_DIR)/$(PROJECT_NAME).a $(OBJS);
	@echo "Creating dynamic library $(LIB_DIR)/$(PROJECT_NAME).so"
	@$(CPP) -o $(LIB_DIR)/$(PROJECT_NAME).so  $(LIB_PATHS) -shared -lcudart $(OBJS)
else
	$(AR) $(LIB_DIR)/$(PROJECT_NAME).a $(OBJ_FILES);
	@echo "Creating dynamic library $(LIB_DIR)/$(PROJECT_NAME).so"
	$(CC) -o $(LIB_DIR)/$(PROJECT_NAME).so  -shared  $(OBJ_FILES)
endif

#--------------------------------------------------------------------------------------
install: lib 
	@echo "Creating Directories"
	@mkdir -p $(INSTALL_DIR_LIB)/
	@mkdir -p $(INSTALL_DIR_HEADER)/
	@echo "Copying files"
	@cp $(INC_DIR)/*.h $(INSTALL_DIR_HEADER)/
	@cp $(LIB_DIR)/$(PROJECT_NAME).a $(INSTALL_DIR_LIB)/
	@echo "Done."

#--------------------------------------------------------------------------------------
clean:
	@echo "Cleaning up..."
	@rm -f $(OBJ_DIR)/*.o
	@rm -f $(BIN_DIR)/main_*
	@rm -f $(BIN_DIR)/out_*
	@rm -f $(LIB_DIR)/*
	@rm -f core
	@echo "Done."

#--------------------------------------------------------------------------------------
uninstall: clean
	@echo "Uninstalling..."
	@rm -f $(INSTALL_DIR_LIB)/$(PROJECT_NAME).a
	@rm -f $(INSTALL_DIR_HEADER)/arbitraryUtil.h
	@rm -f $(INSTALL_DIR_HEADER)/image.h
	@rm -f $(INSTALL_DIR_HEADER)/jconfig.h
	@rm -f $(INSTALL_DIR_HEADER)/jdct.h
	@rm -f $(INSTALL_DIR_HEADER)/jdhuff.h
	@rm -f $(INSTALL_DIR_HEADER)/jerror.h
	@rm -f $(INSTALL_DIR_HEADER)/jinclude.h
	@rm -f $(INSTALL_DIR_HEADER)/jmemsys.h
	@rm -f $(INSTALL_DIR_HEADER)/jmorecfg.h
	@rm -f $(INSTALL_DIR_HEADER)/jpegint.h
	@rm -f $(INSTALL_DIR_HEADER)/jpeglib.h
	@rm -f $(INSTALL_DIR_HEADER)/jversion.h
	@rm -f $(INSTALL_DIR_HEADER)/libmorpho.h
	@rm -f $(INSTALL_DIR_HEADER)/qsort.h
	
	@echo "Done."
