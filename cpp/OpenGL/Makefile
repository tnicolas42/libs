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
mkdir -p $(LIBS_DIR) $(LIBS_DIR)/glad $(LIBS_DIR)/KHR
wget -O $(LIBS_DIR)/glad/glad.h https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/glad/glad.h
wget -O $(LIBS_DIR)/glad/glad.c https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/glad/glad.c
wget -O $(LIBS_DIR)/KHR/khrplatform.h https://raw.githubusercontent.com/tnicolas42/libs/master/cpp/OpenGL/glad/KHR/khrplatform.h
wget -O $(LIBS_DIR)/stb_image.h https://raw.githubusercontent.com/nothings/stb/0224a44/stb_image.h
wget -O $(LIBS_DIR)/json.hpp https://raw.githubusercontent.com/nlohmann/json/8d92ca8/single_include/nlohmann/json.hpp
...
endef
export CONFIGURE
