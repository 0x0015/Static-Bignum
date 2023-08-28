TESTS = bn bn_s bf

all: $(TESTS)

% : %.cpp
	g++ -g -std=c++17 $< -o $@ -lgmpxx -lgmp

clean:
	rm -f $(TESTS)
