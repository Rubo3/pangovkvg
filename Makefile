PANGOCAIRO_REQUIRED=$(shell pkgconf --print-requires --print-requires-private pangocairo | awk '{print $$1}')
PANGOCAIRO_REQUIRED_FLAGS=$(shell pkgconf --cflags --libs $(PANGOCAIRO_REQUIRED))
PANGOCAIRO_FLAGS=$(shell pkgconf --cflags --libs pangocairo)
VKVG_FLAGS=$(shell pkgconf --cflags --libs vkvg)

INCLUDE=-Ipangovkvg/pango -Ipango -Ipango/build -Ipango/pango

all: myproj

config.h:
	meson pango/build pango

myproj: config.h
	cc $(INCLUDE) $(PANGOCAIRO_REQUIRED_FLAGS) $(PANGOCAIRO_FLAGS) $(VKVG_FLAGS)
