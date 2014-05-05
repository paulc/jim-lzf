# Note that if cross compiling, build with:
#

all: lzf.so

static: libjim-lzf.a

lzf.so: lzf.c lzf_c.c lzf_d.c
	$(JIM)/build-jim-ext -I$(JIM) -L$(JIM) $(BUILDOPTS) $^

libjim-lzf.a: lzf.c lzf_c.c lzf_d.c
	$(JIM)/build-jim-ext -I$(JIM) -L$(JIM) --static $(BUILDOPTS) $^

# Note: Currently we don't attempt to set LD_LIBRARY_PATH or equivalent

test:
	JIMLIB=. $(JIM)/jimsh -e 'package require lzf; foreach n {0 1 10 100 1000 10000 100000} { set x [[open /dev/urandom] read $$n]; set z [lzf_compress $$x]; if {[lzf_decompress $$z] == $$x} { puts "$$n - OK" } { puts "$$n - Failed" }}'

clean:
	rm -f *.o *.so *.a
