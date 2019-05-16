all: test_pqueue test_bstream zap

test_pqueue: test_pqueue.cc pqueue.h
	g++ -Wall -Werror -g -std=c++11 -o test_pqueue test_pqueue.cc -pthread -lgtest

test_bstream: test_bstream.cc
	g++ -Wall -Werror -g -std=c++11 -o test_bstream test_bstream.cc -pthread -lgtest

zap: zap.cc huffman.h bstream.h pqueue.h
	g++ -Wall -Werror -g -std=c++11 -o zap zap.cc

clean:
	rm -f *.o test_pqueue test_bstream zap
