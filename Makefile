Clusterer: Image.o Pixel.o ImageCluster.o ImageClusterPixel.o ImagePixel.o Clusterer.o
	g++ Image.o Pixel.o ImageCluster.o ImageClusterPixel.o ImagePixel.o Clusterer.o -o Clusterer --std=c++11

Clusterer.o: ImageClusterPixel.h ImageCluster.h
	g++ -c Clusterer.cpp -o Clusterer.o --std=c++11

test: test.o Image.o Pixel.o ImageCluster.o ImageClusterPixel.o ImagePixel.o
	g++ test.o Image.o Pixel.o ImageCluster.o ImageClusterPixel.o ImagePixel.o -o test --std=c++11

Image.o: Image.h Pixel.h
	g++ -c Image.cpp -o Image.o --std=c++11

Pixel.o: Pixel.h
	g++ -c Pixel.cpp -o Pixel.o --std=c++11

ImageCluster.o: ImageCluster.h Image.h
	g++ -c ImageCluster.cpp -o ImageCluster.o --std=c++11

ImageClusterPixel: ImageClusterPixel.h ImagePixel.h
	g++ -c ImageClusterPixel.cpp -o ImageClusterPixel.o --std=c++11

ImagePixel.o: ImagePixel.h Pixel.h
	g++ -c ImagePixel.cpp -o ImagePixel.o --std=c++11

clean:
	rm *.o ./Clusterer