lzw.out: main.cpp
	g++ -std=c++20 $^ -o $@

test1:
	./lzw.out C ./test/test1_in.txt ./test/test1_tmp.txt
	./lzw.out D ./test/test1_tmp.txt test1_out.txt

test2:
	./lzw.out C ./test/test2_in.txt ./test/test2_tmp.txt
	./lzw.out D ./test/test2_tmp.txt test2_out.txt

test3:
	./lzw.out C ./test/test3_in.txt ./test/test3_tmp.txt
	./lzw.out D ./test/test3_tmp.txt test3_out.txt

test4:
	./lzw.out C ./test/test4_in.tar.gz ./test/test4_tmp.tar.gz
	./lzw.out D ./test/test4_tmp.tar.gz test4_out.tar.gz

test5:
	./lzw.out C ./test/test5_in.png ./test/test5_tmp.png
	./lzw.out D ./test/test5_tmp.png test5_out.png

test6:
	./lzw.out C ./test/test6_in.jpg ./test/test6_tmp.jpg
	./lzw.out D ./test/test6_tmp.jpg test6_out.jpg

test7:
	./lzw.out C ./test/test7_in.txt ./test/test7_tmp.txt
	./lzw.out D ./test/test7_tmp.txt test7_out.txt

test: lzw.out test1 test2 test3 test4 test5 test6 test7

clean:
	rm -rf ./test/*_tmp*
	rm -rf ./*out*