
build: _build
	cd $<; make -j

_build:
	mkdir -p $@
	cd $@; ln -sf ../test_data; cmake ..

unit: build
	_build/unit | tee > /tmp/_unit_out
	if [ -f _unit_out ]; then diff /tmp/_unit_out _unit_out; fi

_unit_out:
	_build/unit > $@

benchmark: build
	_build/benchmark 1000

clean:
	rm -f *.o src/*~ *~
	rm -fr *.o.dSYM _*

%.force:
	rm -f $*; make $*
