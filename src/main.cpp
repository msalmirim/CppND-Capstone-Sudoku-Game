#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main()
{
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{700};
  constexpr std::size_t kGridWidth{640};
  constexpr std::size_t kGridHeight{640};
  constexpr std::size_t kGridRows{9};
  constexpr std::size_t kGridColumns{9};
  
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, kGridColumns, kGridRows);
  Controller controller;
  Game game(kGridWidth, kGridHeight, kGridRows, kGridColumns, Sudoku::Difficulty::sMedium);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  return 0;
}