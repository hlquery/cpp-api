CXX ?= c++
AR ?= ar
OPENSSL ?= 1

BUILD_DIR := build
LIB := $(BUILD_DIR)/libhlqueryclient.a

CPPFLAGS += -Iinclude -Ivendor/json
CXXFLAGS += -std=c++17 -Wall -Wextra -O2
LDFLAGS +=
LDLIBS +=

SOURCES := $(shell find src -name '*.cpp' | sort)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/obj/%.o,$(SOURCES))

EXAMPLE_SOURCES := $(shell find examples -maxdepth 1 -name '*.cpp' | sort)
EXAMPLES := $(patsubst examples/%.cpp,%,$(EXAMPLE_SOURCES))

ifeq ($(OPENSSL),1)
CPPFLAGS += -DHLQUERY_HAS_OPENSSL
OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS := $(shell pkg-config --libs openssl 2>/dev/null || printf '%s\n' '-lssl -lcrypto')
CPPFLAGS += $(OPENSSL_CFLAGS)
LDLIBS += $(OPENSSL_LIBS)
endif

.PHONY: all clean examples

all: $(LIB) examples

examples: $(EXAMPLES)

$(LIB): $(OBJECTS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $^

$(BUILD_DIR)/obj/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(EXAMPLES): %: examples/%.cpp $(LIB)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LIB) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXAMPLES)
