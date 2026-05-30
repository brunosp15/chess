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
} Field;

//----------------------------------------------------------------------------------
// Some globals
//----------------------------------------------------------------------------------
int boardWidth = 8;
int boardHeight = 8;
int FIELD_SIZE = 80;
Field *board[8][8];
Texture2D piece_textures[PIECE_COUNT];

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
    piece_textures[BK] = LoadTexture("assets/b-kinght.png");

    PieceType x_board[8][8] = {{BR, BN, BB, BQ, BK, BB, BN, BR},
                               {BP, BP, BP, BP, BP, BP, BP, BP},

                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},

                               {WP, WP, WP, WP, WP, WP, WP, WP},
                               {WR, WN, WB, WQ, WK, WB, WN, WR}};

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Vector2 *pos = malloc(sizeof(Vector2));
            pos->x = x * FIELD_SIZE;
            pos->y = y * FIELD_SIZE;

            Piece *piece = malloc(sizeof(Piece));
            Vector2 *piecePos = malloc(sizeof(Vector2));
            *piecePos = *pos;

            piece->texture = piece_textures[x_board[x][y]];
            piece->type = x_board[x][y];
            piece->pos = *piecePos;

            Field *field = malloc(sizeof(Field));
            field->pos = *pos;
            field->width = 0;
            field->height = 0;
            field->color = (x + y) % 2 == 0 ? WHITE : DARKGRAY;
            field->piece = piece;

            board[x][y] = field;
        }
    }
}
void Update(float timer, float fieldDelay) {

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (timer / fieldDelay < (x + y))
                continue;
            Field *field = board[x][y];
            if (field->width < FIELD_SIZE) {
                field->width += 12 * timer;
                field->height += 12 * timer;
            }
            DEBUG("Field size %f %f", field->width, field->height);
        }
    }
}

void Draw(float timer, float fieldDelay) {

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Field *field = board[x][y];

            char houseNumber[50] = "";
            sprintf(houseNumber, "x:%d y:%d", x, y);

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

    InitWindow(FIELD_SIZE * boardWidth, boardHeight * FIELD_SIZE, "Chess");
    SetTargetFPS(30);
    initBoard();
    while (!WindowShouldClose()) {
        timer += GetFrameTime();
        // printf("Frame time %f\n", timer);
        Update(timer, fieldDelay);
        BeginDrawing();
        ClearBackground(BLACK);
        Draw(timer, fieldDelay);
        EndDrawing();
    }
    return 0;
}
