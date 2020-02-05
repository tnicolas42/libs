# project global config
CFLAGS = -Ofast -std=c++11 -Wall -Wextra -Wno-deprecated

# libs configuration
LIBS_SRC_C		= glad/glad.c \

LIBS_HEAD		= glad/glad.h \
				  KHR/khrplatform.h \
				  stb_image.h \

LIBS_FLAGS			= -rpath ~/.brew/lib -L ~/.brew/lib -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_gfx \
					  -L ~/.brew/opt/freetype/lib -lfreetype

LIBS_FLAGS_OSX		= -framework OpenGL

LIBS_FLAGS_LINUX	= -lGL -lGLU

LIBS_INC			= ~/.brew/include \
					  /usr/local/opt/freetype/include/freetype2 \
					  ~/.brew/opt/freetype/include/freetype2 \
					  /usr/include/freetype2

# configure file
define CONFIGURE
...
# OSX
brew install sdl2 sdl2_ttf sdl2_image SDL2_gfx
mkdir -p $(LIBS_DIR)
wget -O $(LIBS_DIR)/stb_image.h https://raw.githubusercontent.com/nothings/stb/0224a44/stb_image.h
...
endef
export CONFIGURE
