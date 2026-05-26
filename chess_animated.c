#include <raylib.h>
#include <stdio.h>

#define FIELD_SIZE 80
#define WINDOW_SIZE (8 * FIELD_SIZE) + 40

typedef enum { rook, knight, bishop, queen, king, pawn, empty } Piece;
typedef enum { black, white } PieceColor;

typedef struct {
  Piece piece;
  Color color;

} Field;

Field BOARD[8][8];

void initBoard() {
  Field b_rook = {rook, black};
  Field b_knight = {knight, black};
  Field b_bishop = {bishop, black};
  Field b_queen = {queen, black};
  Field b_king = {king, black};
  Field b_pawn = {pawn, black};

  Field w_rook = {rook, black};
  Field w_knight = {knight, black};
  Field w_bishop = {bishop, black};
  Field w_queen = {queen, black};
  Field w_king = {king, black};
  Field w_pawn = {pawn, black};

  BOARD = {
    {b_rook, b_knight, b_bishop, b_queen, b_king, b_bishop, b_knight, b_rook},
    {b_pawn, b_knight, b_bishop, b_queen, b_king, b_bishop, b_knight, b_rook},

  }
}

void drawField(int x, int y, Piece piece) {
  DrawRectangle(x, y, x * FIELD_SIZE, y * FIELD_SIZE, WHITE);
}

int main() {

  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Animated Chess");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }

  return 0;
}
