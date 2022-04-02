PROJECT_NAME = libkusnik_cuda

CC        = nvcc
CPP       = c++
AR        = ar -cqs
RANLIB    = ranlib

FLAGS     = $(OPTFLAGS)
CFLAGS    = -fPIC
OPTFLAGS  = -O2 --compiler-options '-fPIC' 

LIBS      =  -lcuda -lcublas -lcudart -lpng -lpq -liqa
LIBS      =  -lcuda  -lcudart -lpng -lpq -liqa
LIB_PATHS = -L/usr/local/cuda/lib64
#LIBS      =  -lcuda -lpng -lpq -liqa
#TODO libiqa is currently in the root folder, need to copy it somewhere

SRC_DIR   = src
OBJ_DIR   = objects
BIN_DIR   = bin
MAIN_DIR  = main
LIB_DIR   = lib
INC_DIR   = include


CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CU_FILES  = $(wildcard $(SRC_DIR)/*.cu)

H_FILES   = $(wildcard $(SRC_DIR)/*.h)
CUH_FILES = $(wildcard $(SRC_DIR)/*.cuh)

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
CUO_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CU_FILES:.cu=.cu.o)))

OBJS = $(OBJ_FILES) $(CUO_FILES)

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CU_SRC_FILES = $(wildcard $(SRC_DIR)/*.cu)


MAIN_FILES = $(wildcard $(MAIN_DIR)/*.cpp)
MAIN_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(MAIN_FILES:.cpp=.o)))
BIN_FILES = $(addprefix $(BIN_DIR)/, $(notdir $(MAIN_FILES:.cpp=)))

INSTALL_DIR_LIB    = /usr/local/lib
INSTALL_DIR_HEADER = /usr/local/include

#--------------------------------------------------------------------------------------
default: $(OBJS)
	@mkdir -p $(BIN_DIR)


#--------------------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.cpp $(OBJ_FILES) 
	@echo "Compiling [$(notdir $<)]"
	@$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@
	@echo "Linking objects to exec file [$(BIN_DIR)/$*]"
	cp $(LIB_DIR)/$(PROJECT_NAME).so $(BIN_DIR)/.
	@echo $(CC) -o $(BIN_DIR)/$* $(CUO_FILES) $@ $(FLAGS) $(LIBS) $(LIB_PATHS) -I$(INC_DIR) -L$(LIB_DIR) -lkusnik_cuda
	@$(CC) -o $(BIN_DIR)/$* $(CUO_FILES) $@ $(FLAGS) $(LIBS) $(LIB_PATHS) -I$(INC_DIR) -L$(LIB_DIR) -lkusnik_cuda

#$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.cpp $(OBJ_FILES) 
#	@echo "Compiling [$(notdir $<)]"
#	@$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@
#	@echo "Linking objects to exec file [$(BIN_DIR)/$*]"
#	@$(CC) -o $(BIN_DIR)/$* $(OBJ_FILES) $@ $(FLAGS) $(LIBS) $(LIB_PATHS) -I$(INC_DIR)


#--------------------------------------------------------------------------------------
$(OBJ_DIR)/%.cu.o: $(SRC_DIR)/%.cu
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling [$(notdir $<)]"
	@echo $(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@ 
	@$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@echo "Compiling [$(notdir $<)]"
	@echo $(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@ 
	@$(CC) -c $< $(FLAGS) $(LIB_PATHS) -I$(INC_DIR) -o $@ 


#--------------------------------------------------------------------------------------
main: $(MAIN_OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(BIN_DIR)

#--------------------------------------------------------------------------------------
lib: $(OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "Creating static library $(OBJ_DIR)/$(PROJECT_NAME).a" 
	@$(AR) $(LIB_DIR)/$(PROJECT_NAME).a $(OBJS);
	@echo "Creating dynamic library $(LIB_DIR)/$(PROJECT_NAME).so"
	@echo $(CPP) -o $(LIB_DIR)/$(PROJECT_NAME).so  -shared  $(OBJS)
	@$(CPP) -o $(LIB_DIR)/$(PROJECT_NAME).so  $(LIB_PATHS) -shared -lcudart $(OBJS)

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
	@rm -rf $(OBJ_DIR)
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
