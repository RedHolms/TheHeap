all:
	gcc src/*.c -c
	cp src/*.hpp src/*.tpp src/*.h test/
	g++ test/*.cpp -c
	g++ *.o -o start
	rm *.o test/*.hpp test/*.tpp test/*.h 