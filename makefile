
_bld/kal: cc.sh kal.cpp
	./$^ -o $@

_bld/cjit: cc.sh cjit.cpp
	./$^ -o $@

_bld/demangle: demangle.cpp
	clang++ -std=c++14 -g $^ -o $@

.PHONY: all clean llvm-config

all: _bld/kal _bld/cjit _bld/demangle

clean:
	rm -rf _bld/*

llvm-config:
	# display the compilation flags specified by llvm-config.
	llvm-config --cppflags --ldflags
