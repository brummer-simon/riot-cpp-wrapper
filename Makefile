# name of your application
APPLICATION = riot-cpp-wrapper
BOARD ?= native

CXX = clang++
CPPMIX = 1

# Assemble include directories
INC_1 = -Iinclude
INC_DIRS += $(INC_1)

# Set Flags Compiler Flags
FLAG_1 = -fno-exceptions
FLAG_2 = -fno-rtti
FLAGS += $(FLAG_1) $(FLAG_2)

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Add cpp compiler flags
CXXEXFLAGS += $(INC_DIRS) $(FLAGS)

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
