/*
  LibRCD - Public Interface

  Copyright (C) 2005-2008 Suren A. Chilingaryan <csa@dside.dyndns.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as published
  by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#ifndef _LIBRCD_H
#define _LIBRCD_H

#ifdef __cplusplus
extern "C" {
#endif

enum rcd_russian_charset_t {
    RUSSIAN_CHARSET_WIN = 0,
    RUSSIAN_CHARSET_KOI,
    RUSSIAN_CHARSET_UTF8,
    RUSSIAN_CHARSET_ALT
};
typedef enum rcd_russian_charset_t rcd_russian_charset;

rcd_russian_charset (*rcdGetRussianCharset)(const char *buf, int len) = NULL;

#ifdef __cplusplus
}
#endif

#endif /* _LIBRCD_H */
