#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "sudoku.h"

void Controller::SetFirstSelectionPosition(Sudoku &sudoku)
{
  bool firstSelectedCell = false;

  for(int row = 0; row < sudoku.getGridRows();  row++)
  {
    for(int col = 0; col < sudoku.getGridColumns(); col++)
    {
      if (sudoku.sudokuGrid[row][col].isEditable())
      {
        firstSelectedCell = true;
        sudoku.sudokuGrid[row][col].setSelected(true);
        sel_x = col;
        sel_y = row;
        break;
      }
    }
    if (firstSelectedCell)
      break;
  } 
}


void Controller::HandleKeys(SDL_Event &e, Sudoku &sudoku)
{
  int sym = e.key.keysym.sym;
  switch (sym) {
    case SDLK_UP:
    {
      sel_y--;
      if (sel_y < 0)
        sel_y = 9;
      break;
    }
    case SDLK_DOWN:
    {
      sel_y++;
      if (sel_y > 9)
        sel_y = 0;
      break;
    }
    case SDLK_LEFT:
    {
      sel_x--;
      if ((sel_x < 0) && (sel_y < 9))
      {
        sel_x = 8;
      }
      else if (sel_y == 9) // last line : buttons check and new
      {
        if (sel_x <= 4)
          sel_x = 6;
        else if (sel_x >= 5)
          sel_x = 0;
      }
      break;
    }
    case SDLK_RIGHT:
    {
      sel_x++;
      if ((sel_x > 8) && (sel_y < 9))
      {
        sel_x = 0;
      }
      else if (sel_y == 9) // last line : buttons check and new
      {
        if (sel_x < 5) // button check
          sel_x = 6;
        else if (sel_x >= 5) // button new
          sel_x = 0;
      } 
      break;
    }
    case SDLK_KP_1: case SDLK_KP_2: case SDLK_KP_3:
    case SDLK_KP_4: case SDLK_KP_5: case SDLK_KP_6:
    case SDLK_KP_7:	case SDLK_KP_8:	case SDLK_KP_9:
    {
      if (sudoku.sudokuGrid[sel_y][sel_x].isEditable())
      {
        int prevDispNum = sudoku.sudokuGrid[sel_y][sel_x].getDisplayNum();
        int num = sym - SDLK_KP_1 + 1;
        if (prevDispNum == sym)
          break;
        sudoku.sudokuGrid[sel_y][sel_x].setDisplayNum(num);
      }
      break;
    }
    case SDLK_BACKSPACE:
    {
      if (sudoku.sudokuGrid[sel_y][sel_x].isEditable())
      {
        int prevDispNum = sudoku.sudokuGrid[sel_y][sel_x].getDisplayNum();
        if (prevDispNum == 0)
          break;
        sudoku.sudokuGrid[sel_y][sel_x].setDisplayNum(0);
      }
      break;
    }
    case SDLK_RETURN:
    {
      if (sel_y == 9)
      {
        if (sel_x < 5) // button check pressed
          sudoku.setCheckSolution(true);
        else // button new pressed
          sudoku.setGenerateNewSudoku(true);
      }
    }
  }
}

void Controller::SetSelectedCell(Sudoku &sudoku, const bool select)
{
  if (sel_y == 9)
  {
    if (sel_x < 5)
      sudoku.setCheckSelected(select);
    else if (sel_x >= 5)
      sudoku.setGenerateNewSelected(select);
  }
  else
  {
    sudoku.sudokuGrid[sel_y][sel_x].setSelected(select);
  }
}

void Controller::HandleInput(bool &running, Sudoku &sudoku)
{
  // Enable text input
  SDL_StartTextInput();

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      running = false;
    }   
    else if (e.type == SDL_KEYDOWN)
    {
       SetSelectedCell(sudoku, false);
       HandleKeys(e, sudoku);
       SetSelectedCell(sudoku, true);
    }
  }  
}