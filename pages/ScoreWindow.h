#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct ScoreData
{
    char *pos;
    char *name;
    char *score;
    struct ScoreData *next;
} ScoreData;

ScoreData *getData()
{
    FILE *filePointer = fopen("data/scoreData/score.csv", "r");

    if (filePointer != NULL)
    {
        ScoreData *headerPointer = NULL;
        while (!feof(filePointer))
        {
            char *pos; char *name; char *score;
            fscanf(filePointer, "%s;%s;%s", pos, name, score);

            if (headerPointer == NULL) {
                headerPointer->pos = pos;
                headerPointer->name = name;
                headerPointer->score = score;
                headerPointer->next = NULL;
            } else {
                ScoreData *current = headerPointer;
                while (current->next != NULL) {
                    current = current->next;
                }
                current = current->next;
                current->pos = pos;
                current->name = name;
                current->score = score;
                current->next = NULL;
            }
        }
        fclose(filePointer);
        return headerPointer;
    }
}

void ScoreWindow()
{
    ScoreData *dataToDraw = getData();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2, 150, 40, GREEN);
    int yPosi = 150;
    if (dataToDraw != NULL) {
        ScoreData *current = dataToDraw;
        while(current->next != NULL) {
            DrawText(current->pos, GetScreenWidth() / 2 - MeasureText(current->pos, 40) / 2, yPosi, 25, GREEN);
            DrawText(current->name, GetScreenWidth() / 2 - MeasureText(current->name, 25) / 2, yPosi, 25, GREEN);
            DrawText(current->score, GetScreenWidth() / 2 - MeasureText(current->score, 25) / 2, yPosi, 25, GREEN);
            yPosi += 50;
        }
    }
    EndDrawing();
}