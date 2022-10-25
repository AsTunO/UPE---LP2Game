#include "raylib.h"
#include "stdio.h"

// Pages
#include "./pages/ScoreWindow.h"
#include "./pages/OptionsWindow.h"
#include "./pages/NicknameWindow.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

#define GB_GREEN01 \
    (Color) { 155, 188, 15, 255 }
#define GB_GREEN02 \
    (Color) { 110, 150, 27, 255 }
#define GB_GREEN03 \
    (Color) { 48, 98, 48, 255 }
#define GB_GREEN04 \
    (Color) { 15, 56, 15, 255 }

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
    int fruitsCounter;
    bool allowMove;
    int counterTail;
} Snake;

typedef struct Food
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

typedef struct ScreenSettings
{
    int screenWidth;
    int screenHeight;
    int framesCounter;
    int optionSelect;
    char currentScreen;
    bool gameOver;
    bool pause;
    bool exitGame;
} ScreenSettings;


//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static Vector2 offset = {0};
static ScreenSettings configs;
static Music sound;
static int optionMenu = 0;
static Font font = {0};

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static char menu(void);          // Initialize menu and select options
static void InitScoreGame(void); // Initialize game
static void InitSound(void);     // Init sound settings
static void UpdateGame(void);    // Update game (one frame)
static void DrawCurrentScreen(void);
static void UnloadGame(void);      // Unload game
static void UpdateDrawFrame(void); // Update and Draw (one frame)
static void GameWindow();
static void TitleWindow();

    //------------------------------------------------------------------------------------
    // Program main entry point
    //------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------

    InitWindow(0, 0, "Snake Game");

    configs.screenWidth = GetScreenWidth();
    configs.screenHeight = GetScreenHeight();
    configs.framesCounter = 0;
    configs.pause = false;
    configs.gameOver = false;
    configs.currentScreen = 'T'; // L - Logo; T - Title; M - Mode selector; G - Gameplay; O - Options; S - Score;
    configs.optionSelect = 0;

    font = LoadFont("resources/alpha_beta.png");

    ToggleFullscreen();
    InitScoreGame();
    DisableCursor();
    SetTargetFPS(60);
    InitSound();

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose() && !configs.exitGame) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateMusicStream(sound);
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();  // Unload loaded data (textures, sounds, models...)
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void GameWindow()
{

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!configs.gameOver)
    {
        // Draw BackGround
        // TODO

        // Draw snake
        for (int i = 0; i < snake->counterTail; i++)
            DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

        

        Image img = LoadImage("resources/images/semaforo-_1_.gif");
        Texture2D imgTexture = LoadTextureFromImage(img);
        DrawTextureV(imgTexture, fruit.position, WHITE);
        // Draw fruit to pick
        //ImageDrawPixelV(&cat, fruit.position, fruit.color);

        if (configs.pause)
        {
            DrawRectangle(70, 95, 180, 27, DARKGRAY);
            DrawText(TextFormat("Fruits eated: %i", snake->fruitsCounter), 76, 100, 20, GREEN);
            DrawText("GAME PAUSED", configs.screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, configs.screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText("Press [G] to check us in GitHub!", GetScreenWidth() / 2 - MeasureText("Press [G] to check us in GitHub!", 20) / 2, GetScreenHeight() / 2, 20, GRAY);
    }
    EndDrawing();
}

void TitleWindow()
{
    BeginDrawing();
    ClearBackground(DARKGREEN);
    // IMAGE
    Image img = LoadImage("resources/images/SnakeBackground.gif");
    Texture2D imgTexture = LoadTextureFromImage(img);
    DrawTexture(imgTexture, GetScreenWidth() / 2 + 300, 100, WHITE);
    DrawText("SNAKE GAME", GetScreenWidth() / 2 - MeasureText("SNAKE GAME", 40) / 2, 150, 40, BLACK);
    DrawText("START", GetScreenWidth() / 2 - MeasureText("START", 20) / 2, GetScreenHeight() / 2 + 100, 20, (configs.optionSelect == 0) ? WHITE : BLACK);
    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 20) / 2, GetScreenHeight() / 2 + 150, 20, (configs.optionSelect == 1) ? WHITE : BLACK);
    DrawText("OPTIONS", GetScreenWidth() / 2 - MeasureText("OPTIONS", 20) / 2, GetScreenHeight() / 2 + 200, 20, (configs.optionSelect == 2) ? WHITE : BLACK);
    DrawText("EXIT", GetScreenWidth() / 2 - MeasureText("EXIT", 20) / 2, GetScreenHeight() / 2 + 250, 20, (configs.optionSelect == 3) ? WHITE : BLACK);
    EndDrawing();
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitScoreGame(void)
{
    configs.framesCounter = 0;
    configs.gameOver = false;
    configs.pause = false;

    snake->counterTail = 1;
    snake->allowMove = false;
    snake->fruitsCounter = 0;

    offset.x = configs.screenWidth % SQUARE_SIZE;
    offset.y = configs.screenHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){offset.x / 2, offset.y / 2};
        snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        snake[i].speed = (Vector2){SQUARE_SIZE, 0};

        if (i == 0)
            snake[i].color = DARKGREEN;
        else
            snake[i].color = GREEN;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = (Vector2){0.0f, 0.0f};
    }

    fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    fruit.color = SKYBLUE;
    fruit.active = false;

    SetExitKey(KEY_F10);
}

// Update game (one frame)
void UpdateGame(void)
{
    if (configs.currentScreen == 'T')
    {
        if (IsKeyPressed(KEY_W))
        {
            configs.optionSelect--;
        }
        else if (IsKeyPressed(KEY_S))
        {
            configs.optionSelect++;
        }

        if (configs.optionSelect < 0)
            configs.optionSelect = 0;
        else if (configs.optionSelect > 3)
            configs.optionSelect = 3;

        if (IsKeyPressed(KEY_ENTER))
        {
            switch (configs.optionSelect)
            {
            case 0:
                configs.currentScreen = 'G';
                break;
            case 1:
                configs.currentScreen = 'S';
                break;
            case 2:
                configs.currentScreen = 'O';
                break;
            case 3:
                configs.exitGame = true;
                break;
            default:
                TitleWindow();
                break;

            }
        }
    }

    else if (configs.currentScreen == 'G')
    {
        if (!configs.gameOver)
        {
            if (IsKeyPressed(KEY_ESCAPE))
                configs.pause = !configs.pause;

            if (!configs.pause)
            {
                // Player control
                if (IsKeyPressed(KEY_D) && (snake[0].speed.x == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){SQUARE_SIZE, 0};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_A) && (snake[0].speed.x == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_W) && (snake[0].speed.y == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){0, -SQUARE_SIZE};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_S) && (snake[0].speed.y == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){0, SQUARE_SIZE};
                    snake->allowMove = false;
                }

                // Snake movement
                for (int i = 0; i < snake->counterTail; i++)
                    snakePosition[i] = snake[i].position;

                if ((configs.framesCounter % 5) == 0)
                {
                    for (int i = 0; i < snake->counterTail; i++)
                    {
                        if (i == 0)
                        {
                            snake[0].position.x += snake[0].speed.x;
                            snake[0].position.y += snake[0].speed.y;
                            snake->allowMove = true;
                        }
                        else
                            snake[i].position = snakePosition[i - 1];
                    }
                }

                // Wall behaviour
                if (((snake[0].position.x) > (configs.screenWidth - offset.x)) ||
                    ((snake[0].position.y) > (configs.screenHeight - offset.y)) ||
                    (snake[0].position.x < 0) || (snake[0].position.y < 0))
                {
                    configs.gameOver = true;
                }

                // Collision with yourself
                for (int i = 1; i < snake->counterTail; i++)
                {
                    if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y))
                        configs.gameOver = true;
                }

                // Fruit position calculation
                if (!fruit.active)
                {
                    fruit.active = true;
                    fruit.position = (Vector2){GetRandomValue(0, (configs.screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (configs.screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};

                    for (int i = 0; i < snake->counterTail; i++)
                    {
                        while ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                        {
                            fruit.position = (Vector2){GetRandomValue(0, (configs.screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (configs.screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                            i = 0;
                        }
                    }
                }

                // Collision
                if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
                    (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))
                {
                    snake->fruitsCounter += 1;
                    snake[snake->counterTail].position = snakePosition[snake->counterTail - 1];
                    snake->counterTail += 1;
                    fruit.active = false;
                }

                configs.framesCounter++;
            }
        }
        else
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                InitScoreGame();
                configs.gameOver = false;
            }
            if (IsKeyPressed(KEY_G))
            {
                OpenURL("https://github.com/astuno/UPE---LP2Game");
            }
        }
    }
}

void DrawCurrentScreen() {
    switch (configs.currentScreen)
    {
    case 'G':
        nicknameWindow();
        GameWindow();
        break;
    case 'S':
        ScoreWindow();
        break;
    case 'O':
        OptionsWindow();
        break;
    default:
        TitleWindow();
        break;
    }
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
    UnloadMusicStream(sound);
    CloseAudioDevice();
    UnloadFont(font);
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawCurrentScreen();
}

void InitSound(void)
{
    InitAudioDevice();
    sound = LoadMusicStream("./resources/sounds/backgroundSound.mp3");
    SetMasterVolume(1);
    PlayMusicStream(sound);
}