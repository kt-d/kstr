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
//! kstr string library implementation

#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kstr.h"

//! abort and destroy a string
//!
//! calls `abort()` and destroys the string with kstr_free() in case `SIGABRT`
//! is caught and handled.
//!
//! \param ptr string pointer
//!
//! \return `NULL`
static kstr * kstr_abort(kstr ** ptr);

//! return the number of available bytes in a string's character buffer
//!
//! returns the size of the buffer minus the number of used bytes.
//!
//! \param this string
//!
//! \return the available buffer size in bytes
static size_t kstr_available(kstr * this);

//! append characters to a string's value
//!
//! \param this string
//! \param chars characters to append
//! \param count number of characters from \a chars to append
//! \param visible increase width if true, don't if false
//!
//! \return \a this
static kstr * kstr_add_chars(
      kstr * this, char const * chars, size_t count, bool visible);

//! increase the size of a string's character buffer
//!
//! reallocates the buffer with more space. any pointers to the buffer are
//! invalidated.
//!
//! \param this string
//!
//! \return \a this
static kstr * kstr_grow(kstr * this);

//! reset a string's value to empty
//!
//! the string is changed to use an empty value. the character buffer's
//! allocated size is not altered.
//!
//! \param this string
//! \return \a this
static kstr * kstr_reset(kstr * this);

//! string object structure
struct kstr
{
   size_t data_size; //!< allocated buffer size
   size_t used; //!< number of buffer bytes used (including nul)
   size_t width; //!< string width (visible characters)

   char * basename; //!< storage for the cached basename
   char * data; //!< character buffer
};

static
kstr *
kstr_abort(
      kstr ** ptr)
{
   abort();
   return kstr_free(ptr);
}

kstr *
kstr_add_bg(
      kstr * this,
      kstr_color color)
{
   static char const * const codes[kstr_num_colors] =
   {
      "\x1b[49m", // default
      "\x1b[40m", // black
      "\x1b[44m", // blue
      "\x1b[46m", // cyan
      "\x1b[42m", // green
      "\x1b[45m", // magenta
      "\x1b[41m", // red
      "\x1b[47m", // white
      "\x1b[43m" // yellow
   };

   if ((uintmax_t) color >= (uintmax_t) kstr_num_colors)
      return kstr_abort(&this);

   // append the color control code
   char const * const chars = codes[color];
   return kstr_add_chars(this, chars, strlen(chars), false);
}

kstr *
kstr_add_bold(
      kstr * this,
      bool bold)
{
   // append the bold control code
   char const * const chars = bold ? "\x1b[1m" : "\x1b[22m";
   return kstr_add_chars(this, chars, strlen(chars), false);
}

static
kstr *
kstr_add_chars(
      kstr * this,
      char const * chars,
      size_t count,
      bool visible)
{
   if (chars == NULL || count == 0)
      return this;

   // grow the buffer until enough bytes are available
   while (kstr_available(this) < count)
      if (kstr_grow(this) == NULL)
         return NULL;

   // append the character data
   memcpy(this->data + this->used - 1, chars, count);
   this->used += count;
   this->data[this->used - 1] = '\0';
   if (visible)
      this->width += count;

   // invalidate the cached basename
   free(this->basename);
   this->basename = NULL;

   return this;
}

kstr *
kstr_add_fg(
      kstr * this,
      kstr_color color)
{
   static char const * const codes[kstr_num_colors] =
   {
      "\x1b[39m", // default
      "\x1b[30m", // black
      "\x1b[34m", // blue
      "\x1b[36m", // cyan
      "\x1b[32m", // green
      "\x1b[35m", // magenta
      "\x1b[31m", // red
      "\x1b[37m", // white
      "\x1b[33m" // yellow
   };

   if ((uintmax_t) color >= (uintmax_t) kstr_num_colors)
      return kstr_abort(&this);

   // append the color control code
   char const * const chars = codes[color];
   return kstr_add_chars(this, chars, strlen(chars), false);
}

kstr *
kstr_add_fmt(
      kstr * this,
      char const * fmt,
      ...)
{
   va_list args;
   va_start(args, fmt);
   this = kstr_add_vfmt(this, fmt, args);
   va_end(args);

   return this;
}

kstr *
kstr_add_reset(
      kstr * this)
{
   static char const * const chars = "\x1b[0m";

   // append the reset control code
   return kstr_add_chars(this, chars, strlen(chars), false);
}

kstr *
kstr_add_text(
      kstr * this,
      char const * text)
{
   return kstr_add_chars(this, text, text == NULL ? 0 : strlen(text), true);
}

kstr *
kstr_add_vfmt(
      kstr * this,
      char const * fmt,
      va_list args)
{
   // calculate the formatted string's size
   va_list args_copy;
   va_copy(args_copy, args);
   int length = vsnprintf(NULL, 0, fmt, args_copy);
   va_end(args_copy);

   size_t count = (length < 0) ? 0 : (size_t) length;

   // grow the buffer until enough bytes are available
   while (kstr_available(this) < count)
      if (kstr_grow(this) == NULL)
         return NULL;

   // append the formatted string
   va_copy(args_copy, args);
   vsnprintf(this->data + this->used - 1, count + 1, fmt, args_copy);
   va_end(args_copy);

   this->used += count;
   this->width += count;

   // invalidate the cached basename
   free(this->basename);
   this->basename = NULL;

   return this;
}

static
size_t
kstr_available(
      kstr * this)
{
   return this->data_size - this->used;
}

char const *
kstr_basename(
      kstr * this)
{
   // use the cached basename if available
   if (this->basename != NULL)
      return this->basename;

   // get a modifiable copy of the buffer
   char * data_copy;
   if ((data_copy = kstr_get_copy(this)) == NULL)
      return NULL;

   // calculate and cache the basename
   this->basename = strdup(basename(data_copy));
   free(data_copy);

   if (this->basename == NULL)
      kstr_abort(&this);

   return this->basename;
}

kstr *
kstr_copy(
      kstr * this)
{
   // allocate and initialize the string object
   kstr * this_copy;
   if ((this_copy = malloc(sizeof(*this_copy))) == NULL)
      return kstr_abort(&this);

   this_copy->basename = NULL;
   this_copy->data_size = this->data_size;
   this_copy->used = this->used;
   this_copy->width = this->width;

   // allocate and initialize the character buffer
   if ((this_copy->data = malloc(this_copy->data_size)) == NULL)
   {
      kstr_free(&this_copy);
      return kstr_abort(&this);
   }

   memcpy(this_copy->data, this->data, this_copy->data_size);
   return this_copy;
}

kstr *
kstr_free(
      kstr ** ptr)
{
   if (ptr == NULL)
      return NULL;

   // set the pointer's target to null
   kstr * this = *ptr;
   *ptr = NULL;
   if (this == NULL)
      return NULL;

   // free allocated memory
   free(this->data);
   free(this->basename);
   free(this);
   return NULL;
}

char const *
kstr_get(
      kstr * this)
{
   return this->data;
}

char *
kstr_get_copy(
      kstr * this)
{
   char * data_copy;
   if ((data_copy = strdup(this->data)) == NULL)
      kstr_abort(&this);

   return data_copy;
}

static
kstr *
kstr_grow(
      kstr * this)
{
   static size_t const grow_factor = 2;

   // safely calculate the size of the new buffer
   if (this->data_size == (size_t) -1)
      return kstr_abort(&this);

   size_t new_data_size;
   if (this->data_size > (size_t) -1 / grow_factor)
      new_data_size = (size_t) -1;
   else
      new_data_size = this->data_size * grow_factor;

   // grow the buffer
   char * new_data;
   if ((new_data = realloc(this->data, new_data_size)) == NULL)
      return kstr_abort(&this);

   this->data_size = new_data_size;
   this->data = new_data;
   return this;
}

kstr *
kstr_new(
      char const * text)
{
   static size_t const init_size = 64;

   // allocate and initialize the string object
   kstr * this;
   if ((this = malloc(sizeof(*this))) == NULL)
      return kstr_abort(&this);

   this->basename = NULL;
   this->data_size = init_size;
   this->used = 1;
   this->width = 0;

   // allocate and initialize the character buffer
   if ((this->data = malloc(this->data_size)) == NULL)
      return kstr_abort(&this);
   this->data[0] = '\0';

   if (kstr_add_text(this, text) == NULL)
      return kstr_abort(&this);

   return this;
}

static
kstr *
kstr_reset(
      kstr * this)
{
   // clear the string's value
   this->data[0] = '\0';
   this->used = 1;
   this->width = 0;

   // invalidate the cached basename
   free(this->basename);
   this->basename = NULL;

   return this;
}

kstr *
kstr_set_fmt(
      kstr * this,
      char const * fmt,
      ...)
{
   va_list args;
   va_start(args, fmt);
   this = kstr_set_vfmt(this, fmt, args);
   va_end(args);

   return this;
}

kstr *
kstr_set_text(
      kstr * this,
      char const * text)
{
   kstr_reset(this);
   return kstr_add_text(this, text);
}

kstr *
kstr_set_vfmt(
      kstr * this,
      char const * fmt,
      va_list args)
{
   kstr_reset(this);
   return kstr_add_vfmt(this, fmt, args);
}

size_t
kstr_size(
      kstr * this)
{
   return this->used;
}

size_t
kstr_width(
      kstr * this)
{
   return this->width;
}
