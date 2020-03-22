CC = g++
CFLAGS = -o3 -Wall -std=c++11
LIBS = 
TESTLIBS = 
SRC = src
TEST_DIR = test/unit
PERF_DIR = test/perf
OUT_DIR = bin
SOURCES = $(wildcard $(SRC)/*.cpp)
TESTS = $(wildcard $(TEST_DIR)/*.cpp)
PERFORMANCE = $(wildcard $(PERF_DIR)/*.cpp)
OBJS = bin/matching
OBJSTEST = bin/test_matching
DBGFLAGS = -g
PRFFLAGS = -pg
MKDIR_P = mkdir -p

all: directories
	$(CC) $(CFLAGS) $(SOURCES) -o $(OBJS) $(LIBS)

directories: $(OUT_DIR)

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

debug:
	$(CC) $(CFLAGS) $(DBGFLAGS) $(SOURCES) -o $(OBJS) $(LIBS)

.PHONY: test
test: 
	$(CC) $(CFLAGS) $(TESTS) -o $(OBJSTEST) $(LIBS) $(TESTLIBS)
	./$(OBJSTEST)

perf:
	$(CC) $(CFLAGS) $(PERFORMANCE) -o $(OBJSTEST) $(LIBS) $(TESTLIBS)
	./$(OBJSTEST)

prof:
	$(CC) $(CFLAGS) $(PRFFLAGS) $(SOURCES) -o $(OBJS) $(LIBS)

memleak:
	valgrind --leak-check=yes ./run.sh 

clean:
	rm -rf $(OUT_DIR)/

