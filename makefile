
_bld/kal: cc.sh kal.cpp
	./$^ -o $@

_bld/cjit: cc.sh cjit.cpp
	./$^ -o $@

.PHONY: all clean llvm-config

all: _bld/kal _bld/cjit

clean:
	rm -rf _bld/*

llvm-config:
	# display the compilation flags specified by llvm-config.
	llvm-config --cppflags --ldflags
