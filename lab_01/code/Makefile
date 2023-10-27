enigma.out: main.c enigma.c enigma.h
	gcc $^ -o $@

test1:
	./enigma.out ./test/test1_in.txt ./test/test1_tmp.txt
	./enigma.out ./test/test1_tmp.txt test1_out.txt

test2:
	./enigma.out ./test/test2_in.txt ./test/test2_tmp.txt
	./enigma.out ./test/test2_tmp.txt test2_out.txt

test3:
	./enigma.out ./test/test3_in.txt ./test/test3_tmp.txt
	./enigma.out ./test/test3_tmp.txt test3_out.txt

test4:
	./enigma.out ./test/test4_in.tar.gz ./test/test4_tmp
	./enigma.out ./test/test4_tmp test4_out.tar.gz

tests: enigma.out test1 test2 test3 test4

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*