all : mandelbrot tricorn poster

mandelbrot:
	g++ -o mandelbrot mandelbrot.cpp -O3 -lcairo -fopenmp

tricorn:
	g++ -o tricorn tricorn.cpp -O3 -lcairo -fopenmp

poster:
	g++ -o orbit mandelbrotOrbit.cxx -lcairo -O2 -fopenmp

clean:
	rm mandelbrot
	rm tricorn
	rm orbit
