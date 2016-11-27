all:
	make -C docs
	rm -rf odevzdat || true
	mkdir odevzdat/
	cp -a tbz/. odevzdat/ 
	cp docs/dokumentace.pdf odevzdat/
	find src -name '*.c' -and ! -name '*test.c' -or -name '*.h' | xargs cp -t odevzdat/
	cd odevzdat/ && tar -zcvf ../xkyzli02.tgz *

clean:
	rm -rf odevzdat
	rm xkyzli02.tgz
