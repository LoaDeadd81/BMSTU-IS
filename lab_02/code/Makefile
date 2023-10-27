des.out: main.c DES.c DES.h
	gcc $^ -o $@ -lm

test1:
	./des.out E keykey8b ./test/test1_in.txt ./test/test1_tmp.txt
	./des.out D keykey8b ./test/test1_tmp.txt test1_out.txt

test2:
	./des.out E keykey8b ./test/test2_in.txt ./test/test2_tmp.txt
	./des.out D keykey8b ./test/test2_tmp.txt test2_out.txt

test3:
	./des.out E keykey8b ./test/test3_in.txt ./test/test3_tmp.txt
	./des.out D keykey8b ./test/test3_tmp.txt test3_out.txt

test4:
	./des.out E keykey8b ./test/test4_in.tar.gz ./test/test4_tmp
	./des.out D keykey8b ./test/test4_tmp test4_out.tar.gz

tests: des.out test1 test2 test3 test4

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*