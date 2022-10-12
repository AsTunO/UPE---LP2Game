#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define windowCenter 195
#define screenWidth 450
#define screenHeight 500



void firstWindow(){

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
}

void scoreWindow() {

    // IMAGE
    Image img = LoadImage("resources/pac_chase.gif");
    Texture2D imgTexture = LoadTextureFromImage(img);
    DrawTexture(imgTexture, GetScreenWidth() / 2 - imgTexture.width / 2, 70, WHITE);

    FILE *filePointer = fopen("data/scoreData/score.csv", "r"); // Open the archive

    int i = 50;

    BeginDrawing();

    DrawText("SCORE", windowCenter, 100 + i, 20, WHITE);

    if(filePointer != NULL) {
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
    }
}

void gameWindow() {
    BeginDrawing();
    DrawText("TODO", windowCenter, 100, 20, DARKGRAY);
    EndDrawing();
}

void actions() {
    // BTN
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
        //TODO
    }

}

int state = 0;

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Game");

    //Image icon = LoadImage("resources/window_icon.gif");
    //SetWindowIcon(icon); // Window Icon

    SetTargetFPS(60); // FPS

    while (!WindowShouldClose()) 
    {
        
        scoreWindow();
        //firstWindow();
        //actions();
    }

    CloseWindow();

    return 0;
}