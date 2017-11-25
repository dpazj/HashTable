CC = g++
CFLAGS = -Wall -pedantic -Wextra
FILES = HashNode.h HashTable.h ngram.cpp

ngram: ${FILES}
	${CC} -o ngram ${CFLAGS} ${FILES}
run:
	./ngram

clean:
	rm -f ngram
