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

//! test appending a formatted string with all 8-bit characters
static void test_add_fmt_bytes(void);

//! test appending a formatted string with an empty format string
static void test_add_fmt_empty(void);

//! test appending a formatted string with a long value
static void test_add_fmt_long(void);

//! test appending a formatted string with no format arguments
static void test_add_fmt_no_args(void);

//! test appending a formatted string with a null format string
static void test_add_fmt_null(void);

//! test appending a formatted string with a simple value
static void test_add_fmt_simple(void);

//! test appending a formatted string with a utf-8 value
static void test_add_fmt_utf8(void);

//! test appending a string value including all 8-bit characters
static void test_add_text_bytes(void);

//! test appending an empty string value
static void test_add_text_empty(void);

//! test appending a long string value
static void test_add_text_long(void);

//! test appending a null string value
static void test_add_text_null(void);

//! test appending a simple string value
static void test_add_text_simple(void);

//! test appending a utf-8 string value
static void test_add_text_utf8(void);

//! test getting the basename of an absolute path
static void test_basename_absolute(void);

//! test getting the basename after changing a string
static void test_basename_changed(void);

//! test getting the basename of the current directory
static void test_basename_cwd(void);

//! test getting the basename of an empty path
static void test_basename_empty(void);

//! test getting the basename of a filename
static void test_basename_file(void);

//! test getting the basename of a null path
static void test_basename_null(void);

//! test getting the basename of a relative path
static void test_basename_relative(void);

//! test getting the basename of the root directory
static void test_basename_root(void);

//! test appending control codes to a string
static void test_control_codes(void);

//! test copying a string
static void test_copy(void);

//! test freeing a string
static void test_free_new(void);

//! test freeing a null string
static void test_free_null(void);

//! test freeing a null string pointer
static void test_free_null_ptr(void);

//! test copying a string's value
static void test_get_copy(void);

//! test creating a string with all 8-bit characters
static void test_new_bytes(void);

//! test creating a string with an empty initial value
static void test_new_empty(void);

//! test creating a string with a long initial value
static void test_new_long(void);

//! test creating a string with a null pointer
static void test_new_null_ptr(void);

//! test creating a string with a simple initial value
static void test_new_simple(void);

//! test creating a string with a utf-8 initial value
static void test_new_utf8(void);

//! test formatting a string with all 8-bit characters
static void test_set_fmt_bytes(void);

//! test formatting a string with an empty format string
static void test_set_fmt_empty(void);

//! test formatting a string with a long value
static void test_set_fmt_long(void);

//! test formatting a string with no format arguments
static void test_set_fmt_no_args(void);

//! test formatting a string with a null format string
static void test_set_fmt_null(void);

//! test formatting a string with a simple value
static void test_set_fmt_simple(void);

//! test formatting a string with a utf-8 value
static void test_set_fmt_utf8(void);

//! test setting a string value including all 8-bit characters
static void test_set_text_bytes(void);

//! test setting an empty string value
static void test_set_text_empty(void);

//! test setting a long string value
static void test_set_text_long(void);

//! test setting a null string value
static void test_set_text_null(void);

//! test setting a simple string value
static void test_set_text_simple(void);

//! test setting a utf-8 string value
static void test_set_text_utf8(void);

//! test getting the size of a string of control codes
static void test_size_control(void);

//! test getting the size of an empty string
static void test_size_empty(void);

//! test getting the size of a string of text and control codes
static void test_size_mixed(void);

//! test getting the size of a null string
static void test_size_null(void);

//! test getting the size of a string of text
static void test_size_text(void);

//! test getting the width of a string of control codes
static void test_width_control(void);

//! test getting the width of an empty string
static void test_width_empty(void);

//! test getting the width of a string of text and control codes
static void test_width_mixed(void);

//! test getting the width of a null string
static void test_width_null(void);

//! test getting the width of a string of text
static void test_width_text(void);

//! run the test program
//!
//! \return `EXIT_SUCCESS` on success, `EXIT_FAILURE` on error
int main(void);

//! all 8-bit character values
char const text_bytes[] =
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
      "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
      "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"
      "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"
      "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f"
      "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f"
      "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
      "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f"
      "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"
      "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf"
      "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
      "\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf"
      "\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf"
      "\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef"
      "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff";

//! a long (1 KiB) text value
char const text_long[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

//! a utf-8 text value
//!
//! this is an automated translation of "this is utf-8 text" to traditional
//! chinese.
char const text_utf8[] = "這是UTF-8文本";

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

   // test kstr_free()
   test_free_new();
   test_free_null();
   test_free_null_ptr();

   // test kstr_new()
   test_new_bytes();
   test_new_empty();
   test_new_long();
   test_new_null_ptr();
   test_new_simple();
   test_new_utf8();

   // test kstr_copy()
   test_copy();

   // test kstr_get_copy()
   test_get_copy();

   // test kstr_set_text()
   test_set_text_bytes();
   test_set_text_empty();
   test_set_text_long();
   test_set_text_null();
   test_set_text_simple();
   test_set_text_utf8();

   // test kstr_set_fmt()
   test_set_fmt_bytes();
   test_set_fmt_empty();
   test_set_fmt_long();
   test_set_fmt_no_args();
   test_set_fmt_null();
   test_set_fmt_simple();
   test_set_fmt_utf8();

   // test kstr_add_text()
   test_add_text_bytes();
   test_add_text_empty();
   test_add_text_long();
   test_add_text_null();
   test_add_text_simple();
   test_add_text_utf8();

   // test kstr_add_fmt()
   test_add_fmt_bytes();
   test_add_fmt_empty();
   test_add_fmt_long();
   test_add_fmt_no_args();
   test_add_fmt_null();
   test_add_fmt_simple();
   test_add_fmt_utf8();

   // test kstr_basename()
   test_basename_absolute();
   test_basename_changed();
   test_basename_cwd();
   test_basename_empty();
   test_basename_file();
   test_basename_null();
   test_basename_relative();
   test_basename_root();

   // test kstr_size()
   test_size_control();
   test_size_empty();
   test_size_mixed();
   test_size_null();
   test_size_text();

   // test kstr_width()
   test_width_control();
   test_width_empty();
   test_width_mixed();
   test_width_null();
   test_width_text();

   // test kstr_add_bg(), kstr_add_fg(), kstr_add_bold(), kstr_add_reset()
   test_control_codes();

   return EXIT_SUCCESS;
}

static
void
test_add_fmt_bytes(void)
{
   fputs(
         "test: append a formatted string with all 8-bit characters\n",
         stderr);

   char const * const text = text_bytes;
   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, "%s", text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_fmt_empty(void)
{
   fputs(
         "test: append a formatted string with an empty format string\n",
         stderr);

   kstr * str = kstr_new(__func__);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-zero-length"
   kstr_add_fmt(str, "");
#pragma GCC diagnostic pop

   char const * const expected = __func__;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_add_fmt_long(void)
{
   fputs("test: append a formatted string with a long value\n", stderr);

   char const * const text = text_long;
   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, "%s", text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_fmt_no_args(void)
{
   fputs("test: append a formatted string with no format arguments\n", stderr);

   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, __func__);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, __func__) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, __func__);

   kstr_free(&str);
}

static
void
test_add_fmt_null(void)
{
   fputs(
         "test: append a formatted string with a null format string\n",
         stderr);

   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, NULL);

   char const * const expected = __func__;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_add_fmt_simple(void)
{
   fputs("test: append a formatted string with a simple value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, "%s", text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_fmt_utf8(void)
{
   fputs("test: append a formatted string with a utf-8 value\n", stderr);

   char const * const text = text_utf8;
   kstr * str = kstr_new(__func__);
   kstr_add_fmt(str, "%s", text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_text_bytes(void)
{
   fputs(
         "test: append a string value including all 8-bit characters\n",
         stderr);

   char const * const text = text_bytes;
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_text_empty(void)
{
   fputs("test: append an empty string value\n", stderr);

   char const * const text = "";
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_text_long(void)
{
   fputs("test: append a long string value\n", stderr);

   char const * const text = text_long;
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_text_null(void)
{
   fputs("test: append a null string value\n", stderr);

   char const * const text = NULL;
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   char const * const expected = __func__;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_add_text_simple(void)
{
   fputs("test: append a simple string value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_add_text_utf8(void)
{
   fputs("test: append a utf-8 string value\n", stderr);

   char const * const text = text_utf8;
   kstr * str = kstr_new(__func__);
   kstr_add_text(str, text);

   size_t const func_length = sizeof(__func__) - 1;
   char const * const value = kstr_get(str);
   if (
         strncmp(value, __func__, func_length) != 0 ||
         strcmp(value + func_length, text) != 0)
      err("value [%s], expecting [%s%s]", value, __func__, text);

   kstr_free(&str);
}

static
void
test_basename_absolute(void)
{
   fputs("test: get the basename of an absolute path\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "/one/two/%s", __func__);

   char const * const expected = __func__;
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_changed(void)
{
   fputs("test: get the basename after changing a string\n", stderr);

   char const * const text1 = "/one/two/three";
   char const * const text2 = "/four/five/six";
   kstr * str = kstr_new(text1);

   kstr_basename(str);
   kstr_set_text(str, text2);

   char const * const expected = "six";
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_cwd(void)
{
   fputs("test: get the basename of the current directory\n", stderr);

   char const * const text = ".";
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_empty(void)
{
   fputs("test: get the basename of an empty path\n", stderr);

   kstr * str = kstr_new(NULL);

   char const * const expected = ".";
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_file(void)
{
   fputs("test: get the basename of a filename\n", stderr);

   kstr * str = kstr_new(__func__);

   char const * const expected = __func__;
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_null(void)
{
   fputs("test: get the basename of a null path\n", stderr);

   kstr * str = kstr_new(NULL);

   char const * const expected = ".";
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_relative(void)
{
   fputs("test: get the basename of a relative path\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "one/two/%s", __func__);

   char const * const expected = __func__;
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_basename_root(void)
{
   fputs("test: get the basename of the root directory\n", stderr);

   char const * const text = "/";
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_basename(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

//! test appending control codes to a string
static
void
test_control_codes(void)
{
   fputs("test: append control codes to a string\n", stderr);

   char const * const text = "x";
   kstr * str = kstr_new(NULL);

   // print a line for each foreground color
   for (kstr_color fg = 0; fg < kstr_num_colors; fg++)
   {
      // clear the string for this line of output
      kstr_set_text(str, NULL);

      // for each background color, add the fg/bg color attributes, normal
      // text, bold text, and unbolded text
      for (kstr_color bg = 0; bg < kstr_num_colors; bg++)
      {
         kstr_add_fg(str, fg);
         kstr_add_bg(str, bg);
         kstr_add_text(str, text);
         kstr_add_bold(str, true);
         kstr_add_text(str, text);
         kstr_add_bold(str, false);
         kstr_add_text(str, text);
      }

      // reset the attributes and add the last text before printing the line
      kstr_add_bold(str, true);
      kstr_add_reset(str);
      kstr_add_text(str, text);
      kstr_add_text(str, "\n");
      fputs(kstr_get(str), stderr);
   }

   kstr_free(&str);
}

static
void
test_copy(void)
{
   fputs("test: copy a string\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(text);
   kstr * str_copy = kstr_copy(str);

   if (str == str_copy)
      err("str at %p returned self as copy", (void *) str);

   char const * const expected = text;
   char const * const value = kstr_get(str_copy);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
   kstr_free(&str_copy);
}

static
void
test_free_new(void)
{
   fputs("test: free a string\n", stderr);

   kstr * str = kstr_new(NULL);
   if (str == NULL)
      err("null str, expecting non-null");

   kstr_free(&str);
   if (str != NULL)
      err("str at %p, expecting null", (void *) str);
}

static
void
test_free_null(void)
{
   fputs("test: free a null string\n", stderr);

   kstr * str = NULL;
   kstr_free(&str);
   if (str != NULL)
      err("str at %p, expecting null", (void *) str);
}

static
void
test_free_null_ptr(void)
{
   fputs("test: free a null string pointer\n", stderr);
   kstr_free(NULL);
}

static
void
test_get_copy(void)
{
   fputs("test: copy a string's value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   char * const value_copy = kstr_get_copy(str);

   if (value == value_copy)
      err("value at %p returned as copy", (void *) value_copy);
   if (strcmp(value_copy, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   free(value_copy);
   kstr_free(&str);
}

static
void
test_new_bytes(void)
{
   fputs("test: create a string with all 8-bit characters\n", stderr);

   char const * const text = text_bytes;
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_new_empty(void)
{
   fputs("test: create a string with an empty initial value\n", stderr);

   char const * const text = "";
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_new_long(void)
{
   fputs("test: create a string with a long initial value\n", stderr);

   char const * const text = text_long;
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_new_null_ptr(void)
{
   fputs("test: create a string with a null pointer\n", stderr);

   char const * const text = NULL;
   kstr * str = kstr_new(text);
   if (str == NULL)
      err("null str, expecting non-null");

   char const * const expected = "";
   char const * const value = kstr_get(str);
   if (value == NULL)
      err("null value, expecting non-null");
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_new_simple(void)
{
   fputs("test: create a string with a simple initial value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_new_utf8(void)
{
   fputs("test: create a string with a utf-8 initial value\n", stderr);

   char const * const text = text_utf8;
   kstr * str = kstr_new(text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_bytes(void)
{
   fputs("test: format a string with all 8-bit characters\n", stderr);

   char const * const text = text_bytes;
   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "%s", text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_empty(void)
{
   fputs("test: format a string with an empty format string\n", stderr);

   kstr * str = kstr_new(NULL);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-zero-length"
   kstr_set_fmt(str, "");
#pragma GCC diagnostic pop

   char const * const expected = "";
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_long(void)
{
   fputs("test: format a string with a long value\n", stderr);

   char const * const text = text_long;
   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "%s", text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_no_args(void)
{
   fputs("test: format a string with no format arguments\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, __func__);

   char const * const expected = __func__;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_null(void)
{
   fputs("test: format a string with a null format string\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, NULL);

   char const * const expected = "";
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_simple(void)
{
   fputs("test: format a string with a simple value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "%s", text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_fmt_utf8(void)
{
   fputs("test: format a string with a utf-8 value\n", stderr);

   char const * const text = text_utf8;
   kstr * str = kstr_new(NULL);
   kstr_set_fmt(str, "%s", text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_bytes(void)
{
   fputs("test: set a string value including all 8-bit characters\n", stderr);

   char const * const text = text_bytes;
   kstr * str = kstr_new(__func__);
   kstr_set_text(str, text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_empty(void)
{
   fputs("test: set an empty string value\n", stderr);

   char const * const text = "";
   kstr * str = kstr_new(__func__);
   kstr_set_text(str, text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_long(void)
{
   fputs("test: set a long string value\n", stderr);

   char const * const text = text_long;
   kstr * str = kstr_new(__func__);
   kstr_set_text(str, text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_null(void)
{
   fputs("test: set a null string value\n", stderr);

   char const * const text = NULL;
   kstr * str = kstr_new(__func__);
   kstr_set_text(str, text);

   char const * const expected = "";
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_simple(void)
{
   fputs("test: set a simple string value\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(NULL);
   kstr_set_text(str, text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_set_text_utf8(void)
{
   fputs("test: set a utf-8 string value\n", stderr);

   char const * const text = text_utf8;
   kstr * str = kstr_new(__func__);
   kstr_set_text(str, text);

   char const * const expected = text;
   char const * const value = kstr_get(str);
   if (strcmp(value, expected) != 0)
      err("value [%s], expecting [%s]", value, expected);

   kstr_free(&str);
}

static
void
test_size_control(void)
{
   fputs("test: get the size of a string of control codes\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_add_bold(str, true);
   kstr_add_bold(str, false);

   size_t const size = kstr_size(str);
   if (size == 0)
      err("size [0], expecting [>0]");

   kstr_free(&str);
}

static
void
test_size_empty(void)
{
   fputs("test: get the size of an empty string\n", stderr);

   char const text[] = "";
   kstr * str = kstr_new(text);

   size_t const expected = sizeof(text);
   size_t const size = kstr_size(str);
   if (size != expected)
      err("size [%zu], expecting [%zu]", size, expected);

   kstr_free(&str);
}

static
void
test_size_mixed(void)
{
   fputs("test: get the size of a string of text and control codes\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_add_bold(str, true);
   kstr_add_text(str, __func__);
   kstr_add_bold(str, false);

   size_t const size = kstr_size(str);
   if (size <= sizeof(__func__))
      err("size [%zu], expecting [>%zu]", size, sizeof(__func__));

   kstr_free(&str);
}

static
void
test_size_null(void)
{
   fputs("test: get the size of a null string\n", stderr);

   char const text[] = "";
   kstr * str = kstr_new(NULL);

   size_t const expected = sizeof(text);
   size_t const size = kstr_size(str);
   if (size != expected)
      err("size [%zu], expecting [%zu]", size, expected);

   kstr_free(&str);
}

static
void
test_size_text(void)
{
   fputs("test: get the size of a string of text\n", stderr);

   kstr * str = kstr_new(__func__);

   size_t const expected = sizeof(__func__);
   size_t const size = kstr_size(str);
   if (size != expected)
      err("size [%zu], expecting [%zu]", size, expected);

   kstr_free(&str);
}

static
void
test_width_control(void)
{
   fputs("test: get the width of a string of control codes\n", stderr);

   kstr * str = kstr_new(NULL);
   kstr_add_bold(str, true);
   kstr_add_bold(str, false);

   size_t const expected = 0;
   size_t const width = kstr_width(str);
   if (width != expected)
      err("width [%zu], expecting [%zu]", width, expected);

   kstr_free(&str);
}

static
void
test_width_empty(void)
{
   fputs("test: get the width of an empty string\n", stderr);

   kstr * str = kstr_new("");

   size_t const expected = 0;
   size_t const width = kstr_width(str);
   if (width != expected)
      err("width [%zu], expecting [%zu]", width, expected);

   kstr_free(&str);
}

static
void
test_width_mixed(void)
{
   fputs(
         "test: get the width of a string of text and control codes\n",
         stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(NULL);
   kstr_add_bold(str, true);
   kstr_add_text(str, text);
   kstr_add_bold(str, false);

   size_t const expected = strlen(text);
   size_t const width = kstr_width(str);
   if (width != expected)
      err("width [%zu], expecting [%zu]", width, expected);

   kstr_free(&str);
}

static
void
test_width_null(void)
{
   fputs("test: get the width of a null string\n", stderr);

   kstr * str = kstr_new(NULL);

   size_t const expected = 0;
   size_t const width = kstr_width(str);
   if (width != expected)
      err("width [%zu], expecting [%zu]", width, expected);

   kstr_free(&str);
}

static
void
test_width_text(void)
{
   fputs("test: get the width of a string of text\n", stderr);

   char const * const text = __func__;
   kstr * str = kstr_new(text);

   size_t const expected = strlen(text);
   size_t const width = kstr_width(str);
   if (width != expected)
      err("width [%zu], expecting [%zu]", width, expected);

   kstr_free(&str);
}
