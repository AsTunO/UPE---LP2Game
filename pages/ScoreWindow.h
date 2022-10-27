#include "stdio.h"
#include "raylib.h"

void ScoreWindow(int caseOf)
{
    FILE *filePointer = fopen("data/scoreData/score.csv", "r"); // Open the archive

    BeginDrawing();
    ClearBackground(BLACK);

    int i = 50;

    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2, 150, 40, GREEN);

    if (filePointer != NULL)
    {
        while (!feof(filePointer))
        {
            char line[100];
            fgets(line, 100, filePointer);
            if (caseOf == 0)
            {
                BeginDrawing();
                DrawText(line, GetScreenWidth() / 2 - MeasureText(line, 20) / 2, GetScreenHeight() / 2 + i, 20, WHITE);
                EndDrawing();

                i = i + 50;
            }
        }
        caseOf = 1;
        fclose(filePointer);
    }
    EndDrawing();
}