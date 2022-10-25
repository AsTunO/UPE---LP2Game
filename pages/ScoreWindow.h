#include "raylib.h"
#include "stdio.h"

#include "./components/HomeButton.h"

void ScoreWindow()
{
    FILE *filePointer = fopen("data/scoreData/score.csv", "r"); // Open the archive

    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleLines(10, 85, 100, 50, WHITE);
    DrawText("HOME", 30, 100, 20, RED);
    homeBtn();

    int i = 50;

    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2, 150, 40, GREEN);

    if (filePointer != NULL)
    {
        while (!feof(filePointer))
        {
            char line[100];
            fgets(line, 100, filePointer);

            BeginDrawing();
            DrawText(line,GetScreenWidth() / 2 - MeasureText(line, 20) / 2, GetScreenHeight() / 2 + i, 20, WHITE);
            EndDrawing();

            i = i + 50;
        }
        fclose(filePointer);
    }
    EndDrawing();
}