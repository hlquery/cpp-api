# Makefile for hlquery C++ Client
# 
# Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include -I./vendor/json
LDFLAGS = 
OPENSSL ?= auto

# Build directory
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR ?= .

# Optional OpenSSL support
ifeq ($(OPENSSL),1)
    ifneq ($(shell pkg-config --exists openssl && echo yes),yes)
        $(error OpenSSL was requested with OPENSSL=1, but pkg-config could not find openssl)
    endif
    CXXFLAGS += $(shell pkg-config --cflags openssl) -DHLQUERY_HAS_OPENSSL
    LDFLAGS += $(shell pkg-config --libs openssl)
else ifeq ($(OPENSSL),auto)
    ifeq ($(shell pkg-config --exists openssl && echo yes),yes)
        CXXFLAGS += $(shell pkg-config --cflags openssl) -DHLQUERY_HAS_OPENSSL
        LDFLAGS += $(shell pkg-config --libs openssl)
    endif
else ifeq ($(OPENSSL),0)
    # Build HTTP-only client without OpenSSL.
else
    $(error Unsupported OPENSSL value '$(OPENSSL)'; use OPENSSL=auto, OPENSSL=1, or OPENSSL=0)
endif

# Source files
SOURCES = src/client.cpp \
          src/collections.cpp \
          src/documents.cpp \
          src/request.cpp \
          src/response.cpp \
          src/search.cpp \
          src/searchresult.cpp \
          src/ranker.cpp \
          src/utils/Config.cpp \
          src/utils/Validator.cpp \
          src/utils/Url.cpp

# Object files in build directory (preserve directory structure)
OBJECTS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Library
LIBRARY = $(BUILD_DIR)/libhlqueryclient.a

# Examples
EXAMPLE_SRCS = $(wildcard examples/*.cpp)
EXAMPLE_BINS = $(patsubst examples/%.cpp,$(BIN_DIR)/%,$(EXAMPLE_SRCS))

.PHONY: all clean library examples $(BUILD_DIR) $(OBJ_DIR)

all: $(BUILD_DIR) library examples

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/utils

library: $(LIBRARY)

$(LIBRARY): $(OBJECTS) | $(BUILD_DIR)
	ar rcs $@ $^

examples: $(EXAMPLE_BINS)

$(BIN_DIR)/%: examples/%.cpp $(LIBRARY) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< -L$(BUILD_DIR) -lhlqueryclient $(LDFLAGS)

# Pattern rule for object files (handles nested directories)
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXAMPLE_BINS)
