#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define windowCenter 195
#define screenWidth 450
#define screenHeight 500
#define MAX_INPUT_CHARS 9

void scoreWindow();
void homeBtn();
void scoreBtn();
void gameWindow();
void runBtn();
void nicknameWindow();
bool IsAnyKeyPressed();
void playBtn();
void firstWindow();

void scoreWindow()
{
    int caseNum = 0;
    BeginDrawing();
    ClearBackground(BLACK);

    while (!WindowShouldClose())
    {
        // IMAGE
        Image img = LoadImage("resources/pac_chase.gif");
        Texture2D imgTexture = LoadTextureFromImage(img);
        DrawTexture(imgTexture, GetScreenWidth() / 2 - imgTexture.width / 2, 150, WHITE);

        DrawRectangleLines(10, 85, 100, 50, WHITE);
        DrawText("HOME", 30, 100, 20, RED);
        homeBtn();

        FILE *filePointer = fopen("data/scoreData/score.csv", "r"); // Open the archive

        int i = 50;

        DrawText("SCORE", windowCenter, 30, 20, WHITE);

        if (filePointer != NULL && caseNum == 0)
        {
            while (!feof(filePointer))
            {
                char line[100];
                fgets(line, 100, filePointer);

                BeginDrawing();
                DrawText(line, windowCenter - 30, 200 + i, 20, DARKGRAY);
                EndDrawing();

                i = i + 50;
            }
            fclose(filePointer);

            caseNum = 1;
        }

        EndDrawing();
    }
    CloseWindow();
}

void homeBtn()
{
    // PLAY BTN
    Rectangle btnBounds = {10, 85, 100, 50};
    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f}; // MOUSE POINTER

    // BTN CONDITIONS
    mousePoint = GetMousePosition();
    btnAction = false;

    // Check button state
    if (CheckCollisionPointRec(mousePoint, btnBounds))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            btnAction = true;
    }
    else
        btnState = 0;

    if (btnAction) // DO THE NEXT ACTION
    {
        firstWindow();
    }
}

void scoreBtn()
{
    // SCORE BTN
    Rectangle btnBounds = {windowCenter - 70, 385, 200, 50};
    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f}; // MOUSE POINTER

    // BTN CONDITIONS
    mousePoint = GetMousePosition();
    btnAction = false;

    // Check button state
    if (CheckCollisionPointRec(mousePoint, btnBounds))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            btnAction = true;
    }
    else
        btnState = 0;

    if (btnAction) // DO THE NEXT ACTION
    {
        scoreWindow();
    }
}

void gameWindow()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("TODO", windowCenter, 100, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
}

void runBtn()
{
    // PLAY BTN
    Rectangle btnBounds = {windowCenter - 70, 385, 200, 50};
    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f}; // MOUSE POINTER

    // BTN CONDITIONS
    mousePoint = GetMousePosition();
    btnAction = false;

    // Check button state
    if (CheckCollisionPointRec(mousePoint, btnBounds))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            btnAction = true;
    }
    else
        btnState = 0;

    if (btnAction) // DO THE NEXT ACTION
    {
        gameWindow();
    }
}


void nicknameWindow()
{

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = {screenWidth / 2.0f - 100, 180, 225, 50};
    bool mouseOnText = false;

    int framesCounter = 0;

    while (!WindowShouldClose())
    {

        BeginDrawing();
        // IMAGE
        Image img = LoadImage("resources/bar_fruit.gif");
        Texture2D imgTexture = LoadTextureFromImage(img);
        DrawTexture(imgTexture, GetScreenWidth() / 2 - imgTexture.width / 2, 30, WHITE);

        ClearBackground(BLACK);

        DrawRectangleLines(10, 85, 100, 50, WHITE);
        DrawText("HOME", 30, 100, 20, RED);
        homeBtn();

        DrawText("NICKNAME", windowCenter - 20, 100, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            mouseOnText = true;
        else
            mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed(); // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText)
            framesCounter++;
        else
            framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
            }
        }

        DrawRectangleLines(windowCenter - 70, 385, 200, 50, WHITE);
        DrawText("COMEÇAR", windowCenter - 20, 400, 20, GREEN);

        runBtn();

        EndDrawing();
    }

    CloseWindow();

    // Check if any key is pressed
    // NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
}

bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126))
        keyPressed = true;

    return keyPressed;
}

void playBtn()
{
    // PLAY BTN
    Rectangle btnBounds = {windowCenter - 70, 285, 200, 50};
    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f}; // MOUSE POINTER

    // BTN CONDITIONS
    mousePoint = GetMousePosition();
    btnAction = false;

    // Check button state
    if (CheckCollisionPointRec(mousePoint, btnBounds))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            btnAction = true;
    }
    else
        btnState = 0;

    if (btnAction) // DO THE NEXT ACTION
    {
        nicknameWindow();
    }
}

void firstWindow()
{
    InitAudioDevice();

    Music music = LoadMusicStream("resources/pacman_ringtone.mp3");

    PlayMusicStream(music);

    while (!WindowShouldClose())
    {

        UpdateMusicStream(music);

        // IMAGE
        Image img = LoadImage("resources/ghost_chase.gif");
        Texture2D imgTexture = LoadTextureFromImage(img);

        // TEXTS
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("PAC-MAN", windowCenter - 20, 10, 20, WHITE);
        DrawTexture(imgTexture, GetScreenWidth() / 2 - imgTexture.width / 2, 140, WHITE);
        DrawRectangleLines(windowCenter - 70, 285, 200, 50, WHITE);
        DrawText("PLAY", windowCenter, 300, 20, DARKGRAY);
        DrawRectangleLines(windowCenter - 70, 385, 200, 50, WHITE);
        DrawText("SCORE", windowCenter, 400, 20, DARKGRAY);
        EndDrawing();
        playBtn();
        scoreBtn();
    }
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Game");
    // Image icon = LoadImage("resources/window_icon.gif");
    // SetWindowIcon(icon); // Window Icon
    SetTargetFPS(60); // FPS
    firstWindow();

    return 0;
}
