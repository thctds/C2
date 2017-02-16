all:
	g++ *.cpp -std=c++11 -pthread -O0 -g3 -Wall -o lp2-Thiago

clean:
	rm -f *~ && rm lp2-Thiago && rm log.txt

