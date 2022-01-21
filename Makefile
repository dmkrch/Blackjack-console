CXX = g++
CXXFLAGS = -g -Wall

run:
	./prog
clean:
	rm -f *.o prog

Card.o: Card.cpp Card.h
	$(CXX) $(CXXFLAGS) -c Card.cpp -o Card.o

CardDeck.o: CardDeck.cpp CardDeck.h
	$(CXX) $(CXXFLAGS) -c CardDeck.cpp -o CardDeck.o

Casino.o: Casino.cpp Casino.h
	$(CXX) $(CXXFLAGS) -c Casino.cpp -o Casino.o

Croupier.o: Croupier.cpp Croupier.h
	$(CXX) $(CXXFLAGS) -c Croupier.cpp -o Croupier.o

Player.o: Player.cpp Player.h
	$(CXX) $(CXXFLAGS) -c Player.cpp -o Player.o

Server.o: Server.cpp Server.h
	$(CXX) $(CXXFLAGS) -c Server.cpp -o Server.o

Table.o: Table.cpp Table.h
	$(CXX) $(CXXFLAGS) -c Table.cpp -o Table.o

prog: main.cpp CardDeck.o Card.o Casino.o Croupier.o Player.o Server.o Table.o
	$(CXX) $(CXXFLAGS)  -pthread main.cpp Card.o CardDeck.o Casino.o Croupier.o Player.o Server.o Table.o -o prog
