aes.out: main.c AES.c AES.h
	gcc $^ -o $@ -lm

test1:
	./aes.out E keykeykeykeykeyb ./test/test1_in.txt ./test/test1_tmp.txt
	./aes.out D keykeykeykeykeyb ./test/test1_tmp.txt test1_out.txt

test2:
	./aes.out E keykeykeykeykeyb ./test/test2_in.txt ./test/test2_tmp.txt
	./aes.out D keykeykeykeykeyb ./test/test2_tmp.txt test2_out.txt

test3:
	./aes.out E keykeykeykeykeyb ./test/test3_in.txt ./test/test3_tmp.txt
	./aes.out D keykeykeykeykeyb ./test/test3_tmp.txt test3_out.txt

test4:
	./aes.out E keykeykeykeykeyb ./test/test4_in.tar.gz ./test/test4_tmp
	./aes.out D keykeykeykeykeyb ./test/test4_tmp test4_out.tar.gz

tests: aes.out test1 test2 test3 test4

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*