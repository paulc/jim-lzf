# Note that if cross compiling, build with:
#

all: lzf.so

static: libjim-lzf.a

lzf.so: lzf.c lzf_c.c lzf_d.c
	$(JIM)/build-jim-ext -I$(JIM) -L$(JIM) $(BUILDOPTS) $^

libjim-lzf.a: lzf.c lzf_c.c lzf_d.c
	$(JIM)/build-jim-ext -I$(JIM) -L$(JIM) --static $(BUILDOPTS) $^

# Note: Currently we don't attempt to set LD_LIBRARY_PATH or equivalent

test: lzf.so
	JIMLIB=. $(JIM)/jimsh t1.tcl
	
clean:
	rm -f *.o *.so *.a
