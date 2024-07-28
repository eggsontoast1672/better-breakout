#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "glad/glad.h"

static const std::array<float, 16> s_vertices = {
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
    0.5f,  0.5f,  1.0f, 1.0f, // Top right
};

static const std::array<unsigned int, 6> s_indices = {
    0, 1, 3, 0, 3, 2,
};

static const char *const vertex_source = R"glsl(
  #version 330 core

  layout (location = 0) in vec2 position;
  layout (location = 1) in vec2 tex_coord;

  out vec2 v_tex_coord;

  void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    v_tex_coord = tex_coord;
  }
)glsl";

static const char *const fragment_source = R"glsl(
  #version 330 core

  in vec2 v_tex_coord;

  out vec4 f_color;

  uniform sampler2D u_texture;

  void main() {
    f_color = texture(u_texture, v_tex_coord);
  }
)glsl";

std::uint32_t get_pixel(const SDL_Surface *surface, int x, int y) {
  // I'm not sure if this is going to be the case, but I'm assuming it is since
  // we're loading PNG data.
  assert(surface->format->BytesPerPixel == 4);
  std::uint8_t *p = reinterpret_cast<std::uint8_t *>(surface->pixels) + x * 4 +
                    y * surface->pitch;
  return *reinterpret_cast<std::uint32_t *>(p);
}

struct ImageData {
  std::vector<std::uint8_t> bytes;
  GLsizei width;
  GLsizei height;
};

ImageData load_png_data(const std::string &path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == nullptr) {
    std::cerr << "Failed to load image data: " << IMG_GetError() << '\n';
    return {};
  }

  std::vector<std::uint8_t> bytes;

#define LOAD_PNG_DATA_LOOP_BACKWARD 1
#if LOAD_PNG_DATA_LOOP_BACKWARD
  for (int y = surface->h - 1; y >= 0; y--) {
    for (int x = 0; x < surface->w; x++) {
      std::uint32_t color = get_pixel(surface, x, y);
      std::uint8_t r, g, b, a;
      SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
      bytes.push_back(r);
      bytes.push_back(g);
      bytes.push_back(b);
      bytes.push_back(a);
    }
  }
#else
  for (int y = 0; y < surface->h; y++) {
    for (int x = 0; x < surface->w; x++) {
      std::uint32_t color = get_pixel(surface, x, y);
      std::uint8_t r, g, b, a;
      SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
      bytes.push_back(r);
      bytes.push_back(g);
      bytes.push_back(b);
      bytes.push_back(a);
    }
  }
#endif

  return {bytes, surface->w, surface->h};
}

// Don't change the signature of main even though argc and argv aren't used. SDL
// needs this on windows. FML.
int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
  }

  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    std::cerr << "Failed to initialize SDL image: " << IMG_GetError() << '\n';
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_Window *window =
      SDL_CreateWindow("Better Breakout", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << '\n';
  }

  if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
    std::cerr << "Failed to initialize glad\n";
  }

  // Debug output some crap

  std::cerr << "Vendor: " << glGetString(GL_VENDOR) << '\n';
  std::cerr << "Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
  std::cerr << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  // Triangle stuff

  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, s_vertices.size() * sizeof(float),
               s_vertices.data(), GL_STATIC_DRAW);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indices.size() * sizeof(unsigned int),
               s_indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(2 * sizeof(float)));

  // Shader stuff

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
  glCompileShader(vertex_shader);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
  glCompileShader(fragment_shader);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);

  glDeleteShader(fragment_shader);
  glDeleteShader(vertex_shader);

  glUseProgram(program);

  ImageData data = load_png_data("res/textures/awesomeface.png");

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data.bytes.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // I think 0 is the default??
  glUniform1i(glGetUniformLocation(program, "u_texture"), 0);

  // TODO: Error handling

  glClearColor(0.53f, 0.91f, 0.28f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  bool window_should_close = false;
  while (!window_should_close) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        window_should_close = true;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   reinterpret_cast<void *>(0));
    SDL_GL_SwapWindow(window);
  }

  glDeleteProgram(program);
  glDeleteBuffers(1, &index_buffer);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteVertexArrays(1, &vertex_array);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
