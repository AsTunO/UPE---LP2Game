#include "raylib.h"


int main(void)
{

    const int screenWidth = 450;
    const int screenHeight = 500;
    const int windowCenter = ((screenWidth / 2) - 30);


    InitWindow(screenWidth, screenHeight, "Pac-Man");

    Image img = LoadImage("resources/ghost_chase.gif");

    Texture2D texScarfyAnim = LoadTextureFromImage(img);

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
     
        BeginDrawing();

        ClearBackground(BLACK);


        DrawText("PAC-MAN", windowCenter - 20, 10, 20, WHITE);

        DrawTexture(texScarfyAnim, GetScreenWidth() / 2 - texScarfyAnim.width / 2, 140, WHITE);

        DrawRectangleLines(windowCenter - 70, 285, 200, 50, WHITE);
        DrawText("PLAY", windowCenter, 300, 20, DARKGRAY);
        DrawRectangleLines(windowCenter - 70, 385, 200, 50, WHITE);
        DrawText("EXIT", windowCenter, 400, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
