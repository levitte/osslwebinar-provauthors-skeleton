CC=clang

all: myapp
myapp: LDLIBS=-lcrypto
myapp: myapp.o helloworld.o
helloworld.o: CFLAGS=-DOSSL_provider_init=my_prov_init
helloworld.o: helloworld.c

try:
	./myapp

clean:
	rm *.o myapp
