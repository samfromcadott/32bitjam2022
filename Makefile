.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
cleansub:
	$(MAKE) clean
	rm -f witch.cue witch.bin

TARGET = witch

SRCS = src/main.c src/render.c src/input.c src/objects.h src/player.c src/util.c src/file.c src/texture.c

include common.mk
