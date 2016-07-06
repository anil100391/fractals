all : mandelbrot tricorn

mandelbrot:
	g++ -o mandelbrot mandelbrot.cpp -O3 -lcairo -fopenmp
	./mandelbrot
	convert mandelbrot.png0 mandelbrot.png1 -composite mandelbrot.png
	rm mandelbrot.png0 mandelbrot.png1

tricorn:
	g++ -o tricorn tricorn.cpp -O3 -lcairo -fopenmp
	./tricorn
	convert tricorn.png0 tricorn.png1 -composite tricorn.png
	eog tricorn.png
	
clean:
	rm mandelbrot
	rm tricorn

poster:
	g++ -o orbit mandelbrotOrbit.cxx -lcairo -O2 -fopenmp
	./orbit
	convert new.png0 new.png1 -composite out.png1
	convert new.png2 new.png3 -composite out.png2
	convert out.png1 out.png2 -composite out.png
	mv out.png temp.png
