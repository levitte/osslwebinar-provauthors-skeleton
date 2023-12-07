CC=clang
COMMITID=$(shell git rev-parse HEAD)

%.so: %.o
	$(CC) -shared $(LDFLAGS) $(LDLIBS) -o $@ $+

all: helloworld.so
helloworld.so: LDFLAGS=-Wl,--export-dynamic-symbol=OSSL_provider_init
helloworld.o: CFLAGS=-g -O0 -fPIC -DCOMMITID='"$(COMMITID)"'

try:
	OPENSSL_MODULES=. openssl list -provider helloworld -verbose \
		-providers

clean:
	rm *.so *.o
