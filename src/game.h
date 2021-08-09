#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "sudoku.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_rows, std::size_t grid_columns, Sudoku::Difficulty diffic);
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;

 private:
  Sudoku sudoku_;

  int score_{0};
  bool isScoreUpdated_{false};

  void placeInitialNumbers();
  void update(bool &completed);
};

#endif