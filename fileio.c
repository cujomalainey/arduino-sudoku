//fileio.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  uint8_t device;
  uint8_t shield;
  uint8_t led;
  uint8_t color_id;
  uint8_t locked;
} pixel;

pixel board[9][9];

/*the following function takes two input arguments
 1- the file to read a pre-created sudoku puzzle from
 2- a struct holding the values for each of the 81 elements in the Puzzle

the function reads from the file and stores the appropriate values into the
struct
*/
void loadPuzzle(char filename[55])
{
  FILE * puzzle_file;
  char buffer[81];
  puzzle_file = fopen(filename, "rb");
  
  if(puzzle_file != NULL) {
    printf("REACHED!\n");
    fgets(buffer, 81, puzzle_file);
    printf("%s", buffer);

    for(int i = 0; i < 9; i++) {
      for(int j = 0; j < 9; j++) {
        if(buffer[i*9 + j] != '.') {
          sscanf(&buffer[i*9 + j], "%1d", (int*)&board[i][j].color_id);
        }
      }
    }
  }

  fclose(puzzle_file);
}

void printBoard()
{
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      printf("%d ", board[i][j].color_id);
    }
    printf("\n");
  }  
}

int main()
{
  loadPuzzle("msk_009.txt");
  printBoard();
}
