#include "raylib.h"

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
        //TODO
    }
}
