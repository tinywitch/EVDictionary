#!/bin/bash
echo "Chào mừng đến với từ điển EVDictionary!"
echo "Nhấn Enter để tiếp tục quá trình cài đặt"
read x
cd Controller
cd Data
echo "0%"
make clean >> log 2>> logerr
echo "20%"
make >> log 2>> logerr
echo "40%"
cd ..
cp Data/lib/libbt.a libbt.a
gcc -o btree_dict_generate btree_dict_generate.c dict_generate.h libbt.a -g -Iinc -Llib  -lbt -lm >> Data/log 2>> Data/logerr
./btree_dict_generate 
echo "60%"
gcc -o btree_soundex_generate btree_soundex_generate.c dict_generate.h soundex.h libbt.a -g -Iinc -Llib  -lbt -lm >> Data/log 2>> Data/logerr
./btree_soundex_generate
echo "80%"
gcc -o EnViDictionary main_controller.c main_library.h libbt.a -g -Iinc -Llib  -lbt -lm `pkg-config --cflags --libs gtk+-2.0` >> Data/log 2>> Data/logerr
echo "100%"

echo -n "Bạn đã hoàn tất quá trình cài đặt. Bạn có muốn khởi chạy luôn từ điển không? (y/n) "
read -n 1 selection
if [[ $selection = $'y' ]]
then
	./EnViDictionary >> Data/log 2>> Data/logerr
else
	exit 0
fi
exit 0