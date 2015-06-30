#!/bin/bash
echo "Chào mừng đến với từ điển EVDictionary!"
echo "Nhấn Enter để tiếp tục quá trình cài đặt"
read x
cd Controller
cd Data
echo "0%"
#make clean 
echo "20%"
#make 
echo "40%"
cd ..
# cp Data/lib/libbt.a libbt.a
# gcc -o btree_dict_generate btree_dict_generate.c dict_generate.h libbt.a -g -Iinc -Llib  -lbt -lm
# ./btree_dict_generate 
echo "60%"
# gcc -o btree_soundex_generate btree_soundex_generate.c dict_generate.h soundex.h libbt.a -g -Iinc -Llib  -lbt -lm 
# ./btree_soundex_generate
echo "80%"
gcc -o EnViDictionary main_controller.c main_library.h libbt.a -g -Iinc -Llib  -lbt -lm `pkg-config --cflags --libs gtk+-2.0` 
echo "100%"
#ln Controller/EnViDictionary EnViDictionary

echo -n "Bạn đã hoàn tất quá trình cài đặt. Bạn có muốn khởi chạy luôn từ điển không? (y/n)  "
read -n 1 selection
if [[ $selection = $'y' ]]
then
	./EnViDictionary 
else
	exit 0
fi
exit 0