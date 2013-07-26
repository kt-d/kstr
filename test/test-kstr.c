// Copyright (c) 2013, kt.d <kt@kt.d>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
// OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

//! \file
//!
//! kstr test program implementation

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kstr.h>

//! write an error message to stderr and exit
//!
//! \param fmt format string
//! \param ... format string arguments
static _Noreturn void err(char const * fmt, ...);

//! run the test program
//!
//! \return `EXIT_SUCCESS` on success, `EXIT_FAILURE` on error
int main(void);

static
void
err(
      char const * fmt,
      ...)
{
   va_list args;
   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   va_end(args);

   fputc('\n', stderr);
   fflush(stderr);
   exit(EXIT_FAILURE);
}

int
main(void)
{
   // try to set the default locale
   setlocale(LC_ALL, "");

   kstr * str;
   char const * value;

   fputs("test: create a string with a null pointer\n", stderr);
   value = "";
   if ((str = kstr_new(NULL)) == NULL)
      err("null str, expecting non-null");
   if (kstr_get(str) == NULL)
      err("null value, expecting non-null");
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: free a string\n", stderr);
   kstr_free(&str);
   if (str != NULL)
      err("str at %p, expecting null", (void *) str);

   fputs("test: create a string with an initial value\n", stderr);
   value = "test";
   str = kstr_new(value);
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: copy a string's value\n", stderr);
   char * value_copy = kstr_get_copy(str);
   if (value_copy == NULL)
      err("null copy, expecting non-null");
   if (kstr_get(str) == value_copy)
      err("value at %p returned self as copy", (void *) value_copy);
   kstr_free(&str);
   if (strcmp(value_copy, value) != 0)
      err("value copy [%s], expecting [%s]", value_copy, value);
   free(value_copy);

   fputs("test: clear a string's value\n", stderr);
   value = "";
   str = kstr_new("test");
   kstr_set_text(str, NULL);
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: set a string's value\n", stderr);
   value = "test";
   kstr_set_text(str, value);
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: append text\n", stderr);
   value = "test-new";
   kstr_add_text(str, "-new");
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: append formatted text\n", stderr);
   value = "test-new-3";
   kstr_add_fmt(str, "-%d", 3);
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: format a string's value\n", stderr);
   value = "foo/bar/baz";
   kstr_set_fmt(str, "%s", value);
   if (strcmp(kstr_get(str), value) != 0)
      err("value [%s], expecting [%s]", kstr_get(str), value);

   fputs("test: get the basename\n", stderr);
   value = "baz";
   if (strcmp(kstr_basename(str), value) != 0)
      err("basename [%s], expecting [%s]", kstr_basename(str), value);

   fputs("test: copy a string\n", stderr);
   kstr * str_copy = kstr_copy(str);
   if (str == str_copy)
      err("str at %p returned self as copy", (void *) str);
   if (strcmp(kstr_get(str), kstr_get(str_copy)) != 0)
      err("value [%s] != copy [%s]", kstr_get(str), kstr_get(str_copy));
   kstr_free(&str_copy);

   fputs("test: get a string's width\n", stderr);
   size_t width = 11;
   if (kstr_width(str) != width)
      err("width [%zu], expecting [%zu]", kstr_width(str), width);

   fputs("test: build a large string\n", stderr);
   width = 1024;
   value = ".";
   kstr_set_text(str, NULL);
   for (size_t i = 0; i < width; i++)
      kstr_add_text(str, value);
   if (kstr_width(str) != width)
      err("width [%zu], expecting [%zu]", kstr_width(str), width);
   char const * s = kstr_get(str);
   for (size_t i = 0; i < width; i++)
      if (s[i] != *value)
         err("value [%s], expecting %zu * '%c'", s, width, *value);

   fputs("test: get a string's size\n", stderr);
   value = "test";
   width = 4;
   kstr_set_text(str, value);
   kstr_add_bold(str, true);
   if (kstr_width(str) != width)
      err("width [%zu], expecting [%zu]", kstr_width(str), width);
   if (kstr_size(str) == width)
      err("size [%zu], expecting larger", kstr_size(str));

   fputs("test: color/style attributes\n", stderr);
   value = "X";
   for (kstr_color fg = 0; fg < kstr_num_colors; fg++)
   {
      kstr_set_text(str, NULL);
      for (kstr_color bg = 0; bg < kstr_num_colors; bg++)
      {
         kstr_add_fg(str, fg);
         kstr_add_bg(str, bg);
         kstr_add_text(str, value);
         kstr_add_bold(str, true);
         kstr_add_text(str, value);
         kstr_add_bold(str, false);
         kstr_add_text(str, value);
      }

      kstr_add_bold(str, true);
      kstr_add_reset(str);
      kstr_add_text(str, value);
      kstr_add_text(str, "\n");
      fputs(kstr_get(str), stderr);
   }

   kstr_free(&str);
   return EXIT_SUCCESS;
}
