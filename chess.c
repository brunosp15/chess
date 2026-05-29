#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_RED "\033[1;31m"
#define ANSI_GREEN "\033[1;32m"
#define ANSI_YELLOW "\033[1;33m"
#define ANSI_BLUE "\033[1;34m"
#define ANSI_RESET "\033[0m"

typedef enum {
  empty,
  rook,
  knight,
  bishop,
  queen,
  king,
  pawn,

} PieceType;

typedef enum {
  downRight,
  downLeft,
  upRight,
  upLeft,
  up,
  down,
  left,
  right
} Direction;

typedef enum {
  white,
  black,
} PieceColor;

typedef struct {
  PieceColor color;
  PieceType type;
  Texture2D texture;
  Vector2 pos;
  Vector2 target;
  char *asset;
  Vector2 originalPos;
  bool isMoving;
  Direction direction;

} Piece;

Piece BOARD[8][8];
bool PATH_BOARD[8][8];
int FIELD_SIZE = 80;

Piece *selected;

bool isComputingClick = false;
char *errorMessage;
char *helpMessage;

Piece w_rook1 = {.color = white, .type = rook, .asset = "assets/w-rook.png"};
Piece w_knight1 = {
    .color = white, .type = knight, .asset = "assets/w-knight.png"};
Piece w_bishop1 = {
    .color = white, .type = bishop, .asset = "assets/w-bishop.png"};
Piece w_king = {.color = white, .type = king, .asset = "assets/w-king.png"};
Piece w_queen = {.color = white, .type = queen, .asset = "assets/w-queen.png"};
Piece w_knight2 = {
    .color = white, .type = knight, .asset = "assets/w-knight.png"};
Piece w_bishop2 = {
    .color = white, .type = bishop, .asset = "assets/w-bishop.png"};
Piece w_rook2 = {.color = white, .type = rook, .asset = "assets/w-rook.png"};

Piece w_pawn1 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn2 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn3 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn4 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn5 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn6 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn7 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};
Piece w_pawn8 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png"};

Piece b_rook1 = {.color = black, .type = rook, .asset = "assets/b-rook.png"};
Piece b_knight1 = {
    .color = black, .type = knight, .asset = "assets/b-knight.png"};
Piece b_bishop1 = {
    .color = black, .type = bishop, .asset = "assets/b-bishop.png"};
Piece b_king = {.color = black, .type = king, .asset = "assets/b-king.png"};
Piece b_queen = {.color = black, .type = queen, .asset = "assets/b-queen.png"};
Piece b_knight2 = {
    .color = black, .type = knight, .asset = "assets/b-knight.png"};
Piece b_bishop2 = {
    .color = black, .type = bishop, .asset = "assets/b-bishop.png"};
Piece b_rook2 = {.color = black, .type = rook, .asset = "assets/b-rook.png"};

Piece b_pawn1 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn2 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn3 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn4 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn5 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn6 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn7 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};
Piece b_pawn8 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png"};

Piece emptyPiece = {.type = empty};

Piece *getPieceFromBoard(Vector2 pos) {
  Piece *piece = &BOARD[(int)pos.x][(int)pos.y];
  // piece.pos = pos;
  return piece;
}

void fillEmptyBoard() {
  printf(ANSI_GREEN "Initalizing board\n" ANSI_RESET);
  BOARD[0][0] = b_rook1;
  BOARD[1][0] = b_knight1;
  BOARD[2][0] = b_bishop1;
  BOARD[3][0] = b_king;
  BOARD[4][0] = b_queen;
  BOARD[5][0] = b_bishop2;
  BOARD[6][0] = b_knight2;
  BOARD[7][0] = b_rook2;

  BOARD[0][1] = b_pawn1;
  BOARD[1][1] = b_pawn2;
  BOARD[2][1] = b_pawn3;
  BOARD[3][1] = b_pawn4;
  BOARD[4][1] = b_pawn5;
  BOARD[5][1] = b_pawn6;
  BOARD[6][1] = b_pawn7;
  BOARD[7][1] = b_pawn8;

  BOARD[0][6] = w_pawn1;
  BOARD[1][6] = w_pawn2;
  BOARD[2][6] = w_pawn3;
  BOARD[3][6] = w_pawn4;
  BOARD[4][6] = w_pawn5;
  BOARD[5][6] = w_pawn6;
  BOARD[6][6] = w_pawn7;
  BOARD[7][6] = w_pawn8;

  BOARD[0][7] = w_rook1;
  BOARD[1][7] = w_knight1;
  BOARD[2][7] = w_bishop1;
  BOARD[3][7] = w_queen;
  BOARD[4][7] = w_king;
  BOARD[5][7] = w_bishop2;
  BOARD[6][7] = w_knight2;
  BOARD[7][7] = w_rook2;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      Piece *piece = &BOARD[x][y];
      piece->pos = (Vector2){.x = x * FIELD_SIZE, .y = y * FIELD_SIZE};
      piece->texture = LoadTexture(piece->asset);
    }
  }
  printf(ANSI_GREEN "======= Initialization finished \n" ANSI_RESET);
}

void startPieceAnimation(Direction direction) {
  selected->originalPos = selected->pos;
  selected->isMoving = true;
  selected->direction = direction;
  printf(ANSI_RED "Original position %f %f\n" ANSI_RESET,
         selected->originalPos.x, selected->originalPos.y);
}

void movePawn(Piece *piece) {
  Piece *pieceOnTarget = getPieceFromBoard(piece->target);
  if (piece->color == white) {
    printf("white pawn moving from: %f to: %f\n", piece->pos.y / FIELD_SIZE,
           piece->target.y);
    if (selected->pos.y / FIELD_SIZE - selected->target.y == 1 &&
        selected->pos.x / FIELD_SIZE == selected->target.x) {
      if (pieceOnTarget->type == empty) {
        startPieceAnimation(up);
        return;
      }
    }

    bool hasBlackPieceOnTarget =
        pieceOnTarget->color == black && pieceOnTarget->type != empty;
    if (hasBlackPieceOnTarget) {
      if (selected->pos.x / FIELD_SIZE - selected->target.x == 1) {
        startPieceAnimation(upLeft);
        return;
      } else if (selected->pos.x / FIELD_SIZE - selected->target.x == -1) {
        startPieceAnimation(upRight);
        return;
      }
    }
  } else {
    printf("black pawn moving\n");

    if (selected->target.y - selected->pos.y / FIELD_SIZE == 1 &&
        selected->pos.x / FIELD_SIZE == selected->target.x) {
      if (getPieceFromBoard(piece->target)->type == empty) {
        startPieceAnimation(down);
        return;
      }
    }

    if (pieceOnTarget->color == white && pieceOnTarget->type != empty) {
      if (selected->pos.x / FIELD_SIZE - selected->target.x == 1) {
        startPieceAnimation(downLeft);
        return;
      } else if (selected->pos.x / FIELD_SIZE - selected->target.x == -1) {
        startPieceAnimation(downRight);
        return;
      }
    }
  }

  errorMessage = "Invalid movement";
}
char *getBishopErrorMessage(Piece piece, int x, int y) {

  printf(ANSI_RED "%d %d\n" ANSI_RESET, x, y);
  Piece pieceOnTheWay = BOARD[x][y];
  if (pieceOnTheWay.type != empty && pieceOnTheWay.color == piece.color) {
    return "Same color piece on the way";
  } else if (pieceOnTheWay.type != empty &&
             pieceOnTheWay.color != piece.color && x != selected->target.x) {
    return "Oposite color piece on the way";
  }

  return NULL;
}
void moveBishop(Piece piece) {
  if (fabsf(selected->target.x - selected->pos.x / FIELD_SIZE) !=
      fabsf(selected->target.y - selected->pos.y / FIELD_SIZE)) {
    errorMessage = "invalid movement";
    return;
  }

  Direction direction = downRight;

  if (selected->target.y < selected->pos.y / FIELD_SIZE) {
    if (selected->target.x < selected->pos.x / FIELD_SIZE) {
      direction = upLeft;
    } else {
      direction = upRight;
    }
  } else {
    if (selected->target.x < selected->pos.x / FIELD_SIZE) {
      direction = downLeft;
    } else {
      direction = downRight;
    }
  }

  if (direction == upLeft) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selected->pos.y / FIELD_SIZE - n;
      int scanningX = selected->pos.x / FIELD_SIZE - n;
      if (scanningX < 0 || scanningY < 0 || scanningY < selected->target.y) {
        reachBoardLimit = true;
      } else {
        char *result = getBishopErrorMessage(piece, scanningX, scanningY);
        if (result == NULL) {
          n++;
        } else {
          errorMessage = result;
          return;
        }
      }
    }
    startPieceAnimation(direction);
  } else if (direction == upRight) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selected->pos.y / FIELD_SIZE - n;
      int scanningX = selected->pos.x / FIELD_SIZE + n;
      if (scanningX > 7 || scanningY < 0 || scanningY < selected->target.y) {
        reachBoardLimit = true;
      } else {
        char *result = getBishopErrorMessage(piece, scanningX, scanningY);
        if (result == NULL) {
          n++;
        } else {
          errorMessage = result;
          return;
        }
      }
    }
    startPieceAnimation(direction);
  } else if (direction == downRight) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selected->pos.y / FIELD_SIZE + n;
      int scanningX = selected->pos.x / FIELD_SIZE + n;
      if (scanningX > 7 || scanningY > 7 || scanningY > selected->target.y) {
        reachBoardLimit = true;
      } else {
        char *result = getBishopErrorMessage(piece, scanningX, scanningY);
        if (result == NULL) {
          n++;
        } else {
          errorMessage = result;
          return;
        }
      }
    }
    startPieceAnimation(direction);

  } else if (direction == downLeft) {
    bool reachBoardLimit = false;
    int n = 1;
    while (!reachBoardLimit) {
      int scanningY = selected->pos.y / FIELD_SIZE + n;
      int scanningX = selected->pos.x / FIELD_SIZE - n;
      if (scanningX < 0 || scanningY > 7 ||
          scanningY < (selected->target.y - 1)) {
        reachBoardLimit = true;
      } else {
        char *result = getBishopErrorMessage(piece, scanningX, scanningY);
        if (result == NULL) {
          n++;
        } else {
          errorMessage = result;
          return;
        }
      }
    }
    startPieceAnimation(direction);
  }

  printf("direction: %d\n", direction);
}

void moveKnight(Piece piece) {
  Piece pieceOnTarget = *getPieceFromBoard(piece.target);
  if (abs(selected->target.x - selected->pos.x) == 1 &&
      abs(selected->target.y - selected->pos.y) == 2 &&
      pieceOnTarget.color != piece.color) {
    // startPieceAnimation();
    return;
  }

  if (abs(selected->target.x - selected->pos.x) == 2 &&
      abs(selected->target.y - selected->pos.y) == 1 &&
      pieceOnTarget.color != piece.color) {
    // startPieceAnimation();
    return;
  }
  errorMessage = "Invalid movement";
}

void movePiece() {
  printf("target.x: %f\ntarget.y: %f\n", selected->target.x,
         selected->target.y);

  if (selected->target.x >= 0) {
    switch (selected->type) {
    case pawn:
      movePawn(selected);
      break;

    case empty:
    case rook:
      break;
    case knight:
      moveKnight(*selected);
      break;
    case bishop:
      moveBishop(*selected);
      break;
    case queen:
    case king:
      break;
    }
  }
}

void drawPiece(Piece *piece) {
  DrawTexturePro(
      piece->texture,
      (Rectangle){
          0, 0, (float)piece->texture.width,
          (float)piece->texture.height}, // source rectangle (full image)
      (Rectangle){
          piece->pos.x, // X position on screen
          piece->pos.y, // Y position on screen
          FIELD_SIZE,   // width to draw (scaled)
          FIELD_SIZE    // height to draw (scaled)
      },
      (Vector2){0, 0}, // origin offset (not needed here)
      0.0f,            // rotation
      WHITE            // tint (WHITE = no tint)
  );
}

void drawMovingPiece() {
  if (selected != NULL && selected->isMoving) {
    drawPiece(selected);
    printf(ANSI_YELLOW "pos.y %f, target.y %f\n", selected->pos.y / FIELD_SIZE,
           selected->target.y);
    if (selected->pos.y / FIELD_SIZE == selected->target.y &&
        selected->pos.x / FIELD_SIZE == selected->target.x) {
      selected->isMoving = false;

      // Update the selected piece in the BOARD
      BOARD[(int)(selected->pos.x / FIELD_SIZE)]
           [(int)(selected->pos.y / FIELD_SIZE)] = *selected;

      // Remove the piece from original BOARD place
      BOARD[(int)(selected->originalPos.x / FIELD_SIZE)]
           [(int)(selected->originalPos.y / FIELD_SIZE)] = emptyPiece;

    } else {
      switch (selected->direction) {
      case downRight:
        selected->pos.x += 10;
        selected->pos.y += 10;
        break;
      case downLeft:
        selected->pos.x -= 10;
        selected->pos.y += 10;
        break;
      case upRight:
        selected->pos.x += 10;
        selected->pos.y -= 10;
        break;
      case upLeft:
        selected->pos.x -= 10;
        selected->pos.y -= 10;
        break;
      case up:
        selected->pos.y -= 10;
        break;
      case down:
        selected->pos.y += 10;
        break;
      case left:
        selected->pos.x -= 10;
        break;
      case right:
        selected->pos.x += 10;
        break;
      }
    }
  }
}

Color getFieldColor(int x, int y) {
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

  if (selected != NULL && y == (int)selected->pos.y / FIELD_SIZE &&
      x == (int)selected->pos.x / FIELD_SIZE) {
    color = GREEN;
  }
  return color;
}

int main() {
  InitWindow(FIELD_SIZE * 8, (FIELD_SIZE * 8) + 40, "Chess");

  fillEmptyBoard();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !isComputingClick) {
      Vector2 mp = GetMousePosition();
      Vector2 pos = {(int)mp.x / FIELD_SIZE, (int)mp.y / FIELD_SIZE};

      selected = getPieceFromBoard(pos);
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !isComputingClick) {
      Vector2 mp = GetMousePosition();
      Vector2 pos = {(int)mp.x / FIELD_SIZE, (int)mp.y / FIELD_SIZE};
      selected->target = pos;
      isComputingClick = true;
      movePiece();
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
      isComputingClick = false; // Reset once the user lets go
    }

    ClearBackground(BLUE);

    if (errorMessage != NULL) {
      DrawText(errorMessage, 20, (FIELD_SIZE * 8) + 10, 16, RED);
    }

    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        int position_x = x * FIELD_SIZE;
        int position_y = y * FIELD_SIZE;
        char houseNumber[50] = "";
        sprintf(houseNumber, "x:%d y:%d", x, y);
        // printf("Drawing %s\n", houseNumber);

        DrawRectangle(position_x, position_y, FIELD_SIZE, FIELD_SIZE,
                      getFieldColor(x, y));
        Vector2 pos = {x, y};
        Piece piece = *getPieceFromBoard(pos);

        if (!piece.isMoving) {
          drawPiece(&piece);
          // printf(ANSI_BLUE "Drawing piece %d from BOARD[%d][%d]" ANSI_RESET,
          //        piece.type, x, y);
        }

        DrawText(houseNumber, position_x, position_y, 16, BLUE);
        // printf(ANSI_GREEN "%s Drawn", houseNumber);
      }
    }

    drawMovingPiece();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
