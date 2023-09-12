TESTS = bn_test bf_test

all: $(TESTS)

% : %.cpp
	g++ -g -std=c++17 $< -o $@ -lgmpxx -lgmp

clean:
	rm -f $(TESTS)
