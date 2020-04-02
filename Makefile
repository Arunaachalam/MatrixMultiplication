CXX = g++
CXXFLAGS = -std=c++1z -Wall -Wextra -Wshadow -Werror -O3 -DNDEBUG -mavx

INCLUDES =
LDFLAGS =
LIBS =

# blas
INCLUDES += -I/usr/lib64/atlas/include/
LDFLAGS += -L/usr/lib64/atlas/
LIBS += -lcblas -latlas

# likwid
CXXFLAGS += -DUSE_LIKWID -pthread
INCLUDES += -I/usr/local/likwid-3.1.2/include/
LDFLAGS += -L/usr/local/likwid-3.1.2/lib/
LIBS += -llikwid

TARGET = matmult
OBJS = $(TARGET).o

all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

$(TARGET).o: $(TARGET).cpp Timer.h Creatematrix.h Filereader.h Matmult.h Matrix.h Makefile
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(TARGET).cpp

clean:
	@$(RM) -rf *.o $(TARGET)
