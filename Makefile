CC=clang

%.so: %.o
	$(CC) -shared $(LDFLAGS) $(LDLIBS) -o $@ $+

all: helloworld.so
helloworld.so: LDFLAGS=-Wl,--export-dynamic-symbol=OSSL_provider_init
helloworld.o: CFLAGS=-g -O0 -fPIC

try:
	# Unfortunately, 'oepnssl list' is too good at suppressing errors,
	# so it's not feasible for demonstrations.
	OPENSSL_MODULES=. openssl enc -provider helloworld -blargh

clean:
	rm *.so *.o
