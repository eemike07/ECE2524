# ECE 2524 Homework 5: Make a Makefile
# Michael Surratt

all : exprtest

exprtest : exprtest.o driver.o parser.tab.o scanner.o
		g++ -Wall -g -o exprtest exprtest.cpp driver.cpp parser.tab.cpp scanner.cpp

exprtest.o : exprtest.cpp
		g++ -c exprtest.cpp

driver.o : driver.cpp
		g++ -c driver.cpp

parser.tab.o : parser.tab.cpp
		g++ -c parser.tab.cpp

scanner.o : scanner.cpp
		g++ -c scanner.cpp

driver.cpp : scanner.hpp driver.hpp

scanner.hpp : parser.tab.hpp

parser.tab.hpp : parser.ypp
		bison parser.ypp

parser.tab.cpp : parser.ypp expression.hpp scanner.hpp driver.hpp parser.tab.hpp
		bison parser.ypp
	
scanner.cpp : scanner.ll scanner.hpp
		flex -t scanner.ll > scanner.cpp

clean : 
	rm exprtest *.o *.hh *.tab.hpp *.tab.cpp scanner.cpp
