TESTS =  test.c.out \
	 uint_stress.c.out \
	 sint_stress.c.out \
	 float_stress.c.out \
	 bf-2-1-test.c.out \
	 bn_s-2-1-test.c.out \
	 uint_test.cpp.out \
	 uint_test_2.cpp.out \
	 sint_test.cpp.out \
	 sint_test_2.cpp.out \
	 float_test.cpp.out \
	 float_test_2.cpp.out

BIGNUM_FILES = ../bn.c ../bn_s.c ../bf.c

all: $(TESTS)
	

%.cpp.out: %.cpp
	g++ $< $(BIGNUM_FILES) -g -o $@

%.c.out: %.c
	gcc $< $(BIGNUM_FILES) -g -o $@ -lm

clean:
	#rm -f test uint_stress sint_stress float_stress uint_test uint_test_2 sint_test sint_test_2 float_test
	rm -f $(TESTS)
