CC = g++
CFLAGS = -Wall -g

TARGET = subclu

all: $(TARGET)

$(TARGET): Subspace.o BitsUtil.o Main.o Cluster.o Relation.o ReadInput.o DBSCAN.o SUBCLU.o
			$(CC) $(CFLAGS) -o $(TARGET) Main.o Subspace.o BitsUtil.o Cluster.o Relation.o ReadInput.o DBSCAN.o SUBCLU.o

Main.o: Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp

SUBCLU.o: SUBCLU.h Cluster.h Relation.h DBSCAN.h ReadInput.h SUBCLU.cpp
	$(CC) $(CFLAGS) -c SUBCLU.cpp

DBSCAN.o: DBSCAN.h Cluster.h Relation.h Subspace.h DBSCAN.cpp
	$(CC) $(CFLAGS) -c DBSCAN.cpp

Cluster.o: Cluster.h Subspace.h Cluster.cpp
	$(CC) $(CFLAGS) -c Cluster.cpp

Subspace.o: Subspace.h BitsUtil.h Subspace.cpp
	$(CC) $(CFLAGS) -c Subspace.cpp

BitsUtil.o: BitsUtil.h BitsUtil.cpp
	$(CC) $(CFLAGS) -c BitsUtil.cpp

Relation.o: Relation.h Relation.cpp
	$(CC) $(CFLAGS) -c Relation.cpp

ReadInput.o: Relation.h ReadInput.h ReadInput.h
	$(CC) $(CFLAGS) -c ReadInput.cpp

clean:
			$(RM) $(TARGET) *.o
