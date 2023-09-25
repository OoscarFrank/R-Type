all:
	g++ -std=c++17 main.cpp -o hello

clean:
	$(RM) hello