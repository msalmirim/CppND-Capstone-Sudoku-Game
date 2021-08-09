#include "renderer.h"
#include <iostream>
#include <string>

#define COLOR_BACKGROUND        0x00, 0x00, 0x00, 0xFF 
#define COLOR_SELECT            0xFF, 0x80, 0x00, 0x40
#define COLOR_GRID              0xFF, 0xFF, 0xFF, 0xFF 
#define COLOR_NOT_EDITABLE      0xDF, 0xD9, 0xD9, 0xFF
#define COLOR_CORRECT_SOULTION  0x5B, 0xBF, 0x74, 0xFF
#define COLOR_BAD_SOLUTION      0xC8, 0x49, 0x2E, 0xFF

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width,
                   const std::size_t grid_height,
                   const std::size_t grid_columns,
                   const std::size_t grid_rows)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      grid_width_(grid_width),
      grid_height_(grid_height),
      grid_columns_(grid_columns),
      grid_rows_(grid_rows),
      sdl_font_size_(grid_height/9)
{
        init_SDL();
}

Renderer::~Renderer()
{
  // Disable text input
  SDL_StopTextInput();
  for (int i = 0; i < 14; i++)
  {
    // Free texture if it exists
    if (sdl_textures_[i] != nullptr)
    {
      SDL_DestroyTexture(sdl_textures_[i]);
      sdl_textures_[i] = nullptr;
    }
  }
  SDL_DestroyWindow(sdl_window_);
  
  TTF_CloseFont(sdl_font_);
  sdl_font_ = nullptr;
  SDL_Quit();
  TTF_Quit();
}

void Renderer::init_SDL()
{
    // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
        
  // Initialise SDL_ttf
  if (TTF_Init() == -1)
  {
    std::cerr << "SDL_TTF could not initialize. \n ";
    std::cerr << "SDL_TTF_Error: " << TTF_GetError() << "\n";
  }

  // Create Window
  sdl_window_ = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width_,
                                screen_height_, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window_)
  {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer_)
  {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  
  // Load font for text
  sdl_font_ = TTF_OpenFont("../fonts/FreeSerif.ttf", sdl_font_size_);
  if (sdl_font_ == nullptr)
  {
    std::cerr << "Failed to load lazy font! Error: " << TTF_GetError() << std::endl;
  }
}

void Renderer::loadTexture(const char* text, SDL_Color& fontColor, SDL_Texture*& texture)
{
  SDL_Surface* surfaceMsg = TTF_RenderText_Solid(sdl_font_, text, fontColor);
  
  if (surfaceMsg == nullptr)
  {
    std::cerr << "TTF Surface could not be created. \n";
    std::cerr << "SDL_TTF_Error: " << TTF_GetError() << "\n";
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(sdl_renderer_, surfaceMsg);
    if (texture == nullptr)
    {
      std::cerr << "Texture could not be created. \n";
      std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
    SDL_FreeSurface(surfaceMsg);
  }
}

void Renderer::defineTextures()
{
  SDL_Color fontColor = { 0, 0, 0, SDL_ALPHA_OPAQUE};

  loadTexture(" ", fontColor, sdl_textures_[0]);

  // Load texture for numbers from 1 to 9 
  for (int counter = 1; counter < 10; ++counter)
  {
    const char txt[] = {'0' + counter, '\0'};
    loadTexture(txt, fontColor, sdl_textures_[counter]);
  }
    
  // Load texture for "Check", "Solve", and "New" buttons
  loadTexture("Check", fontColor, sdl_textures_[10]);
  loadTexture("New", fontColor, sdl_textures_[11]);
  loadTexture("Wrong!", fontColor, sdl_textures_[12]);
  loadTexture("Right!", fontColor, sdl_textures_[13]);  
}

void Renderer::drawColorCell(SudokuCell &cell, SDL_Rect &cellRect)
{
  if (cell.getSelected())
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_SELECT);
  }
  else if (!cell.isEditable())
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_NOT_EDITABLE);
  }
  else
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_GRID);
  }

  SDL_RenderFillRect(sdl_renderer_, &cellRect);
}

void Renderer::Render(Sudoku &sudoku, bool &completed)
{
  defineTextures();

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer_, COLOR_BACKGROUND);
  SDL_RenderClear(sdl_renderer_);

  // Render Sudoku Cells : Draw Grid
  SDL_SetRenderDrawColor(sdl_renderer_, COLOR_GRID);

  // Define thick and thin borders
  const int thinBorder = 2;
  const int thickBorder = thinBorder + 6;
  const int std_column_width = (grid_width_ - 4 * thickBorder - 6 * thinBorder) / grid_columns_;
  const int std_row_height = (screen_height_ - 6 * thinBorder - 4 * thickBorder) / 10;

  int drawCursorPositionX = 0;
  int drawCursorPositionY = 0;

  // Draw Grid
  for (int row = 0; row < grid_rows_; row++)
  {
    if (row == 0)
    {
      drawCursorPositionY += thickBorder;
    }
    else if ((row % 3) == 0)
    {
      drawCursorPositionY += std_row_height + thickBorder;
    }
    else
    {
      drawCursorPositionY += std_row_height + thinBorder;      
    }
    
    // Reset starting position for columns
    drawCursorPositionX = 0;

    for (int col = 0; col < grid_columns_; col++)
    {
      if (col == 0)
      {
        drawCursorPositionX += thickBorder;
      }
      else if ((col % 3) == 0) 
      {
        drawCursorPositionX += std_column_width + thickBorder;
      }        
      else
      {
        drawCursorPositionX += std_column_width + thinBorder;      
      }

      // Grid Cell display
      SDL_Rect gridCell;
      gridCell.w = std_column_width;
      gridCell.h = std_row_height;
      gridCell.x = drawCursorPositionX;
      gridCell.y = drawCursorPositionY;

      drawColorCell(sudoku.sudokuGrid[row][col], gridCell);
      
      // Texture Rectangle display
      SDL_Rect textRect;
      int textureWidth;
      int textureHeight;

      SDL_QueryTexture(sdl_textures_[sudoku.sudokuGrid[row][col].getDisplayNum()], NULL, NULL, &textureWidth, &textureHeight);

      const int textureStartRow = gridCell.y + 0.5 * (gridCell.h - textureHeight);
      const int textureStartCol = gridCell.x + 0.5 * (gridCell.w - textureWidth);

      textRect = { textureStartCol, textureStartRow, textureWidth, textureHeight };
      
      SDL_RenderCopy(sdl_renderer_, sdl_textures_[sudoku.sudokuGrid[row][col].getDisplayNum()], NULL, &textRect);
    } 
  }

  // Render Game controll buttons display
  const int buttonsNumbers = 2;
  int std_gameButtonWidth = (grid_width_ - 3 * thickBorder) / buttonsNumbers;

  drawCursorPositionY += thickBorder + std_row_height;
  drawCursorPositionX = thickBorder;

  // Render "Check" button display
  SDL_Rect buttonCheck;
  buttonCheck.w = std_gameButtonWidth;
  buttonCheck.h = std_row_height;
  buttonCheck.x = drawCursorPositionX;
  buttonCheck.y = drawCursorPositionY;

  SDL_Rect textCheckRect;
  int textureCheckWidth;
  int textureCheckHeight;

  SDL_QueryTexture(sdl_textures_[10], NULL, NULL, &textureCheckWidth, &textureCheckHeight);
  const int textureCheckStartRow = buttonCheck.y + 0.5 * (buttonCheck.h - textureCheckHeight);
  const int textureCheckStartCol = buttonCheck.x + 0.5 * (buttonCheck.w - textureCheckWidth);

  textCheckRect = { textureCheckStartCol, textureCheckStartRow, textureCheckWidth, textureCheckHeight };
  SDL_Texture* texture = sdl_textures_[10];

  if (completed) // correct solution
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_CORRECT_SOULTION);
    texture = sdl_textures_[13];
  }
  else if ((!completed) && (sudoku.getCheckSolution())) // bad solution
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_BAD_SOLUTION);
    texture = sdl_textures_[12];
  }
  else if (sudoku.getCheckSelected()) // check button is selected
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_SELECT);
  }
  else
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_NOT_EDITABLE);    
  }

  SDL_RenderFillRect(sdl_renderer_, &buttonCheck);
  SDL_RenderCopy(sdl_renderer_, texture, NULL, &textCheckRect);

  // Render "New" button
  drawCursorPositionX += thickBorder + std_gameButtonWidth;

  if (sudoku.getGenerateNewSelected()) // new button is selected
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_SELECT);
  }
  else
  {
    SDL_SetRenderDrawColor(sdl_renderer_, COLOR_NOT_EDITABLE);    
  }

  SDL_Rect buttonNew;
  buttonNew.w = std_gameButtonWidth;
  buttonNew.h = std_row_height;
  buttonNew.x = drawCursorPositionX;
  buttonNew.y = drawCursorPositionY;
  SDL_RenderFillRect(sdl_renderer_, &buttonNew);  
  
  SDL_Rect textNewRect;
  int textureNewWidth;
  int textureNewHeight;
  SDL_QueryTexture(sdl_textures_[11], NULL, NULL, &textureNewWidth, &textureNewHeight);
  const int textureNewStartRow = buttonNew.y + 0.5 * (buttonNew.h - textureNewHeight);
  const int textureNewStartCol = buttonNew.x + 0.5 * (buttonNew.w - textureNewWidth);

  textNewRect = { textureNewStartCol, textureNewStartRow, textureNewWidth, textureNewHeight };

  SDL_RenderCopy(sdl_renderer_, sdl_textures_[11], NULL, &textNewRect);

  // Update Screen
  SDL_RenderPresent(sdl_renderer_);
  
}

void Renderer::UpdateWindowTitle(int score)
{
  std::string title{"Sudoku Score: " + std::to_string(score)};
  SDL_SetWindowTitle(sdl_window_, title.c_str());
}
