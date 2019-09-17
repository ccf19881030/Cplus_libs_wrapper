EXEC= mLualib

all: mLualib

mLualib.o: mLualib.cpp
	gcc -Wall -g -fPIC -c mLualib.cpp -o mLualib.o
mLualib: mLualib.o
	gcc -shared mLualib.o -o mLualib.so
clean:
	rm *.o *.so

