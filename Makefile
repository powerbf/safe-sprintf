
all: test_makestring

test_makestring: test_makestring.cc make_string.cc make_string.h
	g++ -Wall -O2 -o test_makestring test_makestring.cc make_string.cc

clean:
	rm -f test_makestring
