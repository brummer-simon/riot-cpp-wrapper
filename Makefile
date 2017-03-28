# name of your application
APPLICATION = riot-cpp-wrapper
BOARD ?= native

CXX = clang++
CPPMIX = 1

FEATURES_REQUIRED += cpp

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

USEMODULE += sema

# Set Flags Compiler Flags
FLAG_1 = -fno-exceptions
FLAG_2 = -fno-rtti
FLAGS += $(FLAG_1) $(FLAG_2)

# Include
INC_1 = -Iinclude
INCS += $(INC_1)

# External Libs

# Assemble Compiler Flags
CXXEXFLAGS += -Os -Wall $(INCS) $(FLAGS)

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
