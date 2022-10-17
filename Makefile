.PHONY: all cleansub
all:
	mkpsxiso -y isoconfig.xml
cleansub:
	$(MAKE) clean
	rm -f witch.cue witch.bin

TARGET = witch

# SRCS = src/main.c src/init.c src/render.c
SRCS = src/main.c \

include common.mk
