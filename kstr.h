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
//! kstr string library interface
//!
//! kstr implements a string object and associated methods. a string manages
//! its own memory, grows dynamically as text is added to it, supports embedded
//! ansi terminal control codes for colorization and styling, and tracks its
//! width (visible character count) and size (number of bytes used)
//! independently.
//!
//! the api deals with opaque pointers to ::kstr objects, which must be created
//! with kstr_new() and destroyed with kstr_free().
//!
//! any method that modifies a string will raise `SIGABRT` if memory allocation
//! fails. if the signal is caught and handled, the method will destroy the
//! string with kstr_free() and return a null pointer.

#ifndef kstr_h
#define kstr_h

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

//! string object type
typedef struct kstr kstr;

//! string colors
typedef enum kstr_color
{
   kstr_color_default, //!< default color
   kstr_color_black, //!< black
   kstr_color_blue, //!< blue
   kstr_color_cyan, //!< cyan
   kstr_color_green, //!< green
   kstr_color_magenta, //!< magenta
   kstr_color_red, //!< red
   kstr_color_white, //!< white
   kstr_color_yellow, //!< yellow
   kstr_num_colors //!< symbolic number of enumerators
} kstr_color;

//! create a new string
//!
//! allocates memory for the string object. if \a text is not a null pointer,
//! the string is initialized with the given value; otherwise, it is
//! initialized with an empty value. the returned string must be destroyed with
//! kstr_free() when it is no longer needed.
//!
//! \param text initial value
//!
//! \return a new string
kstr * kstr_new(char const * text);

//! destroy a string
//!
//! if \a ptr is not null, the string it points to is destroyed and is set to
//! `NULL`. all memory used by the string is freed, and any pointers to it are
//! no longer valid.
//!
//! \param ptr string pointer
//!
//! \return `NULL`
kstr * kstr_free(kstr ** ptr);

//! create a copy of a string
//!
//! allocates memory for a new string object that is identical to \a this. the
//! returned string must be destroyed with kstr_free() when it is no longer
//! needed.
//!
//! \param this string
//!
//! \return a copy of the string
kstr * kstr_copy(kstr * this);

//! set a string's value
//!
//! if \a text is not a null pointer, the string is changed to use the given
//! value; otherwise, it is changed to use an empty value.
//!
//! \param this string
//! \param text new value
//!
//! \return \a this
kstr * kstr_set_text(kstr * this, char const * text);

//! format a string's value
//!
//! the string is changed to use a value calculated from the given
//! *printf()* -style format string and its arguments.
//!
//! \param this string
//! \param fmt format string
//! \param ... format string arguments
//!
//! \return \a this
kstr * kstr_set_fmt(kstr * this, char const * fmt, ...)
#ifndef kstr_stdc
   __attribute__((format(printf, 2, 3)))
#endif
;

//! format a string's value using an arg list
//!
//! the string is changed to use a value calculated from the given
//! *printf()* -style format string and its arguments.
//!
//! \param this string
//! \param fmt format string
//! \param args format string arguments
//!
//! \return \a this
kstr * kstr_set_vfmt(kstr * this, char const * fmt, va_list args)
#ifndef kstr_stdc
   __attribute__((format(printf, 2, 0)))
#endif
;

//! add text to a string
//!
//! if \a text is not a null pointer, its contents are appended to the string's
//! value.
//!
//! \param this string
//! \param text additional value
//!
//! \return \a this
kstr * kstr_add_text(kstr * this, char const * text);

//! add formatted text to a string
//!
//! the given *printf()* -style format string and its arguments are used to
//! append text to the string's value.
//!
//! \param this string
//! \param fmt format string
//! \param ... format string arguments
//!
//! \return \a this
kstr * kstr_add_fmt(kstr * this, char const * fmt, ...)
#ifndef kstr_stdc
   __attribute__((format(printf, 2, 3)))
#endif
;

//! append text to a string's value using a format string and arg list
//!
//! the given *printf()* -style format string and its arguments are used to
//! append text to the string's value.
//!
//! \param this string
//! \param fmt format string
//! \param args format string arguments
//!
//! \return \a this
kstr * kstr_add_vfmt(kstr * this, char const * fmt, va_list args)
#ifndef kstr_stdc
   __attribute__((format(printf, 2, 0)))
#endif
;

//! add a control code to set or clear the bold text attribute
//!
//! if \a bold is true, the ansi control code for enabling bold text is
//! appended to the string's value; otherwise, the ansi control code for
//! disabling bold text is appended. this does not increase the string's width,
//! as control codes are not considered visible text.
//!
//! \param this string
//! \param bold enable bold if true, disable bold if false
//!
//! \return \a this
kstr * kstr_add_bold(kstr * this, bool bold);

//! add a control code to set the foreground color
//!
//! appends the ansi control code for enabling the foreground color indicated
//! by \a color to the string's value. this does not increase the string's
//! width, as control codes are not considered visible text.
//!
//! \param this string
//! \param color foreground color
//!
//! \return \a this
kstr * kstr_add_fg(kstr * this, kstr_color color);

//! add a control code to set the background color
//!
//! appends the ansi control code for enabling the background color indicated
//! by \a color to the string's value. this does not increase the string's
//! width, as control codes are not considered visible text.
//!
//! \param this string
//! \param color background color
//!
//! \return \a this
kstr * kstr_add_bg(kstr * this, kstr_color color);

//! add a control code to reset all text attributes
//!
//! appends the ansi control code for turning off all attributes to the
//! string's value. this does not increase the string's width, as control codes
//! are not considered visible text.
//!
//! \param this string
//!
//! \return \a this
kstr * kstr_add_reset(kstr * this);

//! get the width of a string's value
//!
//! returns the width of the string's value in bytes. the width does not
//! include the nul terminator or any control codes added with kstr_add_bold(),
//! kstr_add_fg(), or kstr_add_bg().
//!
//! \param this string
//!
//! \return the width in bytes
size_t kstr_width(kstr * this);

//! get the size of a string's value
//!
//! returns the number of bytes used by the string's value. this does not
//! represent the actual size of the buffer used to store the string, only the
//! number of bytes used (up to and including the nul terminator).
//!
//! \param this string
//!
//! \return the size in bytes
size_t kstr_size(kstr * this);

//! get a string's value
//!
//! returns the string's value, which is guaranteed to be nul-terminated. the
//! returned pointer becomes invalid if the string is modified or destroyed.
//!
//! \param this string
//!
//! \return the string's value
char const * kstr_get(kstr * this);

//! create a copy of a string's value
//!
//! allocates and returns a copy of the string's value, which is guaranteed to
//! be nul-terminated. the returned pointer must be destroyed with `free()`
//! when it is no longer needed.
//!
//! \param this string
//!
//! \return a copy of the string's value
char * kstr_get_copy(kstr * this);

//! get the basename of a string's value
//!
//! calculates and returns the basename of the string's value (see the standard
//! `basename()` function). the returned pointer becomes invalid if the string
//! is modified or destroyed.
//!
//! \param this string
//!
//! \return the basename of the string's value
char const * kstr_basename(kstr * this);

#endif
