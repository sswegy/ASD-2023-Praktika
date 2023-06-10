#define _CRT_SECURE_NO_WARNINGS
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

struct tile{
    Rectangle button;
    Color buttonColor;
    bool buttonPressed;
    int points;
    char* text;
};

struct letter {
    Color tileColor;
    int points;
    char symbol;
    int cordX;
    int cordY;
};

void drawGame(struct tile tileGrid[15][15], struct tile returnButton, struct tile endTurnButton, struct tile options[7]) {
    // risuvame grida
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 15; y++) {
            DrawRectangleRec(tileGrid[x][y].button, tileGrid[x][y].buttonColor);
            DrawText(tileGrid[x][y].text, tileGrid[x][y].button.x + 7, tileGrid[x][y].button.y + 13, 28, WHITE);
        }
    }
    // Risuvame returnButton
    DrawRectangleRec(returnButton.button, returnButton.buttonColor);
    DrawText(returnButton.text, returnButton.button.x + 5, returnButton.button.y + 10, 28, WHITE);
    // Risuvame endTurnButton
    DrawRectangleRec(endTurnButton.button, endTurnButton.buttonColor);
    DrawText(endTurnButton.text, endTurnButton.button.x + 5, endTurnButton.button.y + 10, 28, WHITE); 

    //Nashite bukvi
    for (int i = 0; i < 7; i++) {
        DrawRectangleRec(options[i].button, options[i].buttonColor);
        DrawText(options[i].text, options[i].button.x + 7, options[i].button.y + 13, 30, WHITE);
    }
}

int main() {
    // Initialize Raylib
    InitWindow(1000, 1000, "Raylib Button");

    struct tile tileGrid[15][15]; // { corX, corY, sizeX, sizeY } sizeX = 50

    struct letter currentWord[15]; //maksimalnata duljina na dumata e 15 poleta
    int wordLength = 0;

    struct tile options[7];
    int corX = 300;
    //!!!VREMENO!!!
    for (int i = 0; i < 7; i++) {
        options[i].button = (Rectangle){ corX, 55 * 15 + 5, 80, 60 };
        options[i].buttonColor = ORANGE;
        options[i].points = i*10;
        options[i].buttonPressed = false;
        options[i].text = (char*)malloc((sizeof(char) * 4) + 1); // bukva + space + tochki
        sprintf(options[i].text, "%c %d", 'A' + i, options[i].points);
        corX += 85;
    }

    struct tile returnButton;
    returnButton.button = (Rectangle){ 10, 55*15 + 5, 105, 50 };
    returnButton.buttonColor = RED;
    returnButton.buttonPressed = false;
    returnButton.text = (char*)malloc((sizeof(char) * 6) + 1); // Allocate memory for "n_" plus null terminator
    sprintf(returnButton.text, "Return");

    struct tile endTurnButton;
    endTurnButton.button = (Rectangle){ 120, 55 * 15 + 5, 140, 50 };
    endTurnButton.buttonColor = LIME;
    endTurnButton.buttonPressed = false;
    endTurnButton.text = (char*)malloc((sizeof(char) * 7) + 1); // Allocate memory for "n_" plus null terminator
    sprintf(endTurnButton.text, "End Turn");

    corX = 5;
    int corY = 5;

    Color gridColors[15][15] ={ 
    {MAROON, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY,LIGHTGRAY, LIGHTGRAY, MAROON, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, MAROON},
    {LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY},
    {LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY,LIGHTGRAY, SKYBLUE, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY},
    {SKYBLUE, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY,LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, SKYBLUE},
    {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK,LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY},
    {LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY},
    {LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY,LIGHTGRAY, SKYBLUE, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY},
    {MAROON, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY,LIGHTGRAY, LIGHTGRAY, GOLD, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, MAROON},
    {LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY,LIGHTGRAY, SKYBLUE, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY},
    {LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY},
    {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK,LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY},
    {SKYBLUE, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY,LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, SKYBLUE},
    {LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY,LIGHTGRAY, SKYBLUE, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY},
    {LIGHTGRAY, PINK, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, DARKBLUE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, PINK, LIGHTGRAY},
    {MAROON, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY,LIGHTGRAY, LIGHTGRAY, MAROON, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, SKYBLUE, LIGHTGRAY, LIGHTGRAY, MAROON},
    };

    //Initzializaciq na grid-a
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 15; y++) {
            tileGrid[x][y].button = (Rectangle){ corX, corY, 60, 50 };
            corY += 55;
            tileGrid[x][y].buttonColor = gridColors[x][y];
            tileGrid[x][y].points = 0;
            tileGrid[x][y].buttonPressed = false;
            tileGrid[x][y].text = (char*)malloc((sizeof(char) * 4) + 1); // bukva + space + tochki
            sprintf(tileGrid[x][y].text, " -- "); //sprintf(tileGrid[x][y].text, "_ %d", tileGrid[x][y].points); 
        }
        corX += 65;
        corY = 5;
    }


    //Game Loop-a
    while (!WindowShouldClose()) {
        //Kato cqlo slaganeto na bukvite
        for (int x = 0; x < 15; x++) {
            for (int y = 0; y < 15; y++) {
                if (CheckCollisionPointRec(GetMousePosition(), tileGrid[x][y].button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tileGrid[x][y].buttonPressed == false && wordLength < 15) {
                    currentWord[wordLength].tileColor = tileGrid[x][y].buttonColor;
                    currentWord[wordLength].cordX = x;
                    currentWord[wordLength].cordY = y;
                    currentWord[wordLength].points = 1; // !!!!!!!Vremenno!!!!!!!
                    currentWord[wordLength].symbol = 'A'; //!!!!!!!!Sushto vremenno!!!!!!!!!!
                    sprintf(tileGrid[x][y].text, "%c %d", currentWord[wordLength].symbol, currentWord[wordLength].points);
                    wordLength++;
                    tileGrid[x][y].buttonPressed = true;
                    tileGrid[x][y].buttonColor = ORANGE;
                }
            }
        }

        //Return boton logika
        if (CheckCollisionPointRec(GetMousePosition(), returnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.buttonPressed == false) {
            for (int i = 0; i < wordLength; i++) {
                tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonColor = currentWord[i].tileColor;
                sprintf(tileGrid[currentWord[i].cordX][currentWord[i].cordY].text, " -- ");
                tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonPressed = false;
            }
            wordLength = 0;
        }
        // Tva ima problemi kato sa kato sa 3 prilepeni na zavoi!!!!!!!!!!!!
        if (CheckCollisionPointRec(GetMousePosition(), endTurnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && endTurnButton.buttonPressed == false) {
            for (int n = 0; n < wordLength - 2; n++) {
                if ((currentWord[n].cordX != currentWord[n + 1].cordX) && (currentWord[n].cordY != currentWord[n + 1].cordY)) { //  if(!((currentWord[n].cordX == currentWord[n+1].cordX == currentWord[n + 2].cordX) || (currentWord[n].cordY == currentWord[n + 1].cordY == currentWord[n + 2].cordY)))
                    for (int i = 0; i < wordLength; i++) {
                        tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonColor = currentWord[i].tileColor;
                        sprintf(tileGrid[currentWord[i].cordX][currentWord[i].cordY].text, " -- ");
                        tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonPressed = false;
                    }
                    break;
                }
            }
            wordLength = 0;
        }

        //Risuvame
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawGame(tileGrid, returnButton, endTurnButton, options);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
