#include "sudoku.h"
#include <cmath>
#include <iostream>
#include <algorithm>

SudokuCell::SudokuCell()
  : editable_(false),
    selected_(false),
    solutionNum_(0),
    displayNum_(0)
    {};

bool SudokuCell::operator==(const int &num)
{
  return (displayNum_ == num);
}

bool SudokuCell::cellNumVerification() const
{
  if (displayNum_ == solutionNum_)
    return true;
  else 
    return false;
}

Sudoku::Sudoku(std::size_t grid_width, std::size_t grid_height, std::size_t grid_rows, std::size_t grid_columns, Difficulty diffic)
  : grid_width_(grid_width),
    grid_height_(grid_height),
    grid_rows_ (grid_rows),
    grid_columns_(grid_columns),
    difficulty_(diffic),
    engine_(dev_()),
    checkSolution_(false),  
    checkSelected_(false),
    generateNewSudoku_{false},
    newGridSelected_(false)
{
  initGrid();
}

Sudoku::Sudoku(const Sudoku &sudoku)
{
  grid_width_ = sudoku.grid_width_;
  grid_height_ = sudoku.grid_height_;
  grid_rows_ = sudoku.grid_rows_;
  grid_columns_ = sudoku.grid_columns_;
  difficulty_ = sudoku.difficulty_;
  std::copy(sudoku.sudokuGrid.begin(),sudoku.sudokuGrid.end(), back_inserter(sudokuGrid));
  std::copy(sudoku.positionGrid_.begin(),sudoku.positionGrid_.end(), back_inserter(positionGrid_));
  std::copy(sudoku.guessNum_.begin(),sudoku.guessNum_.end(), back_inserter(guessNum_));
  std::cout << "Copy constructor called \n";
}

Sudoku &Sudoku::operator=(const Sudoku &sudoku)
{
  if (this == &sudoku)
    return *this;
  grid_width_ = sudoku.grid_width_;
  grid_height_ = sudoku.grid_height_;
  grid_rows_ = sudoku.grid_rows_;
  grid_columns_ = sudoku.grid_columns_;
  difficulty_ = sudoku.difficulty_;
  std::copy(sudoku.sudokuGrid.begin(),sudoku.sudokuGrid.end(), back_inserter(sudokuGrid));
  std::copy(sudoku.positionGrid_.begin(),sudoku.positionGrid_.end(), back_inserter(positionGrid_));
  std::copy(sudoku.guessNum_.begin(),sudoku.guessNum_.end(), back_inserter(guessNum_));

  std::cout << "Copy assignment Operator called \n";
  return *this;
}

void Sudoku::initGrid()
{
  positionGrid_.clear();
  guessNum_.clear();
  sudokuGrid.clear();

  // Randomly shuffling the array of grid positions
  for (int counter = 0; counter < grid_rows_ * grid_columns_ ; counter++)
  {
    positionGrid_.emplace_back(counter);
  }

  std::shuffle(positionGrid_.begin(), positionGrid_.end(), engine_);

  // Randomly shuffling the guessing number array
  for (int counter = 1; counter < 10 ; counter++)
  {
    guessNum_.emplace_back(counter);
  }

  std::shuffle(guessNum_.begin(), guessNum_.end(), engine_); 

  // Initialising the grid
  for (int row = 0; row < grid_rows_; row++)
  {
    std::vector<SudokuCell> VecCel;
    for (int col = 0; col < grid_columns_; col++)
    {
      SudokuCell cell;
      VecCel.emplace_back(cell);
    }
    sudokuGrid.emplace_back(VecCel);
  }
}

bool Sudoku::findUnassignedCell(int &unassignedCellRowindex, int &unassignedCellColindex) const
{
  for (int row = 0; row < grid_rows_; row++)
  {
    for (int col = 0; col < grid_columns_; col++)
    {
      if (sudokuGrid[row][col].getDisplayNum() == 0)
      {
        unassignedCellRowindex = row;
        unassignedCellColindex = col;
        return true;
      }
    }
  }
  return false;
}

/* Returns a boolean which indicates whether the checked number is already
 * assigned to a cell in the current column.
 */
bool Sudoku::usedInCol(const int &row, const int &num)
{
  bool isUsed = false;
  if (std::find(sudokuGrid[row].begin(), sudokuGrid[row].end(), num) != sudokuGrid[row].end())
  {
    isUsed = true;
  }
  return isUsed;
}

void Sudoku::getColumn(std::vector<SudokuCell> &sudokuCol, const int &col)
{
  for (int rowIdx = 0; rowIdx < grid_rows_; rowIdx++)
  {
    for (int colIdx = 0; colIdx < grid_columns_; colIdx++)
    {
      if (colIdx == col)
      {
        sudokuCol.emplace_back(sudokuGrid[rowIdx][colIdx]);
        break;
      }
    }
  }
}

/* Returns a boolean which indicates whether the checked number is already
 * assigned to a cell in the current row.
 */
bool Sudoku::usedInRow(const int &column, const int &num)
{
  bool isUsed = false;
  std::vector<SudokuCell> sudokuCol;
  getColumn(sudokuCol, column);

  if (std::find(sudokuCol.begin(), sudokuCol.end(), num) != sudokuCol.end())
  {
    isUsed = true;
  }
  return isUsed;
}

/* Returns a boolean which indicates whether the checked number is already
assigned to a cell in the current 3x3 box. */
bool Sudoku::usedInBox(const int &boxStartRow, const int &boxStartCol, const int &num)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (sudokuGrid[row + boxStartRow][col + boxStartCol].getDisplayNum() == num)
                return true;
        }
    }

    return false;
}

/* Returns a boolean which indicates whether it will be legal to assign
   num to the given cell. */
bool Sudoku::isLegalAssignement(const int &row, const int &col, const int &num)
{
    /* Check if 'num' is not already placed in current row,
       current column and current 3x3 box */
    return (!usedInRow(col, num) &&
            !usedInCol(row, num) &&
            !usedInBox((row - (row % 3)), (col - (col % 3)), num));
}

bool Sudoku::generateSolutionGrid()
{
    int row, col;
    // Continue solving the grid if a default assigned cell is detected (0)
    if (!findUnassignedCell(row, col))
       return true;

    // Consider digits 1 to 9
    for (int num = 0; num < 9; num++)
    {
        if (isLegalAssignement(row, col, guessNum_[num]))
        {
            sudokuGrid[row][col].setDisplayNum(this->guessNum_[num]);

            if (generateSolutionGrid())
                return true;

            // failure, unmake & try again
            sudokuGrid[row][col].setDisplayNum(0);
        }
    }

    return false;
}

int Sudoku::getEmptyCellsNum() const
{
  int emptyCells = 36; //easy mode by default
  switch (difficulty_) 
  {
    case Difficulty::sEasy:
      emptyCells = 36;
      break;
    case Difficulty::sMedium:
      emptyCells = 48;
      break;
    case Difficulty::sDifficult:
      emptyCells = 56;
  }
  return emptyCells;
}

void Sudoku::generateDisplayGrid()
{
  bool firstSelectedCell = false;
  for(int counter = 0; counter < Sudoku::getEmptyCellsNum(); counter++)
  {
    int x_pos = positionGrid_[counter] / 9;
    int y_pos = positionGrid_[counter] % 9;
    sudokuGrid[x_pos][y_pos].setDisplayNum(0);
    sudokuGrid[x_pos][y_pos].setEditable(true);
  }
}

// Printing the grid only for testing purpose
void Sudoku::printGrid() const
{
  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      if(sudokuGrid[i][j].getDisplayNum() == 0)
        std::cout<<".";
      else
        std::cout<<sudokuGrid[i][j].getDisplayNum() ;
      std::cout << "|";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void Sudoku::Update(bool &completed)
{
  if (generateNewSudoku_) // Request for a new grid
  {
    initGrid();
    GenerateGrid();

    completed = false;
    checkSolution_ = false;
    generateNewSudoku_ = false;
  }
  else if (checkSolution_) // Request for evaluating the user grid
  {
    // Disable editing of cells
    for(int row = 0; row < grid_rows_; row++)
    {
      for(int col = 0; col < grid_columns_; col++)
      {
         // disable the editing of all the cells
        sudokuGrid[row][col].setEditable(false);
      }
    }

    for (int row = 0; row < grid_rows_; row++)
    {
      for(int col = 0; col < grid_columns_; col++)
      {
        // verify the value of the cell compared to the solution number
        if (!sudokuGrid[row][col].cellNumVerification())
        {
          completed = false;
          break;
        }
        completed = true;
      }
      if (!completed)
        break;
    }
  }
}

void Sudoku::GenerateGrid()
{
  generateSolutionGrid();

  // print solution Grid
  //printGrid(); //Uncomment for debug reasons to check the solution grid

  // Saving the solutionGrid before modiying the displayed grid to the player
  for(int row = 0; row < grid_rows_; row++)
  {
    for(int col = 0; col < grid_columns_; col++)
    {
      sudokuGrid[row][col].setSolutionNum(sudokuGrid[row][col].getDisplayNum());
    }
  }
  generateDisplayGrid();
}

