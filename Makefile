files = simpletonshell.c Makefile README test.sh
.SILENT:
default:
	gcc -o simpsh simpletonshell.c
clean:
	rm -f *.o *~  *.txt simpsh
dist:
	tar -zcvf simpletonshell.tar.gz $(files)
check:
	chmod 744 test.sh
	./test.sh
