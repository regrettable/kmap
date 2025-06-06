LIBS=-lGL -lm -lX11 -ldl
WLIBS=-lm -lopengl32 -lglu32 -lgdi32 -ldbghelp
WARN=-Wall -Wextra -Wno-unused -Wno-discarded-qualifiers
KMDB_CURRENT_VERSION="KMDB_MAKE_VERSION(0, 2)"
OPENGL_MAJOR_RELEASE=4
OPENGL_MINOR_RELEASE=2
# BUILD_INTERNAL: developer only, executable split into 2 files and dynamic reloading is available; dynamic asserts are activated
# DEBUG_INFO: contains debug information related to timings
GENERAL_FLAGS=-DOPENGL_MAJOR_RELEASE=$(OPENGL_MAJOR_RELEASE) -DOPENGL_MINOR_RELEASE=$(OPENGL_MINOR_RELEASE) -DKMDB_CURRENT_VERSION=$(KMDB_CURRENT_VERSION) -DSHADERS_PATH=\"../shaders/\" -gdwarf-3
RELEASE_FLAGS=-DBUILD_INTERNAL=0 -DDEBUG_INFO=0 # -Ofast
INTERNAL_FLAGS=-DBUILD_INTERNAL=1 -DDEBUG_INFO=1
CC=gcc
WCC=x86_64-w64-mingw32-gcc
ALLFLAGS=$(GENERAL_FLAGS) $(INTERNAL_FLAGS)
COMPILE_INTERNAL=$(CC) $(ALLFLAGS) $(WARN)
WCOMPILE_INTERNAL=$(WCC) $(ALLFLAGS) $(WARN)
DATABASE_URL=https://github.com/
CURRENT_DATE=$(shell date +%d-%m-%Y)
MANPATH=/usr/local/man/man1
INSTALL_PATH=/usr/local/bin

.PHONY: all wall install uninstall

all: build/x11_debug build/kmap.so build/converter build/win_debug.exe build/kmap.dll #build/release.exe 

build/x11_debug: x11_main.c x11_main.h platform_config.h kcommon.h
	$(COMPILE_INTERNAL) $< -o $@ $(LIBS)

build/win_debug.exe: win_main.c win_main.h platform_config.h kcommon.h
	$(WCOMPILE_INTERNAL) $< -o $@ $(WLIBS)

install: build/release kmap.1
	cp build/release $(INSTALL_PATH)/kmap
	sed 's?CURRENT_DATE?$(CURRENT_DATE)?g' kmap.1 | \
		sed 's?DATABASE_URL?$(DATABASE_URL)?g' | \
		sed 's?OPENGL_MAJOR_RELEASE?$(OPENGL_MAJOR_RELEASE)?g' | \
		sed 's?OPENGL_MINOR_RELEASE?$(OPENGL_MINOR_RELEASE)?g' > \
		$(MANPATH)/kmap.1
	chmod 644 $(MANPATH)/kmap.1

uninstall:
	rm -rf $(INSTALL_PATH)/kmap

# Important: if you've changed any shaders, don't forget to update the database for the release build
# by running make build/db.h
build/release: x11_main.c x11_main.h kmap.c kmap.h kcommon.h kmdb.h kmath.h platform_config.h theme.h build/db.h
	$(CC) $(GENERAL_FLAGS) $(RELEASE_FLAGS) $(WARN) $(LIBS) $< -o $@

build/release.exe: win_main.c win_main.h kmap.c kmap.h kcommon.h kmdb.h kmath.h platform_config.h theme.h build/db.h
	$(WCC) $(GENERAL_FLAGS) $(RELEASE_FLAGS) $(WARN) $< -o $@ $(WLIBS)

build/db.h: dicts/JMdict_e dicts/kanjidic2.xml dicts/kradfile_utf8 dicts/freq_list dicts/jyouyou__strokeEditDistance.csv build/converter
	build/converter -j $(word 1,$^) -k $(word 2,$^) -r $(word 3,$^) -f $(word 4,$^) -y $(word 5,$^) -s ./shaders -o $@

build/converter: converter.c converter.h kmdb.h
	$(COMPILE_INTERNAL) $< -o $@ $(LIBS)

build/kmap.so: kmap.c kmap.h kcommon.h kmdb.h kmath.h theme.h build/db.h
	$(COMPILE_INTERNAL) --shared -fPIC $< -o $@

build/kmap.dll: kmap.c kmap.h kcommon.h kmdb.h kmath.h theme.h build/db.h
	$(WCOMPILE_INTERNAL) -static --shared $< -o $@ $(WLIBS)

clean cln:
	rm -f build/*
