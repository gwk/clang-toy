
_bld/kaleidoscope: kaleidoscope.cpp
	mkdir -p _bld
	clang++ \
	`llvm-config --cppflags --ldflags --libs core jit native` \
	-std=c++14 \
	-g \
	-Ofast \
	-lz \
	-lcurses \
	$^ \
	-o $@

.PHONY: llvm-config

llvm-config:
	# display the compilation flags specified by llvm-config.
	llvm-config --cppflags --ldflags --libs core jit native
