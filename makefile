
_bld/kaleidoscope: cc.sh kaleidoscope.cpp
	./$^ -o $@

_bld/cjit: cc.sh cjit.cpp
	./$^ -o $@

.PHONY: all clean llvm-config

all: _bld/kaleidoscope _bld/cjit

clean:
	rm -rf _bld/*

llvm-config:
	# display the compilation flags specified by llvm-config.
	llvm-config --cppflags --ldflags
