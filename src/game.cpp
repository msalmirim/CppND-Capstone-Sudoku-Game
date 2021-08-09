#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_rows, std::size_t grid_columns, Sudoku::Difficulty difficulty)
    : sudoku_(grid_width, grid_height, grid_rows, grid_columns, difficulty)
{
      placeInitialNumbers();
}

void Game::placeInitialNumbers()
{
  sudoku_.GenerateGrid();
}

void Game::update(bool &completed)
{
  sudoku_.Update(completed);
  if ((completed) && (!isScoreUpdated_))
  {
    score_++;
    isScoreUpdated_ = true;
  }
  else if ((!completed) && (isScoreUpdated_))
  {
    isScoreUpdated_ = false;
  }
}

void Game::Run(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool completed = false;

  controller.SetFirstSelectionPosition(sudoku_);

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, sudoku_);
    update(completed);
    renderer.Render(sudoku_, completed);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score_);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

int Game::GetScore() const { return score_; }