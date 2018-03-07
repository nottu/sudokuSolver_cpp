
CPP=g++
CFLAGS=-w -c -g
LFLAGS=-g
EXEC=sudoku
CPPS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(CPPS))
OBJDIR:=obj
DIROBJS:=$(addprefix $(OBJDIR)/,$(OBJS))

$(EXEC): $(DIROBJS)
	$(CPP) $(LFLAGS) -o $@ $(DIROBJS) -lm

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CPP) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $@

release:
	$(MAKE) CFLAGS="-O2 -w -c" LFLAGS="-O2"

test_easy: $(EXEC)
	./$(EXEC) Sudoku_Instances/Easy1.txt 9

valgrind_easy: $(EXEC)
	valgrind --track-origins=yes --leak-check=yes ./$(EXEC) Sudoku_Instances/Easy1.txt 9