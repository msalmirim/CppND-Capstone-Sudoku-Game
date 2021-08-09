#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <random>
#include "SDL.h"

// auxiliary class to queue and dequeue waiting vehicles in a thread-safe manner
class SudokuCell
{
 public:
  /* Constructor */
  SudokuCell();
  
  /* Getters and setters */
  void setEditable(const bool editable) { editable_ = editable; };
  bool isEditable(void) const { return editable_; };
  
  void setDisplayNum(const int num) { displayNum_ = num; };
  int getDisplayNum(void) const { return displayNum_; };
    
  void setSelected(const bool sel) { selected_ = sel; };
  int getSelected(void) const { return selected_; };
  
  void setSolutionNum(const int solution) { solutionNum_ = solution; };

  // Define comparison operator for SudokuCell Class
  bool operator==(const int &num);

  void updateCell();
  bool cellNumVerification() const; 

 private:
  bool editable_;
  bool selected_;
  int displayNum_;
  int solutionNum_;

};

class Sudoku {
 public:
  enum class Difficulty { sEasy, sMedium, sDifficult};

  /* Constructor */
  Sudoku(std::size_t grid_width, std::size_t grid_height, std::size_t grid_rows, std::size_t grid_columns, Difficulty difficulty);
  
  /* Copy constructor definition */ 
  Sudoku(const Sudoku &sudoku);
    
  /* Copy assignement operator */
  Sudoku &operator=(const Sudoku &sudoku);
  
  std::size_t getGridRows() { return grid_rows_; };
  std::size_t getGridColumns() {return grid_columns_; };
  
  void setGenerateNewSudoku(const bool newSudoku) { generateNewSudoku_ = newSudoku; };

  void setCheckSolution(const bool checkSol) { checkSolution_ = checkSol; };
  bool getCheckSolution() const { return checkSolution_;};

  void setCheckSelected(const bool checkSel) { checkSelected_ = checkSel; };
  int getCheckSelected(void) const { return checkSelected_; };  
  
  void setGenerateNewSelected(const bool genNewSel) { newGridSelected_ = genNewSel; };
  int getGenerateNewSelected(void) const { return newGridSelected_; }; 

  void Update(bool &completed);
  void GenerateGrid();

  std::vector<std::vector<SudokuCell>> sudokuGrid;

 private:
  Difficulty difficulty_ = Difficulty::sEasy;
  std::size_t grid_width_;
  std::size_t grid_height_;
  std::size_t grid_rows_ {9};
  std::size_t grid_columns_ {9};
  
  std::vector<int> positionGrid_;
  std::vector<int> guessNum_;
  std::random_device dev_;
  std::mt19937 engine_;
  
  bool checkSolution_{false};  
  bool checkSelected_{false};
  bool generateNewSudoku_{false};
  bool newGridSelected_{false};

  void initGrid();
  bool findUnassignedCell(int &unassignedCellRowindex, int &unassignedCellColindex) const;
  bool usedInCol(const int &column, const int &num);
  void getColumn(std::vector<SudokuCell> &sudokuCol, const int &col);
  bool usedInRow(const int &row, const int &num);
  bool usedInBox(const int &boxStartRow, const int &boxStartCol, const int &num);
  bool isLegalAssignement(const int &row, const int &col, const int &num);
  bool generateSolutionGrid();
  int  getEmptyCellsNum() const;
  void generateDisplayGrid();
  void printGrid() const;
  bool cellNumVerification() const;

};

#endif