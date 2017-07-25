CC := g++

CFLAGS := -std=c++11 -O3 -DNDEBUG -lz
CFLAGS_32 := -m32
CFLAGS_64 := -m64

CFLAGS_M := -bundle -undefined dynamic_lookup
CFLAGS_L := -shared -fPIC -lstdc++

SRC = qgz.cpp

m32: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(CFLAGS_32) $(CFLAGS_M) -o qgz_m32.so

m64: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(CFLAGS_64) $(CFLAGS_M) -o qgz_m64.so

l32: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(CFLAGS_32) $(CFLAGS_L) -o qgz_l32.so

l64: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(CFLAGS_64) $(CFLAGS_L) -o qgz_l64.so

clean:
	rm -f qgz_*.so

.PHONY: clean
