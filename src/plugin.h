/*
  LibRCC - plugin's abstraction

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

#ifndef _RCC_PLUGIN_H
#define _RCC_PLUGIN_H
#include "../config.h"


#ifdef HAVE_DLOPEN
# define RCC_PLUGINS
#endif /* HAVE_DLOPEN */

typedef void *rcc_library_handle;

rcc_library_handle rccLibraryOpen(char *filename);
void rccLibraryClose(rcc_library_handle handle);
void* rccLibraryFind(rcc_library_handle handle, const char *symbol);


typedef enum rcc_plugin_type_t {
    RCC_PLUGIN_TYPE_SYSTEMLIB = 0,
    RCC_PLUGIN_TYPE_ENGINE,
    RCC_PLUGIN_TYPE_MAX
} rcc_plugin_type;

struct rcc_plugin_handle_t {
    char *sn;
    rcc_library_handle handle;
    void *info_function;
//    rcc_library_type type;
};

typedef struct rcc_plugin_handle_t rcc_plugin_handle_s;
typedef struct rcc_plugin_handle_t *rcc_plugin_handle;

int rccPluginInit();
void rccPluginFree();

rcc_plugin_handle rccPluginLoad(rcc_plugin_type type, const char *name);
rcc_engine *rccPluginEngineGetInfo(const char *name, const char *language);

#endif /* _RCC_PLUGIN_H */
