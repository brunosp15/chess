#include <math.h>
#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  empty,
  tower,
  knight,
  bishop,
  queen,
  king,
  pawn,

} PieceType;

typedef enum {
  white,
  black,
} PieceColor;

typedef struct {
  PieceColor color;
  PieceType type;
  Texture2D texture;

} Piece;
char *getPieceAsset(Piece);

Piece BOARD[8][8];
bool PATH_BOARD[8][8];
int FIELD_SIZE = 80;

int selectedY = -1;
int selectedX = -1;
int targetX = -1;
int targetY = -1;
int detectedX = -1;
int detectedY = -1;

bool isComputingClick = false;
char *errorMessage;
char *helpMessage;

Piece w_tower = {white, tower};
Piece w_knight = {white, knight};
Piece w_bishop = {white, bishop};
Piece w_king = {white, king};
Piece w_queen = {white, queen};
Piece w_pawn = {white, pawn};

Piece b_tower = {black, tower};
Piece b_knight = {black, knight};
Piece b_bishop = {black, bishop};
Piece b_king = {black, king};
Piece b_queen = {black, queen};
Piece b_pawn = {black, pawn};

void fillEmptyBoard() {

  BOARD[0][0] = b_tower;
  BOARD[0][7] = b_tower;
  BOARD[0][1] = b_knight;
  BOARD[0][6] = b_knight;
  BOARD[0][2] = b_bishop;
  BOARD[0][5] = b_bishop;
  BOARD[0][3] = b_king;
  BOARD[0][4] = b_queen;

  BOARD[1][0] = b_pawn;
  BOARD[1][7] = b_pawn;
  BOARD[1][1] = b_pawn;
  BOARD[1][6] = b_pawn;
  BOARD[1][2] = b_pawn;
  BOARD[1][5] = b_pawn;
  BOARD[1][3] = b_pawn;
  BOARD[1][4] = b_pawn;

  BOARD[6][0] = w_pawn;
  BOARD[6][7] = w_pawn;
  BOARD[6][1] = w_pawn;
  BOARD[6][6] = w_pawn;
  BOARD[6][2] = w_pawn;
  BOARD[6][5] = w_pawn;
  BOARD[6][3] = w_pawn;
  BOARD[6][4] = w_pawn;

  BOARD[7][0] = w_tower;
  BOARD[7][7] = w_tower;
  BOARD[7][1] = w_knight;
  BOARD[7][6] = w_knight;
  BOARD[7][2] = w_bishop;
  BOARD[7][5] = w_bishop;
  BOARD[7][4] = w_king;
  BOARD[7][3] = w_queen;
}

char *getPieceAsset(Piece piece) {
  if (piece.color == black) {
    switch (piece.type) {
    case empty:
      return "";
    case tower:
      return "assets/b-tower.png";
    case knight:
      return "assets/b-knight.png";
    case bishop:
      return "assets/b-bishop.png";
    case queen:
      return "assets/b-queen.png";
    case king:
      return "assets/b-king.png";
    case pawn:
      return "assets/b-pawn.png";
    }
  } else {

    switch (piece.type) {
    case empty:
      return "";
    case tower:
      return "assets/w-tower.png";
    case knight:
      return "assets/w-knight.png";
    case bishop:
      return "assets/w-bishop.png";
    case queen:
      return "assets/w-queen.png";
    case king:
      return "assets/w-king.png";
    case pawn:
      return "assets/w-pawn.png";
    }
  }
}

void loadAssets() {
  b_bishop.texture = LoadTexture(getPieceAsset(b_bishop));
  b_queen.texture = LoadTexture(getPieceAsset(b_queen));
  b_king.texture = LoadTexture(getPieceAsset(b_king));
  b_knight.texture = LoadTexture(getPieceAsset(b_knight));
  b_tower.texture = LoadTexture(getPieceAsset(b_tower));
  b_pawn.texture = LoadTexture(getPieceAsset(b_pawn));

  w_bishop.texture = LoadTexture(getPieceAsset(w_bishop));
  w_queen.texture = LoadTexture(getPieceAsset(w_queen));
  w_king.texture = LoadTexture(getPieceAsset(w_king));
  w_knight.texture = LoadTexture(getPieceAsset(w_knight));
  w_tower.texture = LoadTexture(getPieceAsset(w_tower));
  w_pawn.texture = LoadTexture(getPieceAsset(w_pawn));
}

void movePieceInTheBoard() {
  printf("mooving piece\n");
  Piece piece = BOARD[selectedY][selectedX];
  BOARD[targetY][targetX] = piece;
  Piece noPiece = {white, empty};
  BOARD[selectedY][selectedX] = noPiece;
  errorMessage = NULL;
  selectedX = -1;
  selectedY = -1;
  detectedX = -1;
  detectedY = -1;
}

void movePawn(Piece piece) {
  Piece pieceOnTarget = BOARD[targetY][targetX];
  if (piece.color == white) {
    printf("white pawn moving\n");
    if (selectedY - targetY == 1 && selectedX == targetX) {
      if (pieceOnTarget.type == empty) {
        movePieceInTheBoard();
        return;
      }
    }

    if (selectedX - targetX == 1 || selectedX - targetX == -1) {
      if (pieceOnTarget.color == black && pieceOnTarget.type != empty) {
        movePieceInTheBoard();
        return;
      }
    }
  } else {
    printf("black pawn moving\n");

    if (targetY - selectedY == 1 && selectedX == targetX) {
      if (BOARD[targetY][targetX].type == empty) {
        movePieceInTheBoard();
        return;
      }
    }

    if (selectedX - targetX == 1 || selectedX - targetX == -1) {
      if (pieceOnTarget.color == white && pieceOnTarget.type != empty) {
        movePieceInTheBoard();
        return;
      }
    }
  }

  errorMessage = "Invalid movement";
}
typedef enum {
  downRight,
  downLeft,
  upRight,
  upLeft,
} BishopDirection;

void moveBishop(Piece piece) {
  if (abs(targetX - selectedX) != abs(targetY - selectedY)) {
    errorMessage = "invalid movement";
    return;
  }

  BishopDirection direction = downRight;

  if (targetY < selectedY) {
    if (targetX < selectedX) {
      direction = upLeft;
    } else {
      direction = upRight;
    }
  } else {
    if (targetX < selectedX) {
      direction = downLeft;
    } else {
      direction = downRight;
    }
  }

  if (direction == upLeft) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selectedY - n;
      int scanningX = selectedX - n;
      if (scanningX < 0 || scanningY < 0 || scanningY < targetY) {
        reachBoardLimit = true;
      } else {
        Piece pieceOnTheWay = BOARD[scanningY][scanningX];
        if (pieceOnTheWay.type != empty && pieceOnTheWay.color == piece.color) {
          errorMessage = "Same color piece on the way";
          return;
        } else if (pieceOnTheWay.type != empty &&
                   pieceOnTheWay.color != piece.color && scanningX != targetX) {
          errorMessage = "Oposite color piece on the way";
          return;
        }

        n++;
      }
    }
    movePieceInTheBoard();
  } else if (direction == upRight) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selectedY - n;
      int scanningX = selectedX + n;
      if (scanningX > 7 || scanningY < 0 || scanningY < targetY) {
        reachBoardLimit = true;
      } else {
        Piece pieceOnTheWay = BOARD[scanningY][scanningX];
        if (pieceOnTheWay.type != empty && pieceOnTheWay.color == piece.color) {
          errorMessage = "Same color piece on the way";
          return;
        } else if (pieceOnTheWay.type != empty &&
                   pieceOnTheWay.color != piece.color && scanningX != targetX) {
          errorMessage = "Oposite color piece on the way";
          return;
        }
        n++;
      }
    }
    movePieceInTheBoard();
  } else if (direction == downRight) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selectedY + n;
      int scanningX = selectedX + n;
      if (scanningX > 7 || scanningY > 7 || scanningY > targetY) {
        reachBoardLimit = true;
      } else {
        Piece pieceOnTheWay = BOARD[scanningY][scanningX];
        if (pieceOnTheWay.type != empty && pieceOnTheWay.color == piece.color) {
          errorMessage = "Same color piece on the way";
          return;
        } else if (pieceOnTheWay.type != empty &&
                   pieceOnTheWay.color != piece.color && scanningX != targetX) {
          errorMessage = "Oposite color piece on the way";
          return;
        }
        n++;
      }
    }
    movePieceInTheBoard();
  } else if (direction == downLeft) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selectedY + n;
      int scanningX = selectedX - n;
      if (scanningX < 0 || scanningY > 7 || scanningY < (targetY - 1)) {
        printf("scanningY %d, scanningX %d\n", scanningY, scanningX);
        reachBoardLimit = true;
      } else {
        Piece pieceOnTheWay = BOARD[scanningY][scanningX];
        if (pieceOnTheWay.type != empty && pieceOnTheWay.color == piece.color) {
          errorMessage = "Same color piece on the way";
          return;
        } else if (pieceOnTheWay.type != empty &&
                   pieceOnTheWay.color != piece.color && scanningX != targetX) {
          errorMessage = "Oposite color piece on the way";
          return;
        }
        n++;
      }
    }
    movePieceInTheBoard();
  }

  printf("direction: %d\n", direction);
}

void moveKnight(Piece piece) {
  Piece pieceOnTarget = BOARD[targetY][targetX];
  if (abs(targetX - selectedX) == 1 && abs(targetY - selectedY) == 2 &&
      pieceOnTarget.color != piece.color) {
    movePieceInTheBoard();
    return;
  }

  if (abs(targetX - selectedX) == 2 && abs(targetY - selectedY) == 1 &&
      pieceOnTarget.color != piece.color) {
    movePieceInTheBoard();
    return;
  }
  errorMessage = "Invalid movement";
}

void moveTower(Piece piece) {
  if (targetX != selectedX && targetY != selectedY) {
    errorMessage = "Invalid movement";
    return;
  }

  if (targetX == selectedX) {
    // GOING UP
    if (selectedY - targetY > 0) {
      for (int n = 1; n <= selectedY - targetY; n++) {
        Piece pieceOnTheWay = BOARD[selectedY - n][selectedX];
        if (pieceOnTheWay.type != empty && selectedY - n != targetY) {
          errorMessage = "Piece on the way";
          printf("tem peca aqui: %d %d \n", selectedY - n, selectedX);
          return;
        }
      }
      // GOING DOWN
    } else {
      for (int n = 1; n <= selectedY + targetY; n++) {
        Piece pieceOnTheWay = BOARD[selectedY + n][selectedX];
        if (pieceOnTheWay.type != empty && selectedY + n != targetY) {
          errorMessage = "Piece on the way";
          printf("tem peca aqui: %d %d \n", selectedY - n, selectedX);

          return;
        }
      }
    }
  }

  movePieceInTheBoard();
}

void movePiece() {
  printf("targetX: %d\ntargetY: %d\n", targetX, targetY);

  if (targetX >= 0) {
    Piece piece = BOARD[selectedY][selectedX];
    switch (piece.type) {
    case pawn:
      movePawn(piece);
      break;

    case empty:
    case tower:
      moveTower(piece);
      break;
    case knight:
      moveKnight(piece);
      break;
    case bishop:
      moveBishop(piece);
      break;
    case queen:
    case king:
      break;
    }
  }
}
void drawSelectedPiecePath() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      PATH_BOARD[y][x] = false;
    }
  }
  Piece piece = BOARD[selectedY][selectedX];
  if (piece.type == bishop) {
    for (int n = 1; n + selectedX < 8 && selectedY + n < 8; n++) {
      PATH_BOARD[selectedY + n][selectedX + n] = true;
    }

    for (int n = 1; selectedX - n > -1 && selectedY + n < 8; n++) {
      PATH_BOARD[selectedY + n][selectedX - n] = true;
    }

    for (int n = 1; selectedX + n < 8 && selectedY - n > -1; n++) {
      PATH_BOARD[selectedY - n][selectedX + n] = true;
    }

    for (int n = 1; selectedX - n > -1 && selectedY - n > -1; n++) {
      PATH_BOARD[selectedY - n][selectedX - n] = true;
    }
  } else if (piece.type == knight) {
  }
}
int main() {
  InitWindow(FIELD_SIZE * 8, (FIELD_SIZE * 8) + 40, "Chess");

  loadAssets();
  fillEmptyBoard();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      fillEmptyBoard();
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !isComputingClick) {
      Vector2 mousePosition = GetMousePosition();
      selectedY = (int)mousePosition.y / FIELD_SIZE;
      selectedX = (int)mousePosition.x / FIELD_SIZE;
      isComputingClick = true;
      printf("selectedX:%d\nselectedY:%d\n", selectedX, selectedY);
      drawSelectedPiecePath();
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !isComputingClick) {
      Vector2 mousePosition = GetMousePosition();
      targetX = (int)mousePosition.x / FIELD_SIZE;
      targetY = (int)mousePosition.y / FIELD_SIZE;
      isComputingClick = true;
      movePiece();
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
      isComputingClick = false; // Reset once the user lets go
    }

    ClearBackground(BLACK);

    if (errorMessage != NULL) {
      DrawText(errorMessage, 20, (FIELD_SIZE * 8) + 10, 16, RED);
    }

    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        Color color;
        if (y % 2 == 0) {
          if (x % 2 == 0) {
            color = DARKGRAY;
          } else {
            color = WHITE;
          }
        } else {

          if (x % 2 == 0) {
            color = WHITE;
          } else {
            color = DARKGRAY;
          }
        }

        if (y == detectedY && x == detectedX) {
          color = RED;
        }

        if (y == selectedY && x == selectedX) {
          color = GREEN;
        }
        int position_x = x * FIELD_SIZE;
        int position_y = y * FIELD_SIZE;

        DrawRectangle(position_x, position_y, FIELD_SIZE, FIELD_SIZE, color);
        Texture2D texture = BOARD[y][x].texture;
        if (PATH_BOARD[y][x] == true) {
          DrawRectangle(position_x, position_y, FIELD_SIZE, FIELD_SIZE, YELLOW);
        }
        DrawTexturePro(
            texture,
            (Rectangle){0, 0, (float)texture.width,
                        (float)texture.height}, // source rectangle (full image)
            (Rectangle){
                position_x, // X position on screen
                position_y, // Y position on screen
                FIELD_SIZE, // width to draw (scaled)
                FIELD_SIZE  // height to draw (scaled)
            },
            (Vector2){0, 0}, // origin offset (not needed here)
            0.0f,            // rotation
            WHITE            // tint (WHITE = no tint)
        );
        char houseNumber[50] = "";
        sprintf(houseNumber, "y:%d/x:%d", y, x);
        DrawText(houseNumber, position_x, position_y, 16, BLUE);
      }
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
