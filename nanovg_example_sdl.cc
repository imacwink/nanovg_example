#include <glew.h>
#include <SDL.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#ifdef _MSC_VER
#define snprintf _snprintf
#define FONT_PATH "../../font/mplus-1p-regular.ttf"
#else
#define FONT_PATH "font/mplus-1p-regular.ttf"
#endif

namespace nanovg_example {

class App
{
public:
  App();
  ~App();
  int Run(int argc, char *argv[]);
protected:
  bool Initialize(int argc, char *argv[]);
  void Finalize();
private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  SDL_GLContext gl_;
  NVGcontext *vg_;
  int font_;
};

App::App()
  : window_(NULL)
  , vg_(NULL)
{
}

App::~App()
{
  Finalize();
}

int App::Run(int argc, char *argv[])
{
  if (!Initialize(argc, argv)) {
    return 1;
  }

  bool done = false;
  while (!done) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT ||
	  (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) ||
	  (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_q && (ev.key.keysym.mod == KMOD_LGUI || ev.key.keysym.mod == KMOD_RGUI))) {
	done = true;
      }
    }

    int w, h, w2, h2;
    SDL_GetWindowSize(window_, &w, &h);
    SDL_GL_GetDrawableSize(window_, &w2, &h2);
    float pixel_ratio = static_cast<float>(w2) / w;
    glViewport(0, 0, w2, h2);

    glClearColor(0.3, 0.3, 0.32, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg_, w, h, pixel_ratio);

    nvgFontSize(vg_, 96);
    nvgFontFace(vg_, "font");
    nvgFillColor(vg_, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(vg_, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    char buf[256];
    snprintf(buf, sizeof buf, "%dx%d %dx%d", w, h, w2, h2);
    nvgText(vg_, 10, 10, buf, NULL);
    nvgText(vg_, 100, 100, "Hello! こんにちは！", NULL);

    nvgEndFrame(vg_);

    SDL_GL_SwapWindow(window_);
  }

  return 0;
}

bool App::Initialize(int argc, char *argv[])
{
  int ret;

  ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) {
    printf("SDL_Init() failed: %s\n", SDL_GetError());
    return false;
  }

#ifndef _WIN32
  ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
  ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
  ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
  ret = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
  ret = SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
  ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  if (ret != 0) {
    printf("SDL_GL_SetAttribute() failed: %s\n", SDL_GetError());
    return false;
  }
#endif

  window_ = SDL_CreateWindow("nanovg example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
  if (window_ == NULL) {
    printf("SDL_CraeteWindow() failed: %s\n", SDL_GetError());
    return false;
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == NULL) {
    printf("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
    return false;
  }

  gl_ = SDL_GL_CreateContext(window_);
  if (gl_ == NULL) {
	  printf("SDL_GL_CreateContext() failed: %s\n", SDL_GetError());
	  return false;
  }

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("glewInit() failed.\n");
    return false;
  }
  glGetError();

  vg_ = nvgCreateGL3(NVG_ANTIALIAS);
  if (vg_ == NULL) {
    printf("nvgCreateGL3() failed.\n");
    return false;
  }

  font_ = nvgCreateFont(vg_, "font", FONT_PATH);
  if (font_ < 0) {
    printf("nvgCreateFont() failed.\n");
    return false;
  }

  SDL_GL_SetSwapInterval(1);

  SDL_ShowWindow(window_);

  return true;
}

void App::Finalize()
{
  if (vg_ != NULL) {
    nvgDeleteGL3(vg_);
    vg_ = NULL;
  }

  if (gl_ != NULL) {
    SDL_GL_DeleteContext(gl_);
    gl_ = NULL;
  }

  if (renderer_ != NULL) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = NULL;
  }

  if (window_ != NULL) {
    SDL_DestroyWindow(window_);
    window_ = NULL;
  }

  SDL_Quit();
}

} // nanovg_example

int main(int argc, char *argv[])
{
  nanovg_example::App app;
  return app.Run(argc, argv);
}
