all:
	gcc -g test.c ../bn.c ../bn_s.c ../bf.c -g -o test -lm
	gcc -g uint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o uint_stress -lm
	gcc -g sint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o sint_stress -lm
	gcc -g float_stress.c ../bn.c ../bn_s.c ../bf.c -g -o float_stress -lm
clean:
	rm -f test uint_stress sint_stress float_stress
