all: test_pqueue test_bstream

test_pqueue: test_pqueue.cc pqueue.h
	g++ -Wall -Werror -g -std=c++11 -o test_pqueue test_pqueue.cc -pthread -lgtest

test_bstream: test_bstream.cc bstream.h
	g++ -Wall -Werror -g -std=c++11 -o test_bstream test_bstream.cc -pthread -lgtest

clean:
	rm -f *.o test_pqueue test_bstream
