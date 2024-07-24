CXX = clang++
CXXFLAGS = `llvm-config --cxxflags` -I include -Wno-deprecated-declarations
LDFLAGS = `llvm-config --ldflags --system-libs --libs`

SRCS = src/main.cpp src/compiler/Shipc.cpp src/compiler/CompilerContext.cpp src/compiler/ContextTable.cpp src/lexer/Lexer.cpp src/lexer/Tokens.cpp src/lexer/util.cpp src/parser/Parser.cpp src/parser/ParseExpr.cpp src/parser/ParseStmt.cpp src/parser/ParseType.cpp
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))
TARGET = shipc
PACKAGE = shipc_package

all: build run

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) examples/hello_world.sp

clean-all:
	rm -rf build $(TARGET) $(PACKAGE)

clean:
	rm -rf build/parser

package: $(TARGET)
	mkdir -p $(PACKAGE)
	cp $(TARGET) $(PACKAGE)
	tar -czvf $(PACKAGE).tar.gz $(PACKAGE)

.PHONY: all build clean clean-all run package
