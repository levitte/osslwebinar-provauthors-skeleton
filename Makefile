CC=clang

%.so: %.o
	$(CC) -shared $(LDFLAGS) $(LDLIBS) -o $@ $+

all: helloworld.so
helloworld.so: LDFLAGS=-Wl,--export-dynamic-symbol=OSSL_provider_init
helloworld.o: CFLAGS=-g -O0 -fPIC

try:
	OPENSSL_MODULES=. openssl list -provider helloworld -verbose \
		-digest-algorithms

clean:
	rm *.so *.o
