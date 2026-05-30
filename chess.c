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
    right,
    nope,
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
    bool isMoving;
    Direction direction;

} Piece;

typedef struct {
    int x;
    int y;
} IVector2;

int FIELD_SIZE = 80;
Piece *selectedPiece;

bool isComputingClick = false;
char *errorMessage;
char *helpMessage;

IVector2 possibilities[64];

Piece w_rook1 = {.color = white, .type = rook, .asset = "assets/w-rook.png", .pos = (Vector2){0, 7}};
Piece w_knight1 = {.color = white, .type = knight, .asset = "assets/w-knight.png", .pos = (Vector2){1, 7}};
Piece w_bishop1 = {.color = white, .type = bishop, .asset = "assets/w-bishop.png", .pos = (Vector2){2, 7}};
Piece w_king = {.color = white, .type = king, .asset = "assets/w-king.png", .pos = (Vector2){3, 7}};
Piece w_queen = {.color = white, .type = queen, .asset = "assets/w-queen.png", .pos = (Vector2){4, 7}};
Piece w_knight2 = {.color = white, .type = knight, .asset = "assets/w-knight.png", .pos = (Vector2){5, 7}};
Piece w_bishop2 = {.color = white, .type = bishop, .asset = "assets/w-bishop.png", .pos = (Vector2){6, 7}};
Piece w_rook2 = {.color = white, .type = rook, .asset = "assets/w-rook.png", .pos = (Vector2){7, 7}};

Piece w_pawn1 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){0, 6}};
Piece w_pawn2 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){1, 6}};
Piece w_pawn3 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){2, 6}};
Piece w_pawn4 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){3, 6}};
Piece w_pawn5 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){4, 6}};
Piece w_pawn6 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){5, 6}};
Piece w_pawn7 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){6, 6}};
Piece w_pawn8 = {.color = white, .type = pawn, .asset = "assets/w-pawn.png", .pos = (Vector2){7, 6}};

Piece b_rook1 = {.color = black, .type = rook, .asset = "assets/b-rook.png", .pos = (Vector2){0, 0}};
Piece b_knight1 = {.color = black, .type = knight, .asset = "assets/b-knight.png", .pos = (Vector2){1, 0}};
Piece b_bishop1 = {.color = black, .type = bishop, .asset = "assets/b-bishop.png", .pos = (Vector2){2, 0}};
Piece b_king = {.color = black, .type = king, .asset = "assets/b-king.png", .pos = (Vector2){3, 0}};
Piece b_queen = {.color = black, .type = queen, .asset = "assets/b-queen.png", .pos = (Vector2){4, 0}};
Piece b_knight2 = {.color = black, .type = knight, .asset = "assets/b-knight.png", .pos = (Vector2){5, 0}};
Piece b_bishop2 = {.color = black, .type = bishop, .asset = "assets/b-bishop.png", .pos = (Vector2){6, 0}};
Piece b_rook2 = {.color = black, .type = rook, .asset = "assets/b-rook.png", .pos = (Vector2){7, 0}};

Piece b_pawn1 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){0, 1}};
Piece b_pawn2 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){1, 1}};
Piece b_pawn3 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){2, 1}};
Piece b_pawn4 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){3, 1}};
Piece b_pawn5 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){4, 1}};
Piece b_pawn6 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){5, 1}};
Piece b_pawn7 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){6, 1}};
Piece b_pawn8 = {.color = black, .type = pawn, .asset = "assets/b-pawn.png", .pos = (Vector2){7, 1}};

Piece *piecesList[32];
char computedString[50];

IVector2 convert(Vector2 vector) { return (IVector2){(int)(vector.x / FIELD_SIZE), (int)(vector.y / FIELD_SIZE)}; }
char *getPieceString(Piece *piece) {
    char *color = piece->color == white ? "white" : "black";
    char *name;
    switch (piece->type) {

    case rook:
        name = "rook";
        break;
    case knight:
        name = "knight";
        break;
    case bishop:
        name = "bishop";
        break;
    case queen:
        name = "queen";
        break;
    case king:
        name = "king";
        break;
    case pawn:
        name = "pawn";
        break;
    }
    IVector2 iPos = convert(piece->pos);
    sprintf(computedString, "%s %s on %d %d", color, name, iPos.x, iPos.y);
    return computedString;
}

Piece *getPieceFromPosition(Vector2 pos) {
    for (int i = 0; i < 32; i++) {
        Piece *piece = piecesList[i];
        IVector2 ipos = convert(pos);
        IVector2 piecePos = convert(piece->pos);

        if (piecePos.x == ipos.x && piecePos.y == ipos.y) {
            return piece;
        }
    }
    return NULL;
}

void fillEmptyBoard() {
    printf(ANSI_GREEN "Initalizing "
                      "board\n" ANSI_RESET);
    piecesList[0] = &b_rook1;
    piecesList[1] = &b_knight1;
    piecesList[2] = &b_bishop1;
    piecesList[3] = &b_king;
    piecesList[4] = &b_queen;
    piecesList[5] = &b_bishop2;
    piecesList[6] = &b_knight2;
    piecesList[7] = &b_rook2;
    piecesList[8] = &b_pawn1;
    piecesList[9] = &b_pawn2;
    piecesList[10] = &b_pawn3;
    piecesList[11] = &b_pawn4;
    piecesList[12] = &b_pawn5;
    piecesList[13] = &b_pawn6;
    piecesList[14] = &b_pawn7;
    piecesList[15] = &b_pawn8;
    piecesList[16] = &w_pawn1;
    piecesList[17] = &w_pawn2;
    piecesList[18] = &w_pawn3;
    piecesList[19] = &w_pawn4;
    piecesList[20] = &w_pawn5;
    piecesList[21] = &w_pawn6;
    piecesList[22] = &w_pawn7;
    piecesList[23] = &w_pawn8;
    piecesList[24] = &w_rook1;
    piecesList[25] = &w_knight1;
    piecesList[26] = &w_bishop1;
    piecesList[27] = &w_queen;
    piecesList[28] = &w_king;
    piecesList[29] = &w_bishop2;
    piecesList[30] = &w_knight2;
    piecesList[31] = &w_rook2;

    for (int i = 0; i < 32; i++) {
        Piece *piece = piecesList[i];
        piece->texture = LoadTexture(piece->asset);
        piece->pos.x *= FIELD_SIZE;
        piece->pos.y *= FIELD_SIZE;
        printf(ANSI_BLUE "Initialized position x:%f y:%f\n" ANSI_RESET, piece->pos.x, piece->pos.y);
    }
    printf(ANSI_GREEN "======= Initialization finished \n" ANSI_RESET);
}

void DrawBoard() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            int position_x = x * FIELD_SIZE;
            int position_y = y * FIELD_SIZE;
            char houseNumber[50] = "";
            sprintf(houseNumber, "x:%d y:%d", x, y);

            bool isSelectedField = false;
            if (selectedPiece != NULL) {
                IVector2 iPos = convert(selectedPiece->pos);
                isSelectedField = iPos.x == x && iPos.y == y;
            }
            DrawRectangle(position_x,
                          position_y,
                          FIELD_SIZE,
                          FIELD_SIZE,
                          isSelectedField    ? GREEN
                          : (x + y) % 2 == 0 ? WHITE
                                             : DARKGRAY);

            DrawText(houseNumber, position_x, position_y, 16, BLUE);
        }
    }
}

void computePieceAnimation(Piece *piece) {
    IVector2 pos = convert(piece->pos);
    IVector2 target = convert(piece->target);
    if (pos.x == target.x && pos.y == target.y) {
        piece->isMoving = false;
        piece->pos = (Vector2){target.x * FIELD_SIZE, target.y * FIELD_SIZE};
    } else if (piece->isMoving) {
        switch (piece->direction) {
        case downRight:
            piece->pos.x += 2;
            piece->pos.y += 2;
            break;
        case downLeft:
            piece->pos.x -= 2;
            piece->pos.y += 2;
            break;
        case upRight:
            piece->pos.x += 2;
            piece->pos.y -= 2;
            break;
        case upLeft:
            piece->pos.x -= 2;
            piece->pos.y -= 2;
            break;
        case up:
            piece->pos.x += 0;
            piece->pos.y -= 2;
            break;
        case down:
            piece->pos.x += 0;
            piece->pos.y += 2;
            break;
        case left:
            piece->pos.x -= 2;
            piece->pos.y += 0;
            break;
        case right:
            piece->pos.x += 2;
            piece->pos.y += 0;
            break;
        }
    }
}

void DrawPiece(Piece *piece) {
    if (selectedPiece != NULL && selectedPiece == piece && !selectedPiece->isMoving) {
        DrawRectangle(piece->pos.x, piece->pos.y, FIELD_SIZE, FIELD_SIZE, GREEN);
    }
    DrawTexturePro(piece->texture,
                   (Rectangle){
                       0,
                       0,
                       piece->texture.width,
                       piece->texture.height,
                   },
                   (Rectangle){
                       piece->pos.x,
                       piece->pos.y,
                       FIELD_SIZE,
                       FIELD_SIZE,
                   },
                   (Vector2){1, 1},
                   0,
                   WHITE);
}

Direction getDirection(Vector2 pos, Vector2 target) {
    IVector2 iPos = convert(pos);
    IVector2 iTarget = convert(target);
    int x = iTarget.x - iPos.x;
    int y = iTarget.y - iPos.y;
    bool isSimetrical = abs(x) - abs(y) == 0;

    if (x == 0) {
        if (y > 0) {
            return down;
        } else {
            return up;
        }
    } else if (x > 0) {
        if (y < 0 && isSimetrical)
            return upRight;

        if (y == 0)
            return right;

        if (y > 0 && isSimetrical)
            return downRight;

    } else {
        if (y == 0)
            return left;

        if (y > 0 && isSimetrical)
            return downLeft;

        if (y < 0 && isSimetrical)
            return upLeft;
    }
    return nope;
}

bool isDirectionAllowed(Piece *piece, Direction dir) {
    if (dir == nope) {
        return false;
    }
    switch (piece->type) {
    case rook:
        return dir == right || dir == left || dir == up || dir == down;
    case knight:
        return false;
    case bishop:
        return dir == upRight || dir == upLeft || dir == downLeft || dir == downRight;
    case queen:
        return dir == upRight || dir == upLeft || dir == downLeft || dir == downRight || dir == right || dir == left ||
               dir == up || dir == down;
    case king:
        return dir == upRight || dir == upLeft || dir == downLeft || dir == downRight || dir == right || dir == left ||
               dir == up || dir == down;
    case pawn:
        if (piece->color == white) {
            return dir == up || dir == upRight || dir == upLeft;
        } else {
            return dir == down || dir == downRight || dir == downLeft;
        }
        break;
    }
}

void calculatePossibilities(Piece *piece) {
    IVector2 iPos = convert(piece->pos);
    switch (piece->type) {
    case rook:
    case knight:
    case bishop:
        for (int y = iPos.y + 1; y < 7; y++) {
        }
        break;

    case queen:
    case king:
    case pawn:
        break;
    }
}

void onLeftClick(Vector2 pos) { selectedPiece = getPieceFromPosition(pos); }

void onRightClick(Vector2 target) {
    if (selectedPiece != NULL) {
        selectedPiece->target = target;
    }
}

void Draw() {
    DrawBoard();
    for (int i = 0; i < 32; i++) {
        Piece *piece = piecesList[i];
        DrawPiece(piece);
    }
    DrawText(errorMessage, 20, FIELD_SIZE * 8 + 10, 20, RED);
}

void Update() {
    if (selectedPiece != NULL) {
        calculatePossibilities(selectedPiece);
    }
}

int main() {
    InitWindow(FIELD_SIZE * 8, (FIELD_SIZE * 8) + 40, "Chess");
    int currentMonitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(currentMonitor);

    int monitorHeight = GetMonitorHeight(currentMonitor);
    SetWindowPosition(monitorWidth - 80 - FIELD_SIZE * 8, 80);
    fillEmptyBoard();
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        Update();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            onLeftClick(GetMousePosition());
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            onRightClick(GetMousePosition());
        }

        BeginDrawing();
        ClearBackground(BLUE);
        Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
