check:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
run:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
	./main
test:
	g++ -o main main.cpp -lncurses -std=c++2a -O2
	./main ./textBuffer.cpp
