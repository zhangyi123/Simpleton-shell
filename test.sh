#!/bin/bash

touch file1.txt
touch file2.txt
./simpsh --rdonly file1.txt >/dev/null
if [ $? -ne 0 ]; then 
    echo "error with --rdonly"
fi
./simpsh --wronly file2.txt >/dev/null
if [ $? -ne 0 ]; then 
    echo "error with --wronly"
fi
./simpsh --rdonly f.txt 2>/dev/null
if [ $? -eq 0 ]; then
    echo "non-exist file, no error detected"
fi

#./simpsh --creat --rdonly a.txt >/dev/null
#if [ ! -f ./a.txt ]
#then echo "file flag (creat) error"
#fi

#./simpsh --abort >/dev/null
#if [ $? -eq 0 ]; then
#    echo "abort doesn't work"
#fi
#./simpsh --command 5 6 7 cat 2>/dev/null
#if [ $? -ne 0 ]; then
#    echo "bad file descriptor not detected"
#fi


#rm a.txt
rm file1.txt
rm file2.txt

    
   
		    
