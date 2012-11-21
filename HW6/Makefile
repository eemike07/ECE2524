# ECE 2524 Homework 6: Heard it on the Pipeline
# Michael Surratt

all: generator consumer dispatcher

generator: generator.o
	 g++ -Wall -g -o generator generator.o

generator.o: generator.cpp
	 g++ -Wall -g -c generator.cpp

consumer: consumer.o
	 g++ -Wall -g -o consumer consumer.o

consumer.o: consumer.cpp
	 g++ -Wall -g -c consumer.cpp

dispatcher: dispatcher.o
	g++ -Wall -g -o dispatcher dispatcher.o

dispatcher.o: dispatcher.cpp
	g++ -Wall -g -c dispatcher.cpp

clean:
	rm -f *.o generator consumer dispatcher
