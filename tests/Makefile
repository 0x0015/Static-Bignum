all:
	gcc -g test.c ../bn.c ../bn_s.c ../bf.c -g -o test -lm
	gcc -g uint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o uint_stress -lm
	gcc -g sint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o sint_stress -lm
	gcc -g float_stress.c ../bn.c ../bn_s.c ../bf.c -g -o float_stress -lm
	g++ -g uint_test.cpp ../bn.c ../bn_s.c ../bf.c -o uint_test
	g++ -g sint_test.cpp ../bn.c ../bn_s.c ../bf.c -o sint_test
	g++ -g float_test.cpp ../bn.c ../bn_s.c ../bf.c -o float_test
clean:
	rm -f test uint_stress sint_stress float_stress uint_test sint_test float_test
