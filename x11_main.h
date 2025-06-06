#ifndef X11_MAIN_H
#define X11_MAIN_H

#include "kcommon.h"

#include <unistd.h>
#include <poll.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#if BUILD_INTERNAL
#include <sys/inotify.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#if BUILD_INTERNAL
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glu.h>
#else
#include "kmap.c"
#include <GL/glx.h>
#endif

#include "platform_config.h"
#include "gl_helpers.h"

#endif // X11_MAIN_H
