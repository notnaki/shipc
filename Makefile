CXX = clang++
CXXFLAGS = `llvm-config --cxxflags` -I include
LDFLAGS = `llvm-config --ldflags --system-libs --libs`

SRCS = src/main.cpp src/compiler/Shipc.cpp src/compiler/CompilerContext.cpp src/compiler/ContextTable.cpp src/lexer/Lexer.cpp src/lexer/Tokens.cpp src/lexer/util.cpp src/parser/Parser.cpp src/parser/ParseExpr.cpp src/parser/ParseStmt.cpp src/parser/ParseType.cpp
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))
TARGET = shipc

all: build run

build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all build clean run
