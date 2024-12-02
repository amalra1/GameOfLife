# Nome do executável
TARGET = main

# Diretórios do MergeSAT
MERGESAT_DIR = ./mergesat-files
MERGESAT_SRC = $(MERGESAT_DIR)/src
MERGESAT_LIB = $(MERGESAT_DIR)/lib

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -g -I$(MERGESAT_SRC) -I$(MERGESAT_DIR)/minisat -O2
LDFLAGS = -L$(MERGESAT_LIB)

# Arquivos fonte e objetos
SRCS = main.c reverseLife.c
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)

purge: clean
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean purge
