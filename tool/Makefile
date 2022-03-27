#Set up directories

TOOL_DIR=./
SRC_DIR=$(TOOL_DIR)
BUILD_DIR=$(TOOL_DIR)/build

# Define compilation flags  
# C_SOURCES += $(wildcard $(SRC_DIR)/*.c)
# C_SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
C_SOURCES += stm32tool_main.cpp
# C_SOURCES += main.cpp
C_INCLUDES += $(TOOL_DIR)

include ./utils/Makefile


CC = g++

$(info "############# build tool ##############" )

all : $(BUILD_DIR) $(BUILD_DIR)/stm32tool

$(BUILD_DIR)/stm32tool:$(C_SOURCES)
	@echo "source list => $(C_SOURCES)"
	$(CC) -I$(C_INCLUDES) $^ -o $@



$(BUILD_DIR):
	mkdir $@	

.PHONY : clean

clean :
	rm $(TOOL_DIR)/obj/*