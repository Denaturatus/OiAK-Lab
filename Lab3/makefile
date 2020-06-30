all: kalkulator

kalkulator.o: kalkulator.s
	as -g -o kalkulator.o kalkulator.s

kalkulator: kalkulator.o
	ld -g -o kalkulator kalkulator.o


