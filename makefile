check:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
	rm ./main
build:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
test:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
	./main ./testFile.txt
