#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool init() {
	// SDL_Event *		_event = new SDL_Event();
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logErr("while loading OpenGL: " << SDL_GetError());
        SDL_Quit();
		return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// SDL_Window *	_win;
	_win = SDL_CreateWindow((_gameInfo->title + " OpenGL").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_gameInfo->width, _gameInfo->height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (_win == nullptr) {
        logErr("while loading OpenGL: " << SDL_GetError());
		return false;
	}

	// SDL_GLContext	_context;
	_context = SDL_GL_CreateContext(_win);
    if (_context == 0) {
        logErr("while loading OpenGL: " << SDL_GetError());
        return false;
    }

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        logErr("while loading OpenGL: failed to init glad");
        return false;
	}

	glEnable(GL_MULTISAMPLE);  // anti aliasing
	glEnable(GL_CULL_FACE);  // face culling
	glEnable(GL_BLEND);  // enable blending (used in textRender)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	try {
		// TextRender *		_textRender;
		_textRender = new TextRender(width, height);
		_textRender->loadFont("font", font, 25);
		// Skybox *			_skybox;
		_skybox = new Skybox;
	}
	catch (Shader::ShaderError & e) {
        logErr("while loading OpenGL: " << e.what());
		return false;
	}
	catch (TextRender::TextRenderError & e) {
        logErr("while loading OpenGL: " << e.what());
		return false;
	}

	// Camera *			_cam;
	_cam = new Camera();

	float angle = _cam->zoom;
	float ratio = static_cast<float>(_gameInfo->width) / _gameInfo->height;
	float nearD = 0.1f;
	float farD = 400;
	// glm::mat4			_projection;
	_projection = glm::perspective(glm::radians(angle), ratio, nearD, farD);

	_skybox->getShader().use();
	_skybox->getShader().setMat4("projection", _projection);
	_skybox->getShader().unuse();

    return true;
}

// call each frames
void updateInput() {
	uint64_t time = getMs().count();
	float dtTime = (time - _lastLoopMs) / 1000.0;
	_lastLoopMs = time;
	while (SDL_PollEvent(_event)) {
		if (_event->window.event == SDL_WINDOWEVENT_CLOSE)
			input.quit = true;
		if (_event->key.type == SDL_KEYDOWN) {
			// key down
			if (_event->key.keysym.sym == SDLK_ESCAPE)
				input.quit = true;
		}

		// mouse movement
		if (_event->type == SDL_MOUSEMOTION) {
			_cam->processMouseMovement(_event->motion.xrel, -_event->motion.yrel);
		}
	}

	const Uint8 * keystates = SDL_GetKeyboardState(NULL);

	// key repeat (moving)
	bool isRun = false;
	if (keystates[SDL_SCANCODE_LSHIFT])
		isRun = true;
	if (keystates[SDL_SCANCODE_W])
		_cam->processKeyboard(CamMovement::Forward, dtTime, isRun);
	if (keystates[SDL_SCANCODE_S])
		_cam->processKeyboard(CamMovement::Backward, dtTime, isRun);
	if (keystates[SDL_SCANCODE_A])
		_cam->processKeyboard(CamMovement::Left, dtTime, isRun);
	if (keystates[SDL_SCANCODE_D])
		_cam->processKeyboard(CamMovement::Right, dtTime, isRun);
	if (keystates[SDL_SCANCODE_E])
		_cam->processKeyboard(CamMovement::Up, dtTime, isRun);
	if (keystates[SDL_SCANCODE_Q])
		_cam->processKeyboard(CamMovement::Down, dtTime, isRun);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
    glClearColor(0.11373f, 0.17647f, 0.27059f, 1.0f);

	CAMERA_MAT4 view = _cam->getViewMatrix();

	CAMERA_MAT4	skyView = view;
	skyView[3][0] = 0;  // remove translation for the skybox
	skyView[3][1] = 0;
	skyView[3][2] = 0;
	_skybox->getShader().use();
	_skybox->getShader().setMat4("view", skyView);
	_skybox->getShader().unuse();

	// draw here !!!

	_skybox->draw(0.3);

	// text
    {
		int x = 20;
		int y = _gameInfo->height - _textBasicHeight - 10;
		int lineSz = _textBasicHeight * 1.2;
		std::string text;
		text = "text"
		_textRender->write("font", text, x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
		y -= lineSz;
		text = "text 2";
		_textRender->write("font", text, x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
	}

    SDL_GL_SwapWindow(_win);
	checkError();
}