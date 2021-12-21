

#### also add 'NDEBUG'???

TARGET   := 5iar
CXX      := g++
#~ CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror
#~ CXXFLAGS := -pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion -Werror
#~ CXXFLAGS := -pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion -std=c++17
CXXFLAGS := -pedantic-errors -Wall -Wextra -std=c++17
LDFLAGS  := -L/usr/lib -lstdc++ -lm
INCLUDE  := -Iinclude/

#~ HEADERS := $(wildcard include/*.h) $(wildcard include/*/*.h)
#~ HEADERS := $(shell find ./include -name *.hpp -or -name *.h)
#~ SOURCES := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
#~ SOURCES := $(shell find . -name "*.cpp")
SOURCES := $(shell find src -name *.cpp -or -name *.c -or -name *.s)
#~ OBJECTS := $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))
#~ OBJECTS := $(wildcard obj/*.o)
#~ OBJECTS := $(shell find . -name "*.o")
OBJECTS := $(SOURCES:src/%.cpp=obj/%.o)

DEPENDENCIES := $(OBJECTS:.o=.d)


all: $(TARGET)

#~ obj/%.o: src/%.cpp $(HEADERS)
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -MP -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)


.PHONY: all build clean debug release info

run:
	./$(TARGET)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf obj/*
	-@rm -rvf $(TARGET)

info:
	@echo "[*] Object dir:      obj            "
	@echo "[*] Sources:         ${SOURCES}     "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"

