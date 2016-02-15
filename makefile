# simple makefile for fqcounter  
# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -fopenmp

# g++ -o fqcounter -fopenmp  kseq_fastq_counter_OMP.cpp -lz
# the build target executable:
TARGET = fqcounter

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp -lz

clean:
	$(RM) $(TARGET)
