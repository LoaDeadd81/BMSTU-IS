sign.out: main.c rsa.c rsa.h md5.c md5.h
	gcc $^ -o $@ -lm

keys:
	./sign.out G open.key close.key

test1-sign:
	./sign.out S ./test/test1_in.txt close.key ./test/test1_tmp.sign
	./sign.out C ./test/test1_in.txt open.key ./test/test1_tmp.sign

test2-sign:
	./sign.out S ./test/test2_in.txt close.key ./test/test2_tmp.sign
	./sign.out C ./test/test2_in.txt open.key ./test/test2_tmp.sign

test3-sign:
	./sign.out S ./test/test3_in.txt close.key ./test/test3_tmp.sign
	./sign.out C ./test/test3_in.txt open.key ./test/test3_tmp.sign

test4-sign:
	./sign.out S ./test/test4_in.tar.gz close.key ./test/test4_tmp.sign
	./sign.out C ./test/test4_in.tar.gz open.key ./test/test4_tmp.sign

test5-sign:
	./sign.out S ./test/test5_in.png close.key ./test/test5_tmp.sign
	./sign.out C ./test/test5_in.png open.key ./test/test5_tmp.sign

test6-sign:
	./sign.out S ./test/test4_in.tar.gz close.key ./test/test5_tmp.sign
	./sign.out C ./test/test5_in.png open.key ./test/test5_tmp.sign

test-sign: sign.out keys test1-sign test2-sign test3-sign test4-sign test5-sign test6-sign

test1-md5:
	cmp -n 32 <(./sign.out H ./test/test1_in.txt) <(md5sum ./test/test1_in.txt)

test2-md5:
	cmp -n 32 <(./sign.out H ./test/test2_in.txt) <(md5sum ./test/test2_in.txt)

test3-md5:
	cmp -n 32 <(./sign.out H ./test/test3_in.txt) <(md5sum ./test/test3_in.txt)

test4-md5:
	cmp -n 32 <(./sign.out H ./test/test4_in.tar.gz) <(md5sum ./test/test4_in.tar.gz)

test5-md5:
	cmp -n 32 <(./sign.out H ./test/test5_in.png) <(md5sum ./test/test5_in.png)

test-md5: sign.out test1-md5 test2-md5 test3-md5 test4-md5 test5-md5

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*
	rm -rf *.key *.sign