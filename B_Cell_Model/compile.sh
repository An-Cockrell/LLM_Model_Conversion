clear;
# g++ -std=c++11 ./*.cpp -o test.o;
# g++ -std=c++11 -fPIC ./*.cpp -o test.o
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -O0
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -O3
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -Ofast
# g++ -std=c++11 ./*.cpp  -g -O0 -fPIC -Wall -Wpedantic -lSDL2 -o test.o
# g++ -std=c++11 ./*.cpp -Ofast -fPIC -Wall -Wpedantic -lSDL2 -o test.o
g++ -std=c++11 -Iinclude ./src/*.cpp -Ofast -fPIC -Wall -Wpedantic -lSDL2 -o test.o
