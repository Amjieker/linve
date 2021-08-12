# 基础配置
TOP_DIR:=$(CURDIR)
# 目标
Target:=server
# 源文件路径
SRC_PATH:= $(TOP_DIR) $(TOP_DIR)/business $(TOP_DIR)/http/core/ $(TOP_DIR)/http/tool/
# 编译路径
BUILD_PATH:=$(TOP_DIR)/build
OBJ_PATH:=$(BUILD_PATH)/temp
BIN_PATH:=$(BUILD_PATH)/bin

SRC:=$(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.cpp))
SRC_WITHOUT_DIR:=$(notdir $(SRC))
# 目标文件
OBJ_WITHOUT_DIR:=$(patsubst %.cpp,%.o,$(SRC_WITHOUT_DIR))
# 添加标头
OBJ_WITH_BUILD_DIR:=$(addprefix $(OBJ_PATH)/,$(OBJ_WITHOUT_DIR))
$(info "OBJ_WITH_BUILD_DIR:$(OBJ_WITH_BUILD_DIR)")

# 头文件目录
CFLAGS=$(addprefix -I,$(SRC_PATH)) -g -pthread  -std=c++14
VPATH=$(SRC_PATH)

# MYSQL 链接
MYSQLLINK = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -ldl -lssl -lcrypto -lresolv -lm -lrt

CC = g++

all:build_prepare $(Target)

build_prepare:
	echo $(BUILD_PATH)
	@if [ ! -d $(BUILD_PATH) ]; then \
	mkdir -p $(OBJ_PATH); \
	mkdir -p $(BIN_PATH); \
	fi

$(Target): $(OBJ_WITH_BUILD_DIR) 
	$(CC) $(CFLAGS) -o $(BIN_PATH)/$@ $^ $(MYSQLLINK)
	
$(OBJ_PATH)/%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(MYSQLLINK)

.PHONY:clean

clean:
	-rm -rf $(BIN_PATH)/$(Target) $(OBJ_WITH_BUILD_DIR)

print:
	echo $(SRC_WITHOUT_DIR) $(SRC)


