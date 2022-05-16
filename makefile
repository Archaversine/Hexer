SOURCES=$(wildcard *.cpp)
FLAGS=-O3

hexer:
	g++ $(FLAGS) $(SOURCES) -o hexer

install:
	sudo cp ./hexer /usr/local/bin/

uninstall:
	sudo rm /usr/local/bin/hexer

clean:
	rm hexer
