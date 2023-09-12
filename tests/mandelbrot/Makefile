TESTS = mandelbrot

all: $(TESTS)

% : %.cpp
	g++ -g -std=c++17 -O2 $< -o $@ -lgmpxx -lgmp

clean:
	rm -f $(TESTS)
