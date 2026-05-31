#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define ANSI_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"

#define DEBUG(fmt, ...)                                                                                                \
    fprintf(stderr, ANSI_GREEN "[DEBUG] %s:%d: " fmt ANSI_RESET "\n", __func__, __LINE__, ##__VA_ARGS__)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0][0]))

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------
typedef enum {
    EMPTY,

    WP,
    WN,
    WB,
    WR,
    WQ,
    WK,

    BP,
    BN,
    BB,
    BR,
    BQ,
    BK,

    PIECE_COUNT
} PieceType;

typedef struct {
    PieceType type;
    Vector2 pos;
    Texture2D texture;
} Piece;

typedef struct {
    Piece *piece;
    Vector2 pos;
    float width;
    float height;
    Color color;
    Color originalColor;
} Field;

typedef enum {
    initializing,
    playing,
} GameState;

//----------------------------------------------------------------------------------
// Some globals
//----------------------------------------------------------------------------------
int boardWidth = 8;
int boardHeight = 8;
int FIELD_SIZE = 80;
Field *board[8][8];
Field *possibilities[64];
Texture2D piece_textures[PIECE_COUNT];
GameState gameState;
Field *selectedField;

//----------------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------------

void initBoard() {

    piece_textures[WP] = LoadTexture("assets/w-pawn.png");
    piece_textures[WN] = LoadTexture("assets/w-knight.png");
    piece_textures[WB] = LoadTexture("assets/w-bishop.png");
    piece_textures[WR] = LoadTexture("assets/w-rook.png");
    piece_textures[WQ] = LoadTexture("assets/w-queen.png");
    piece_textures[WK] = LoadTexture("assets/w-king.png");

    piece_textures[BP] = LoadTexture("assets/b-pawn.png");
    piece_textures[BN] = LoadTexture("assets/b-knight.png");
    piece_textures[BB] = LoadTexture("assets/b-bishop.png");
    piece_textures[BR] = LoadTexture("assets/b-rook.png");
    piece_textures[BQ] = LoadTexture("assets/b-queen.png");
    piece_textures[BK] = LoadTexture("assets/b-king.png");

    PieceType x_board[8][8] = {{BR, BN, BB, BQ, BK, BB, BN, BR},
                               {BP, BP, BP, BP, BP, BP, BP, BP},

                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},

                               {WP, WP, WP, WP, WP, WP, WP, WP},
                               {WR, WN, WB, WQ, WK, WB, WN, WR}};

    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            Vector2 *pos = malloc(sizeof(Vector2));
            pos->x = column * FIELD_SIZE;
            pos->y = row * FIELD_SIZE;

            Piece *piece = malloc(sizeof(Piece));
            Vector2 *piecePos = malloc(sizeof(Vector2));
            *piecePos = *pos;

            piece->texture = piece_textures[x_board[row][column]];
            piece->type = x_board[row][column];
            piece->pos = *piecePos;

            Field *field = malloc(sizeof(Field));
            field->pos = *pos;
            field->width = 0;
            field->height = 0;
            field->originalColor = (row + column) % 2 == 0 ? WHITE : DARKGRAY;
            field->color = (row + column) % 2 == 0 ? WHITE : DARKGRAY;
            field->piece = piece;

            board[row][column] = field;
        }
    }
}

void registerPossibilitie(int row, int column) { possibilities[column + row * 8] = board[row][column]; }

void rookPossibilities(int i, int row, int column) {
    // up
    int scanRow = row - i;
    if (scanRow >= 0) {
        registerPossibilitie(scanRow, column);
    }

    // down
    scanRow = row + i;
    if (scanRow < 8) {
        registerPossibilitie(scanRow, column);
    }

    // right
    int scanColumn = column + i;
    if (scanColumn < 8) {
        registerPossibilitie(row, scanColumn);
    }

    // left
    scanColumn = column - i;
    if (scanColumn >= 0) {
        registerPossibilitie(row, scanColumn);
    }
}

void knightPossibilities(int i, int row, int column, Piece *pice) {
    // DOWN
    int scanRow = row + 1;
    if (scanRow < 8) {
        registerPossibilitie(scanRow, column);
        // right
        int scanColumn = column + 1;
        if (scanColumn < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // left
        scanColumn = column - 1;
        if (scanColumn >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }

    // UP
    scanRow = row - 1;
    if (scanRow >= 0) {
        registerPossibilitie(scanRow, column);
        // right
        int scanColumn = column - 1;
        if (scanColumn >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // left
        scanColumn = column + 1;
        if (scanColumn < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }
    // RIGHT
    int scanColumn = column + 1;
    if (scanColumn < 8) {
        registerPossibilitie(row, scanColumn);
        // down
        scanRow = row + 1;
        if (scanRow < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // up
        scanRow = row - 1;
        if (scanRow >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }

    // LEFT
    scanColumn = column - 1;
    if (scanColumn >= 0) {
        registerPossibilitie(row, scanColumn);
        // down
        scanRow = row + 1;
        if (scanRow < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // up
        scanRow = row - 1;
        if (scanRow >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }
}

void kingPossibilities(int i, int row, int column, Piece *piece) {

    // DOWN
    int scanRow = row + 1;
    if (scanRow < 8) {
        // right
        int scanColumn = column + 1;
        if (scanColumn < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // left
        scanColumn = column - 1;
        if (scanColumn >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }

    // UP
    scanRow = row - 1;
    if (scanRow >= 0) {
        // right
        int scanColumn = column - 1;
        if (scanColumn >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // left
        scanColumn = column + 1;
        if (scanColumn < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }
    // RIGHT
    int scanColumn = column + 1;
    if (scanColumn < 8) {
        // down
        scanRow = row + 1;
        if (scanRow < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // up
        scanRow = row - 1;
        if (scanRow >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }

    // LEFT
    scanColumn = column - 1;
    if (scanColumn >= 0) {
        // down
        scanRow = row + 1;
        if (scanRow < 8) {
            registerPossibilitie(scanRow, scanColumn);
        }
        // up
        scanRow = row - 1;
        if (scanRow >= 0) {
            registerPossibilitie(scanRow, scanColumn);
        }
    }
}

void bishopPossibilities(int i, int row, int column) {
    // down right
    int scanRow = row + i;
    int scanColumn = column + i;
    if (scanRow < 8 && scanColumn < 8) {
        possibilities[(scanColumn) + (scanRow * 8)] = board[scanRow][scanColumn];
        registerPossibilitie(scanRow, scanColumn);
    }

    // down left
    scanRow = row + i;
    scanColumn = column - i;
    if (scanRow < 8 && scanColumn >= 0) {
        registerPossibilitie(scanRow, scanColumn);
    }

    // up right
    scanRow = row - i;
    scanColumn = column + i;
    if (scanRow >= 0 && scanColumn < 8) {
        registerPossibilitie(scanRow, scanColumn);
    }

    // up left
    scanRow = row - i;
    scanColumn = column - i;
    if (scanRow >= 0 && scanColumn >= 0) {
        registerPossibilitie(scanRow, scanColumn);
    }
}

void pawnPossibilities(int i, int row, int column, Piece *piece) {
    if (piece->type == WP) {
        int scanRow = row - 1;
        if (scanRow >= 0) {
            registerPossibilitie(scanRow, column);
        }
    } else {
        int scanRow = row + 1;
        if (scanRow < 8) {
            registerPossibilitie(scanRow, column);
        }
    }
}

void Update(float timer, float fieldDelay) {
    if (gameState == initializing) {
        for (int row = 0; row < 8; row++) {
            for (int column = 0; column < 8; column++) {
                if (timer / fieldDelay < (row + column))
                    continue;
                Field *field = board[row][column];
                if (field->width < FIELD_SIZE) {
                    field->width += 12 * timer;
                    field->height += 12 * timer;
                } else if ((row + 1) * (column + 1) == ARRAY_SIZE(board)) {
                    gameState = playing;
                }
                DEBUG("Field size %f %f", field->width, field->height);
            }
        }
    } else if (gameState == playing) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DEBUG("LEFT CLICK");
            Vector2 mp = GetMousePosition();
            int row = (int)mp.y / FIELD_SIZE;
            int column = (int)mp.x / FIELD_SIZE;
            DEBUG("Selected row:%d colum:%d", row, column);

            if (selectedField != NULL) {
                selectedField->color = selectedField->originalColor;
            }

            // Restore original color from the last possibilities
            for (int i = 0; i < 64; i++) {
                Field *possibility = possibilities[i];
                if (possibility != NULL) {
                    possibility->color = possibility->originalColor;
                }
                possibilities[i] = NULL;
            }

            Field *field = board[row][column];
            field->color = GREEN;
            selectedField = field;

            // Find possibilities
            for (int i = 1; i < 8; i++) {

                switch (selectedField->piece->type) {
                case WB:
                case BB:
                    bishopPossibilities(i, row, column);
                    break;
                case WR:
                case BR:
                    rookPossibilities(i, row, column);
                    break;
                case WQ:
                case BQ:
                    bishopPossibilities(i, row, column);
                    rookPossibilities(i, row, column);
                    break;
                case WP:
                case BP:
                    pawnPossibilities(i, row, column, selectedField->piece);
                    break;
                case WN:
                case BN:
                    knightPossibilities(i, row, column, selectedField->piece);
                    break;
                case WK:
                case BK:
                    kingPossibilities(i, row, column, selectedField->piece);
                case EMPTY:
                case PIECE_COUNT:
                    break;
                }
            }

            // Painting the  possibilities yellow
            for (int i = 0; i < 64; i++) {
                Field *possibility = possibilities[i];
                if (possibility != NULL) {
                    possibility->color = YELLOW;
                }
            }
            DEBUG("FINISH PAINTING");
        }
    }
}

void Draw(float timer, float fieldDelay) {

    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            Field *field = board[row][column];

            char houseNumber[50] = "";
            sprintf(houseNumber, "r:%d c:%d", row, column);

            DrawRectangle(field->pos.x, field->pos.y, field->width, field->height, field->color);

            DrawText(houseNumber, field->pos.x, field->pos.y, 16, BLUE);

            DrawTexturePro(field->piece->texture,
                           (Rectangle){
                               0,
                               0,
                               field->piece->texture.width,
                               field->piece->texture.height,
                           },
                           (Rectangle){
                               field->piece->pos.x,
                               field->piece->pos.y,
                               FIELD_SIZE,
                               FIELD_SIZE,
                           },
                           (Vector2){1, 1},
                           0,
                           WHITE);
        }
    }
}
int main() {
    float timer = 0.0f;
    float fieldDelay = 0.10f;

    gameState = initializing;
    InitWindow(FIELD_SIZE * boardWidth, boardHeight * FIELD_SIZE, "Chess");
    int currentMonitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(currentMonitor);

    int monitorHeight = GetMonitorHeight(currentMonitor);
    SetWindowPosition(monitorWidth - 80 - FIELD_SIZE * 8, 80);

    SetTargetFPS(30);
    initBoard();
    while (!WindowShouldClose()) {
        timer += GetFrameTime();
        // DEBUG("DRAWING %f", timer);
        // printf("Frame time %f\n", timer);
        Update(timer, fieldDelay);
        BeginDrawing();
        ClearBackground(BLACK);
        Draw(timer, fieldDelay);
        EndDrawing();
    }
    return 0;
}
