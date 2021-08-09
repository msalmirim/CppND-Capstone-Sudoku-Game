#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sudoku.h"

class Controller {
 public:
  void HandleInput(bool &running, Sudoku &sudoku);
  void SetFirstSelectionPosition(Sudoku &sudoku);

 private:
  int sel_x {0};
  int sel_y {0};
  void HandleKeys(SDL_Event &e, Sudoku &sudoku);
  void SetSelectedCell(Sudoku &sudoku, const bool select);


};

#endif