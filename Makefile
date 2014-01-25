# Note that if cross compiling, build with:
#
#   make NOTEST=1
#
# to avoid trying to load the resulting module.
# Also note that you will need a build-host version of jimsh in the
# PATH in order to build the extension.

# Prefer jimsh in the PATH because it is more likely to be built
# for the build-host rather than the target.

ifdef NOTEST
BUILDOPTS := --notest
endif

export PATH := $(PATH):..

all: lzf.so

static: libjim-lzf.a

lzf.so: lzf.c lzf_c.c lzf_d.c
	../build-jim-ext -I.. -L.. $(BUILDOPTS) $^

libjim-lzf.a: lzf.c lzf_c.c lzf_d.c
	../build-jim-ext -I.. -L.. --static $(BUILDOPTS) $^

# Note: Currently we don't attempt to set LD_LIBRARY_PATH or equivalent

test:
	JIMLIB=. ../jimsh -e 'package require lzf;'

clean:
	rm -f *.o *.so *.a
