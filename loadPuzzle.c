#include <stdio.h>
#include <stdlib.h>

/*the following function takes two input arguments
 1- the file to read a pre-created sudoku puzzle from
 2- a struct holding the values for each of the 81 elements in the Puzzle

the function reads from the file and stores the appropriate values into the
struct
*/
void loadPuzzle(char filename[], pixel * board[][])
{
  FILE * puzzle_file;
  char buffer;
  puzzle_file = fopen(filename, "rb");
  
  if(puzzle_file != NULL)
  {
    for(int i = 0; i < 9; i++)
    {
      for(int j = 0; j < 9; j++)
      {
        fgets(&buffer, 1, puzzle_file);
        if(buffer != '.')
        {
          fscanf(buffer, "%d", &board[i][j].color_id);
        }
      }
    }
  }
}
