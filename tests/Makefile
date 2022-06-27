all:
	gcc test.c ../bn.c ../bn_s.c ../bf.c -g -o test -lm
	gcc uint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o uint_stress -lm
	gcc sint_stress.c ../bn.c ../bn_s.c ../bf.c -g -o sint_stress -lm
	gcc -DBF_DIAGNOSTICS float_stress.c ../bn.c ../bn_s.c ../bf.c -g -o float_stress -lm
clean:
	rm -f test uint_stress sint_stress float_stress
