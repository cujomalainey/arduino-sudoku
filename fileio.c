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
 2- the line of the file to read the puzzle from (1<= line <= 1011)

the function reads from the file and stores the appropriate values into the
struct

the function returns '0' if file loading fails, else it returns '1'
*/
int loadPuzzle(char filename[55], int line)
{
  FILE * puzzle_file;
  char buffer[82];
  puzzle_file = fopen(filename, "rb");
  
  if(puzzle_file != NULL) {
    
    for(int i = 0; i < line; i++)
      fgets(buffer, 83, puzzle_file);

    for(int i = 0; i < 9; i++) {
      for(int j = 0; j < 9; j++) {
        if(buffer[i*9 + j] != '.') {
	  //parse the next number from the buffer
          sscanf(&buffer[i*9 + j], "%1d", (int*)&board[i][j].color_id);
	  //subtract the number by 1 to match colour ids
	  board[i][j].color_id--;
        }
	//if element is to be blank set colour white (id = 9)
	else
	  board[i][j].color_id = 9;
      }
    }

    //close the file
    fclose(puzzle_file);
  }
  //report error if file couldn't be opened
  else
  {
    printf("Could not open file.");
    return 0;
  }
  
  return 1;
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
  int nPuzzle = 1;
  loadPuzzle("msk_009.txt", nPuzzle);
  printBoard();
}
