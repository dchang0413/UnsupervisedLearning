all: kmeans

id3: kmeans.cpp
	g++ -std=c++11 kmeans.cpp -o kmeans
