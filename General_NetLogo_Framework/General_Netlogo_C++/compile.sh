clear;
# g++ -std=c++11 ./*.cpp -o test.o;
# g++ -std=c++11 -fPIC ./*.cpp -o test.o
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -O0
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -O3
# g++ -std=c++11 -fPIC -Wall ./*.cpp -o test.o -Ofast
g++ -std=c++11 ./*.cpp -fPIC -Wall -lSDL2 -o test.o -Ofast

