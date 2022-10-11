#include "raylib.h"


int main(void)
{

    const int screenWidth = 450;
    const int screenHeight = 500;
    const int windowCenter = ((screenWidth / 2) - 50);



    InitWindow(screenWidth, screenHeight, "Pac-Man");

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
     
        BeginDrawing();

        ClearBackground(BLACK);


        DrawText("PAC-MAN", windowCenter, 10, 20, WHITE);

        DrawRectangleLines(windowCenter-50, 185, 200, 50, WHITE);
        DrawText("PLAY", windowCenter+20, 200, 20, DARKGRAY);
        DrawRectangleLines(windowCenter - 50, 385, 200, 50, WHITE);
        DrawText("EXIT", windowCenter+20, 400, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}