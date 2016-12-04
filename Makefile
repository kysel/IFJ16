SHELL := /bin/bash
all:
	make -C docs
	rm -rf odevzdat || true
	mkdir odevzdat/
	cp -a tbz/. odevzdat/ 
	cp docs/dokumentace.pdf odevzdat/
	find src -name '*.c' -and ! -name '*test.c' -or -name '*.h' | xargs cp -t odevzdat/
	cd src/ && make clean && cd .. #smazat logy testu
	cp -r test/ odevzdat/test && ./removator.sh
	cd odevzdat/ && tar -zcvf ../xkyzli02.tgz *

clean:
	rm -rf odevzdat
	rm xkyzli02.tgz
