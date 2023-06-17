#define _CRT_SECURE_NO_WARNINGS
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>


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
            DrawText(tileGrid[x][y].text, tileGrid[x][y].button.x + 7, tileGrid[x][y].button.y + 13, 28, RAYWHITE);
        }
    }
    // Risuvame returnButton
    DrawRectangleRec(returnButton.button, returnButton.buttonColor);
    DrawText(returnButton.text, returnButton.button.x + 5, returnButton.button.y + 10, 28, RAYWHITE);
    // Risuvame endTurnButton
    DrawRectangleRec(endTurnButton.button, endTurnButton.buttonColor);
    DrawText(endTurnButton.text, endTurnButton.button.x + 5, endTurnButton.button.y + 10, 28, RAYWHITE);

    //Nashite bukvi
    for (int i = 0; i < 7; i++) {
        DrawRectangleRec(options[i].button, options[i].buttonColor);
        DrawText(options[i].text, options[i].button.x + 7, options[i].button.y + 13, 30, RAYWHITE);
    }
}

void clearGrid(struct tile tileGrid[15][15], struct letter currentWord[15], int *wordLength, struct tile options[7]) {
    for (int i = 0; i < *wordLength; i++) {
        tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonColor = currentWord[i].tileColor;
        sprintf(tileGrid[currentWord[i].cordX][currentWord[i].cordY].text, " -- ");
        tileGrid[currentWord[i].cordX][currentWord[i].cordY].buttonPressed = false;
    }
    *wordLength = 0;

    for (int i = 0; i < 7; i++) {
        options[i].buttonPressed = false;
        options[i].buttonColor = ORANGE;
    }
}

// Tazi funkciq polzvame za da si pomognem proverqvaneto na tova dali novata duma e dolepena do drugite
// Proverqva dali specialno tozi kordinat se sreshta v dumata
bool isInTheCurrentWord(struct letter currentWord[15], int wordLength, int x, int y) {
    for (int i = 0; i < wordLength; i++) {
        if (currentWord[i].cordX == x && currentWord[i].cordY == y) {
            return true;
        }
    }
    return false;
}

bool isRayWhite(Color buttonColor) {
    if (buttonColor.r == 245 && buttonColor.g == 245 && buttonColor.b == 245)
        return true;
    else return false;
}

int generateRandomLetter(int letters[26], int *numberOfLettersLeft) {
    if (*numberOfLettersLeft == 0)
        return -1;
    int letter;
    while (1) {
        letter = rand() % 26; // dava sluchajna bukva
        if (letters[letter] != 0) break;
    }
    *numberOfLettersLeft -= 1;
    letters[letter]--;
    return letter;
}

void newOptions(struct tile options[7], int letters[26], int letterPoints[26], int *numberOfLettersLeft) {
    for (int i = 0; i < 7; i++) {
        if (isRayWhite(options[i].buttonColor) == true) {
            int letter = generateRandomLetter(letters, numberOfLettersLeft);
            if (letter == -1)
                break;
            options[i].points = letterPoints[letter];
            sprintf(options[i].text, "%c %d", 'A' + letter, options[i].points);
            options[i].buttonColor = ORANGE;
        }
    }
}

int main() {
    srand(time(0));

    // Initialize Raylib
    InitWindow(1000, 1000, "Raylib Button");

    struct tile tileGrid[15][15]; // { corX, corY, sizeX, sizeY } sizeX = 50
    bool emptyBoard = true; // Za da proverka na purviq hod

    struct letter currentWord[15]; //maksimalnata duljina na dumata e 15 poleta
    int wordLength = 0;


    int letterPoints[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };

    int letters[26] = { 9, 2, 2, 4 ,12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1 };
    int numberOfLettersLeft = 98;

    struct tile options[7];
    int corX = 300;
    //!!!VREMENO!!!
    for (int i = 0; i < 7; i++) {
        options[i].button = (Rectangle){ corX, 55 * 15 + 5, 70, 60 };
        options[i].buttonColor = ORANGE;
        options[i].buttonPressed = false;
        options[i].text = (char*)malloc((sizeof(char) * 4) + 1); // bukva + space + tochki
        int letter = generateRandomLetter(letters, &numberOfLettersLeft, &numberOfLettersLeft);
        options[i].points = letterPoints[letter];
        sprintf(options[i].text, "%c %d", 'A' + letter, options[i].points);
        corX += 85;
    }

    struct tile returnButton;
    returnButton.button = (Rectangle){ 10, 55*15 + 5, 105, 50 };
    returnButton.buttonColor = RED;
    returnButton.buttonPressed = false;
    returnButton.text = (char*)malloc((sizeof(char) * 6) + 1); // Return + null terminator
    sprintf(returnButton.text, "Return");

    struct tile endTurnButton;
    endTurnButton.button = (Rectangle){ 120, 55 * 15 + 5, 140, 50 };
    endTurnButton.buttonColor = LIME;
    endTurnButton.buttonPressed = false;
    endTurnButton.text = (char*)malloc((sizeof(char) * 8) + 1); // End Turn + null terminator
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

    // Pravilniq cvqt (Color){ 250, 118, 30, 255 }
    int pressedOptionsButton = -1;
    int vremmeno = 0;

    //Game Loop-a
    while (!WindowShouldClose()) {
        for (int i = 0; i < 7; i++) {
            if (isRayWhite(options[i].buttonColor) == true) {
                options[i].buttonColor = RAYWHITE; 
            }else if (pressedOptionsButton == -1 || pressedOptionsButton != i)
                options[i].buttonColor = ORANGE; // samo edin button moje da e ocveten/ clicknat
            if (CheckCollisionPointRec(GetMousePosition(), options[i].button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && options[i].buttonPressed == false && isRayWhite(options[i].buttonColor) == false) {
                pressedOptionsButton = i;
                options[i].buttonColor = (Color){ 250, 118, 30, 255 }; // red, green, blue, mislq che e opacity 
            }
        }


        // Postavqneto na bukvite na poleto
        for (int x = 0; x < 15; x++) {
            for (int y = 0; y < 15; y++) {
                if (CheckCollisionPointRec(GetMousePosition(), tileGrid[x][y].button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
                    && tileGrid[x][y].buttonPressed == false && wordLength < 15 && pressedOptionsButton != -1) {

                    currentWord[wordLength].tileColor = tileGrid[x][y].buttonColor; // tova ni trqbva za tochkite posle
                    currentWord[wordLength].cordX = x;
                    currentWord[wordLength].cordY = y;
                    currentWord[wordLength].points = options[pressedOptionsButton].points;
                    currentWord[wordLength].symbol = options[pressedOptionsButton].text[0]; // za checkValidWord ili tam kakto se kazva
                    sprintf(tileGrid[x][y].text, "%c %d", currentWord[wordLength].symbol, currentWord[wordLength].points);
                    wordLength++;
                    tileGrid[x][y].buttonPressed = true;
                    tileGrid[x][y].buttonColor = ORANGE;

                    options[pressedOptionsButton].buttonColor = RAYWHITE;
                    pressedOptionsButton = -1;
                }
            }
        }

        //Return boton logika
        if (CheckCollisionPointRec(GetMousePosition(), returnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.buttonPressed == false) {
            clearGrid(tileGrid, currentWord, &wordLength, options);
        }

        // Za v momenta prosto zatvurdqva bukvite, no kato cqlo tuk trqbva da e proverkata za istinska duma!!!!!!
        if (CheckCollisionPointRec(GetMousePosition(), endTurnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && endTurnButton.buttonPressed == false) {
            if (wordLength != 0) { // Dumata vse pak trqbva da ima nqkakvi bukvi
                bool isLegit = true; // Ako ima nqkude greshka stava false i ne se zachita dumata
                
                if (wordLength > 1) { // Proverqva dali sa v liniq
                    int xdiff = currentWord[1].cordX - currentWord[0].cordX;
                    int ydiff = currentWord[1].cordY - currentWord[0].cordY;

                    // Tova trqbva zashtoto ako wordLength = 2 vinagi priema che e vqrna dumata + ako sa po diagonal sushto go zachita kato duma
                    if (!(xdiff == 0 || ydiff == 0)) {
                        clearGrid(tileGrid, currentWord, &wordLength, options);
                        isLegit = false;
                    }

                    for (int i = 2; i < wordLength; i++) {
                        int currXdiff = currentWord[i].cordX - currentWord[i - 1].cordX;
                        int currYdiff = currentWord[i].cordY - currentWord[i - 1].cordY;
                        printf("%d %d = %d %d \n", currXdiff, currYdiff, xdiff, ydiff);

                        if (currXdiff != xdiff || currYdiff != ydiff) {
                            clearGrid(tileGrid, currentWord, &wordLength, options);
                            isLegit = false;
                        }
                    }
                }

                if (emptyBoard == true) { // proverqva dali pruviq hod zapochva v sredata
                    bool flag = false;
                    for (int i = 0; i < wordLength; i++) {
                        if (currentWord[i].cordX == 7 && currentWord[i].cordY == 7) // centura na ekrana
                            flag = true;
                    }
                    if (flag == false) {
                        clearGrid(tileGrid, currentWord, &wordLength, options);
                        isLegit = false;
                    }
                }
                else { // proverqva dali novata duma e svurzana kum veche suzdadenite
                    bool connectedToOtherWords = false;
                    for (int i = 0; i < wordLength; i++) {
                        // Segashnite kordinati
                        int x = currentWord[i].cordX;
                        int y = currentWord[i].cordY;
                        
                        // Check the surrounding cells for "true" values
                        if (x > 0 && tileGrid[x - 1][y].buttonPressed == true && !isInTheCurrentWord(currentWord, wordLength, x - 1, y)) {
                            connectedToOtherWords = true;
                        }
                        if (x < 14 && tileGrid[x + 1][y].buttonPressed == true && !isInTheCurrentWord(currentWord, wordLength, x + 1, y)) { // <14, zashtoto daskata e 15x15, posledniq ni index e 14, a nie ne iskame da izlezem ot daskata 
                            connectedToOtherWords = true;
                        }
                        if (y > 0 && tileGrid[x][y - 1].buttonPressed == true && !isInTheCurrentWord(currentWord, wordLength, x, y - 1)) {
                            connectedToOtherWords = true;
                        }
                        if (y < 14 && tileGrid[x][y + 1].buttonPressed == true && !isInTheCurrentWord(currentWord, wordLength, x, y + 1)) {// <14, zashtoto daskata e 15x15, posledniq ni index e 14, a nie ne iskame da izlezem ot daskata 
                            connectedToOtherWords = true;
                        }
                    }
                    if (connectedToOtherWords == false) {
                        clearGrid(tileGrid, currentWord, &wordLength, options);
                        isLegit = false;
                    }

                }
                if (isLegit == true) {
                    emptyBoard = false;
                    newOptions(options, letters, letterPoints, &numberOfLettersLeft);
                }
                wordLength = 0;
                pressedOptionsButton = -1;
            }
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
