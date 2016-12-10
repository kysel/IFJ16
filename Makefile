SHELL := /bin/bash
all:
	make -C docs
	rm -rf odevzdat || true
	mkdir odevzdat/
	cp -a tbz/. odevzdat/ 
	cp docs/dokumentace.pdf odevzdat/
	find src -name '*.c' -and ! -name '*test.c' -or -name '*.h' | xargs cp -t odevzdat/
	make clean -C src #smazat logy testu
	cp -r test/ odevzdat/test && ./removator.sh
	sed -i -e '18s/.*/SOURCE_FILE=\..\/ifj16/' odevzdat/test/test
	cd odevzdat/ && tar -zcvf ../xkyzli02.tgz *

clean:
	rm -rf odevzdat
	rm -f xkyzli02.tgz
	make clean -C docs
	make clean -C src
