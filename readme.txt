% kstr
% kt.d <kt@kt.d>
% 2013-07-26

kstr is an open source (isc) dynamic string library written in c. it requires a
modern c compiler (gcc, clang) and gnu make and can be linked statically or
dynamically, and it's small enough that it can be included and built directly
with other source.

for api documentation, refer to the comments in the header file, or, if doxygen
is available, generate more readable html documents using the appropriate
target in the makefile.

a small test program is included under the test/ directory that runs some basic
regression tests on the library and demonstrates its use.

# building and installing

to build, it should be enough to just run `make`. to install the library and
header file (defaults to `~/.local`, but this can be changed by setting the
`prefix` variable), run `make install`. the test program is built with
`make test`, the api documentation can be generated with `make docs`, and
`make clean` will remove everything created by the build process.

the makefile uses the standard CC, CFLAGS, and LDFLAGS variables to determine
the c compiler, compiler flags, and linker flags, respectively. any of these
can be defined at build-time to use something other than the system defaults,
e.g. `make CC=clang`.
