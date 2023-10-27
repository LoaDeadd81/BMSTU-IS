des.out: main.c enigma.c enigma.h
	gcc $^ -o $@

test1:
	./des.out ./test/test1_in.txt ./test/test1_tmp.txt
	./des.out ./test/test1_tmp.txt test1_out.txt

test2:
	./des.out ./test/test2_in.txt ./test/test2_tmp.txt
	./des.out ./test/test2_tmp.txt test2_out.txt

test3:
	./des.out ./test/test3_in.txt ./test/test3_tmp.txt
	./des.out ./test/test3_tmp.txt test3_out.txt

test4:
	./des.out ./test/test4_in.tar.gz ./test/test4_tmp
	./des.out ./test/test4_tmp test4_out.tar.gz

tests: test1 test2 test3 test4

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*