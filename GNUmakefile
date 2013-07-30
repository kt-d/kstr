# Copyright (c) 2013, kt.d <kt@kt.d>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

# library version information
v_x := 0
v_y := 0
v_z := 0

# installation prefix and directory creation mode
prefix := $(HOME)/.local
mode := 0700

# api output directory (doxygen)
api := api

# library files
lib_o := $(patsubst %.c,%.o,$(wildcard *.c))
lib_h := kstr.h
lib_a := lib$(lib_h:.h=.a)
lib_so := $(lib_a:.a=.so)
lib_so_x := $(lib_so).0
lib_so_xyz := $(lib_so_x).0.0
libs := $(lib_a) $(lib_so) $(lib_so_x) $(lib_so_xyz)

# any .c file in test/ is built into a binary with the same basename
test_bin := $(basename $(wildcard test/*.c))

# use posix.1-2008 xsi interfaces
override CFLAGS += -D _XOPEN_SOURCE=700

# build everything
.DEFAULT_GOAL := all
.PHONY: all
all: $(lib_so)

# clean everything
.PHONY: clean
clean:
	rm -f -- $(libs) $(test_bin) $(wildcard *.o) $(wildcard test/*.o)
	rm -fr -- $(api)/

# generate api documentation
.PHONY: docs
docs:
	rm -fr -- $(api)/
	doxygen -g - | \
		sed \
			-e "/^CALLER_GRAPH[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^CALL_GRAPH[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^DOT_FONTNAME[[:blank:]]*=/s/=.*/= monospace/" \
			-e "/^DOT_GRAPH_MAX_NODES[[:blank:]]*=/s/=.*/= 100/" \
			-e "/^DOT_TRANSPARENT[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^ENUM_VALUES_PER_LINE[[:blank:]]*=/s/=.*/= 1/" \
			-e "/^EXTRACT_ALL[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^EXTRACT_ANON_NSPACES[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^EXTRACT_PACKAGE[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^EXTRACT_PRIVATE[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^EXTRACT_STATIC[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^GENERATE_LATEX[[:blank:]]*=/s/=.*/= NO/" \
			-e "/^GENERATE_TREEVIEW[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^HAVE_DOT[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^HTML_DYNAMIC_SECTIONS[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^HTML_OUTPUT[[:blank:]]*=/s/=.*/= $(api)/" \
			-e "/^INLINE_SOURCES[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^JAVADOC_AUTOBRIEF[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^OPTIMIZE_OUTPUT_FOR_C[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^PREDEFINED[[:blank:]]*=/s/=.*/= kstr_stdc/" \
			-e "/^PROJECT_NAME[[:blank:]]*=/s/=.*/= \"kst api\"/" \
			-e "/^QUIET[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^RECURSIVE[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^REFERENCED_BY_RELATION[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^REFERENCES_RELATION[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^SOURCE_BROWSER[[:blank:]]*=/s/=.*/= YES/" \
			-e "/^STRIP_CODE_COMMENTS[[:blank:]]*=/s/=.*/= NO/" \
			-e "/^WARN_NO_PARAMDOC[[:blank:]]*=/s/=.*/= YES/" | \
		doxygen -

# install everything
.PHONY: install
install: $(libs)
	mkdir -p -m $(mode) -- $(prefix)/include
	mkdir -p -m $(mode) -- $(prefix)/lib
	cp -- $(lib_h) $(prefix)/include/
	cp -P -- $(libs) $(prefix)/lib/

# build the libraries
.PHONY: lib
lib: $(libs)

$(libs): CFLAGS += -fPIC

$(lib_a): $(lib_o)
	$(AR) $(ARFLAGS) $@ $^

$(lib_so): $(lib_so).$(v_x)
	ln -fs $< $@

$(lib_so).$(v_x): $(lib_so).$(v_x).$(v_y).$(v_z)
	ln -fs $< $@

$(lib_so).$(v_x).$(v_y).$(v_z): $(lib_o)
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(lib_so).$(v_x) -o $@ $^ $(LDFLAGS)

# build the tests
.PHONY: test
test: $(test_bin)

test/%: test/%.c $(lib_o)
	$(CC) $(CFLAGS) -I . -o $@ $^ $(LDFLAGS)
