# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

#We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

HEADER_ROOT=../Core/Inc
SRC_ROOT=../Core/Src
UNITY_ROOT=./unity
TEST_ROOT=.

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
#CFLAGS += -Wno-misleading-indentation

TARGET_BASE1=all_tests
TARGET1 = $(TARGET_BASE1)$(TARGET_EXTENSION)
SRC_FILES1=\
  $(UNITY_ROOT)/src/unity.c \
  $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
  $(SRC_ROOT)/hand_storage.c \
  $(TEST_ROOT)/TestDriverMotor.c \
  $(TEST_ROOT)/test_runners/TestDriverMotor_Runner.c \
  $(TEST_ROOT)/test_runners/all_tests.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src -I$(HEADER_ROOT) -I$(UNITY_ROOT)/extras/fixture/src
SYMBOLS=-DUNITY_FIXTURE_NO_EXTRAS

all: clean default

default:
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES1) -o ./bin/$(TARGET1)
	- ./bin/$(TARGET1) -v

clean:
	$(CLEANUP) $(TARGET1)

ci: CFLAGS += -Werror
ci: default