#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#ifdef _MSC_VER
#define snprintf _snprintf
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
  static void error_callback(int err, const char *msg);
private:
  GLFWwindow *window_;
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

  while (!glfwWindowShouldClose(window_)) {
    int w, h, w2, h2;
    glfwGetWindowSize(window_, &w, &h);
    glfwGetFramebufferSize(window_, &w2, &h2);
    float pixel_ratio = static_cast<float>(w2) / w;
    glViewport(0, 0, w2, h2);

    glClearColor(0.3, 0.3, 0.32, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg_, w, h, pixel_ratio, NVG_STRAIGHT_ALPHA);

    nvgFontSize(vg_, 96);
    nvgFontFace(vg_, "font");
    nvgFillColor(vg_, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(vg_, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    char buf[256];
    snprintf(buf, sizeof buf, "%dx%d %dx%d", w, h, w2, h2);
    nvgText(vg_, 10, 10, buf, NULL);
    nvgText(vg_, 100, 100, "Hello! こんにちは！", NULL);

    nvgEndFrame(vg_);

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  return 0;
}

bool App::Initialize(int argc, char *argv[])
{
  if (!glfwInit()) {
    printf("glfwInit() failed\n");
    return false;
  }

  glfwSetErrorCallback(error_callback);
#ifndef _WIN32
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  window_ = glfwCreateWindow(1000, 600, "nanovg example", NULL, NULL);
  if (window_ == NULL) {
    printf("glfwCreateWindow() failed\n");
    return false;
  }

  glfwMakeContextCurrent(window_);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("glewInit() failed\n");
    return false;
  }
  glGetError();

  vg_ = nvgCreateGL3(512, 512, NVG_ANTIALIAS);
  if (vg_ == NULL) {
    printf("nvgCreateGL3() failed\n");
    return false;
  }

  font_ = nvgCreateFont(vg_, "font", "../../font/mplus-1p-regular.ttf");
  if (font_ < 0) {
    printf("nvgCreateFont() failed\n");
    return false;
  }

  glfwSwapInterval(0);

  return true;
}

void App::Finalize()
{
  if (vg_ != NULL) {
    nvgDeleteGL3(vg_);
    vg_ = NULL;
  }

  if (window_ != NULL) {
    glfwDestroyWindow(window_);
    window_ = NULL;
  }

  glfwTerminate();
}

void App::error_callback(int err, const char *msg)
{
  printf("GLFW error %d: %s\n", err, msg);
}

} // nanovg_example

int main(int argc, char *argv[])
{
  nanovg_example::App app;
  return app.Run(argc, argv);
}
