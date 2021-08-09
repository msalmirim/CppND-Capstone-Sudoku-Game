#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "sudoku.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width,
           const std::size_t screen_height,
           const std::size_t grid_width,
           const std::size_t grid_height,
           const std::size_t grid_columns,
           const std::size_t grid_rows);
  ~Renderer();

  void Render(Sudoku &sudoku, bool &completed);
  void UpdateWindowTitle(int score);

 private:
  SDL_Window* sdl_window_;
  SDL_Renderer* sdl_renderer_;
  TTF_Font* sdl_font_;
  SDL_Texture* sdl_textures_[14];
  
  int sdl_font_size_;
  const std::size_t screen_width_;
  const std::size_t screen_height_;
  const std::size_t grid_width_;
  const std::size_t grid_height_;
  const std::size_t grid_columns_;
  const std::size_t grid_rows_;

  void init_SDL();
  void defineTextures();
  void loadTexture(const char* text, SDL_Color& fontColor, SDL_Texture*& texture);
  void drawColorCell(SudokuCell &cell, SDL_Rect &cellRect);

};

#endif