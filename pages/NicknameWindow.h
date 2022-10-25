#include "raylib.h"

void nicknameWindow()
{

    char name[10] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = {GetScreenWidth() / 2.0f - 100, 180, 225, 50};
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

        DrawText("NICKNAME", GetScreenHeight() / 2, 100, 20, WHITE);

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
                if ((key >= 32) && (key <= 125) && (letterCount < 9))
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
            if (letterCount < 9)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
            }
        }

        DrawRectangleLines(GetScreenHeight() / 2, 385, 200, 50, WHITE);
        DrawText("COMEÇAR", GetScreenHeight() / 2, 400, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();

    // Check if any key is pressed
    // NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
}