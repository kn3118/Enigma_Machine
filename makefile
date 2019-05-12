enigma: main.o enigma.o plugboard.o rotors.o reflector.o
	g++ main.o enigma.o plugboard.o rotors.o reflector.o -o enigma

main.o: main.cpp errors.h enigma.h
	g++ -g -Wall -c main.cpp

enigma.o: enigma.cpp errors.h enigma.h plugboard.h rotors.h reflector.h
	g++ -g -Wall -c enigma.cpp

plugboard.o: plugboard.cpp errors.h plugboard.h
	g++ -g -Wall -c plugboard.cpp

rotors.o: rotors.cpp errors.h rotors.h
	g++ -g -Wall -c rotors.cpp

reflector.o: reflector.cpp errors.h reflector.h
	g++ -g -Wall -c reflector.cpp

clean:
	rm -f *.o enigma
