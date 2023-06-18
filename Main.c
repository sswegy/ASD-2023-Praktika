#define _CRT_SECURE_NO_WARNINGS
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>



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

struct word {
    int cordX;
    int cordY;
    int direction; // 0 - po x   1 - po y
    int length;
    int points;
};

struct specialTile {
    int cordX;
    int cordY;
    bool doubleWord;
    bool tripleWord;
    bool doubleLetter;
    bool tripleLetter;
};

#define LETTERS 26

typedef struct node_t
{
    bool endOfWord;
    struct node_t* children[LETTERS];
} node_t;

//KRIS
node_t* initNode(bool endOfWord)
{
    node_t* node = (node_t*)calloc(1, sizeof * node);
    assert(node);
    node->endOfWord = endOfWord;
    return node;
}

node_t* generateTree(const char* filename)
{
    FILE* file = fopen(filename, "r");
    char buffer[256];

    node_t* root = initNode(false);

    while (fgets(buffer, 256, file))
    {
        node_t* tmp = root;

        for (size_t i = 0; buffer[i] != '\n'; tmp = tmp->children[buffer[i] - 'A'], ++i)
            if (tmp->children[buffer[i] - 'A'] == NULL)
                tmp->children[buffer[i] - 'A'] = initNode(buffer[i + 1] == '\n' ? true : false);
    }

    fclose(file);
    return root;
}

//VIKTOR
void addWord(node_t* root, char* word)
{
    int length = strlen(word);
    for (int i = 0; i < length; i++)
        word[i] = (word[i] >= 'a' && word[i] <= 'z') ? word[i] -= 32 : word[i];
    node_t* temp = root;
    for (int i = 0; i < length; i++)
    {
        int index = word[i] - 'A';
        if (!temp->children[index])
            temp->children[index] = initNode(word[i + 1] == '\n' ? true : false);
        temp = temp->children[index];
    }
    temp->endOfWord = true;
}

void _writeToFile(FILE* dictionary, node_t* root, char* word, int level)
{
    if (root->endOfWord)
    {
        word[level] = '\0';
        fprintf(dictionary, "%s\n", word);
    }
    for (int i = 0; i < LETTERS; i++)
        if (root->children[i])
        {
            word[level] = i + 'A';
            _writeToFile(dictionary, root->children[i], word, level + 1);
        }
}

void writeToFile(const char* filename, node_t* root)
{
    FILE* output = fopen(filename, "w");
    char wrd[20];
    _writeToFile(output, root, wrd, 0);
    fclose(output);
}

//MLADEN
bool validateWord(node_t* root, const char* word)
{
    node_t* temp = root;
    for (; *word != '\0'; word++)
    {
        if (temp->children[*word - 'A'] == NULL)
            return false;

        temp = temp->children[*word - 'A'];
    }
    return temp->endOfWord == true;
}


void drawGame(struct tile tileGrid[15][15], struct tile returnButton, struct tile endTurnButton, struct tile options[7],
            struct tile* players, int numberOfPlayers, struct tile round, struct tile winner) {
    if (winner.points == 0) {
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

        // Tochkite na igrachite
        for (int i = 0; i < numberOfPlayers; i++) {
            DrawRectangleRec(players[i].button, players[i].buttonColor);
            DrawText(players[i].text, players[i].button.x + 7, players[i].button.y + 25, 30, RAYWHITE);
        }

        // Rundovete
        DrawRectangleRec(round.button, round.buttonColor);
        DrawText(round.text, round.button.x + 45, round.button.y + 25, 30, RAYWHITE);
    }
    else {
        DrawRectangleRec(winner.button, winner.buttonColor);
        DrawText(winner.text, winner.button.x + 60, winner.button.y + 25, 64, BLACK);
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

int generateRandomLetter(int letters[26], int* numberOfLettersLeft) {
    if (*numberOfLettersLeft == 0)
        return -1;
    int letter, index = -1;
    while (*numberOfLettersLeft > 0) {
        bool flag = false;
        letter = rand() % *numberOfLettersLeft + 1;
        for (int i = 0; i < 26; i++) {
            letter -= letters[i];
            if (letter <= 0) {
                flag = true;
                index = i;
                break;
            }
        }
        if (flag) break;
    }
    *numberOfLettersLeft -= 1;
    letters[index]--;
    return index;
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

bool isSkyBlue(Color buttonColor) {
    if (buttonColor.r == 102 && buttonColor.g == 191 && buttonColor.b == 255) // { 102, 191, 255, 255 }  
        return true;
    else return false;
}

bool isDarkBlue(Color buttonColor) {
    if (buttonColor.r == 0 && buttonColor.g == 82 && buttonColor.b == 172) // { 0, 82, 172, 255 }  
        return true;
    else return false;
}

bool isPink(Color buttonColor) {
    if (buttonColor.r == 255 && buttonColor.g == 109 && buttonColor.b == 194) // { 255, 109, 194, 255 }
        return true;
    else return false;
}

bool isMaroon(Color buttonColor) {
    if (buttonColor.r == 190 && buttonColor.g == 33 && buttonColor.b == 55) // { 190, 33, 55, 255 } 
        return true;
    else return false;
}


int calculatePoints(struct letter currentWord[15], struct tile tileGrid[15][15], int wordLength, node_t* root) {
    struct word words[8]; // max edna nova duma moje da uchastva v 8 dumi (Primer edna po x + 7 po y)
    int wordsCount = 0;

    struct specialTile specials[5]; // na edin red MAX specialni poleta sa 5
    int specialsCount = 0;
    for (int i = 0; i < wordLength; i++) { // specialnite poleta
        if (isSkyBlue(currentWord[i].tileColor) == true) { // double letter
            specials[specialsCount].cordX = currentWord[i].cordX;
            specials[specialsCount].cordY = currentWord[i].cordY;
            specials[specialsCount].doubleLetter = true;
            specials[specialsCount].tripleLetter = false;
            specials[specialsCount].doubleWord = false;
            specials[specialsCount].tripleWord = false;
            specialsCount++;
        }

        if (isDarkBlue(currentWord[i].tileColor) == true) { // triple letter
            specials[specialsCount].cordX = currentWord[i].cordX;
            specials[specialsCount].cordY = currentWord[i].cordY;
            specials[specialsCount].doubleLetter = false;
            specials[specialsCount].tripleLetter = true;
            specials[specialsCount].doubleWord = false;
            specials[specialsCount].tripleWord = false;
            specialsCount++;
        }

        if (isPink(currentWord[i].tileColor) == true) { // double word
            specials[specialsCount].cordX = currentWord[i].cordX;
            specials[specialsCount].cordY = currentWord[i].cordY;
            specials[specialsCount].doubleLetter = false;
            specials[specialsCount].tripleLetter = false;
            specials[specialsCount].doubleWord = true;
            specials[specialsCount].tripleWord = false;
            specialsCount++;
        }

        if (isMaroon(currentWord[i].tileColor) == true) { // triple word
            specials[specialsCount].cordX = currentWord[i].cordX;
            specials[specialsCount].cordY = currentWord[i].cordY;
            specials[specialsCount].doubleLetter = false;
            specials[specialsCount].tripleLetter = false;
            specials[specialsCount].doubleWord = false;
            specials[specialsCount].tripleWord = true;
            specialsCount++;
        }
    }

    for (int i = 0; i < wordLength; i++) {
        int currX = currentWord[i].cordX; // current cordinates
        int currY = currentWord[i].cordY;

        //PRoverqvame za dumite po X
        //printf("----XXXX----\n");
        int x;
        for (x = currentWord[i].cordX; x >= 0; x--) { // stigame do naj-lqvata bukva
            if(x == 0){
                if (tileGrid[0][currY].buttonPressed == false) {
                    x += 1;
                }
                break;
            }
            if (tileGrid[x - 1][currY].buttonPressed == false) {
                break;
            }
        }
        //printf("X = %d, Y = %d\n", x, currY);
        int tempLength = 1;
        int tempPoints = 0;

        int tripleWordS = 0;
        int doubleWordS = 0;
        for (int tempX = x; tempX < 15; tempX++) {
            //printf("length = %d, points = %d\n", tempLength, tileGrid[tempX][currY].points);
            if (tileGrid[tempX][currY].buttonPressed == false) {
                tempLength -= 1;
                break;
            }

            int doubleLetterS = 0;
            int tripleLetterS = 0;
            for (int n = 0; n < specialsCount; n++) {
                if(specials[n].cordX == tempX && specials[n].cordY == currY){
                    if (specials[n].doubleLetter) doubleLetterS++;
                    else if (specials[n].tripleLetter) tripleLetterS++;
                    else if (specials[n].doubleWord) doubleWordS++;
                    else if (specials[n].tripleWord) tripleWordS++;
                }
            }
            tempPoints += tileGrid[tempX][currY].points + doubleLetterS * tileGrid[tempX][currY].points + tripleLetterS * 2 * tileGrid[tempX][currY].points;
            //obiknovenno samo + tileGrid[tempX][currY].points;, pri double letter 2*tileGrid[tempX][currY].points;, pri tripleLetter 3*tileGrid[tempX][currY].points;
            tempLength++;
        }
        tempPoints += doubleWordS * tempPoints + tripleWordS * 2 * tempPoints;// sushtoto kato gore


        if (tempLength > 1) {
            //printf("Adding to words[]\n");
            struct word temp;
            temp.cordX = x;
            temp.cordY = currY;
            temp.direction = 0;
            temp.length = tempLength;
            temp.points = tempPoints;
            bool exists = false;
            for (int i = 0; i < wordsCount; i++) {
                if (temp.cordX == words[i].cordX && temp.cordY == words[i].cordY && temp.direction == words[i].direction
                    && temp.length == words[i].length && temp.points == words[i].points) {
                    exists = true;
                    break;
                }
            }
            if (exists == false) {
                words[wordsCount].cordX = temp.cordX;
                words[wordsCount].cordY = temp.cordY;
                words[wordsCount].direction = temp.direction;
                words[wordsCount].length = temp.length;
                words[wordsCount].points = temp.points;
                wordsCount++;
                //printf("Added to words[]\n");
            }
        }
            //printf("----YYYY----\n");
            //PRoverqvame za dumite po Y
            int y;
            for (y = currentWord[i].cordY; y >= 0; y--) { // stigame do naj-lqvata bukva
                if (y == 0) {
                    if (tileGrid[currX][0].buttonPressed == false) {
                        y += 1;
                    }
                    break;
                }
                if (tileGrid[currX][y - 1].buttonPressed == false) {
                    break;
                }
            }
            //printf("X = %d, Y = %d\n", currX, y);
            tempLength = 1;
            tempPoints = 0;

            tripleWordS = 0;
            doubleWordS = 0;
            for (int tempY = y; tempY < 15; tempY++) {
                //printf("length = %d, points = %d\n", tempLength, tileGrid[currX][tempY].points);
                if (tileGrid[currX][tempY].buttonPressed == false) {
                    tempLength -= 1;
                    break;
                }

                int doubleLetterS = 0;
                int tripleLetterS = 0;
                for (int n = 0; n < specialsCount; n++) {
                    if (specials[n].cordX == currX && specials[n].cordY == tempY) {
                        if (specials[n].doubleLetter) doubleLetterS++;
                        else if (specials[n].tripleLetter) tripleLetterS++;
                        else if (specials[n].doubleWord) doubleWordS++;
                        else if (specials[n].tripleWord) tripleWordS++;
                    }
                }
                //printf("dl = %d, tl = %d\n", doubleLetterS, tripleLetterS);
                tempPoints += tileGrid[currX][tempY].points + doubleLetterS * tileGrid[currX][tempY].points + tripleLetterS * 2 * tileGrid[currX][tempY].points;
                //obiknovenno samo + tileGrid[tempX][currY].points;, pri double letter 2*tileGrid[tempX][currY].points;, pri tripleLetter 3*tileGrid[tempX][currY].points;
                tempLength++;
            }
            tempPoints += doubleWordS * tempPoints + tripleWordS * 2 * tempPoints;// sushtoto kato gore

            if (tempLength > 1) {
                //printf("Adding to words[]\n");
                struct word temp;
                temp.cordX = currX;
                temp.cordY = y;
                temp.direction = 1;
                temp.length = tempLength;
                temp.points = tempPoints;
                bool exists = false;
                for (int i = 0; i < wordsCount; i++) {
                    if (temp.cordX == words[i].cordX && temp.cordY == words[i].cordY && temp.direction == words[i].direction
                        && temp.length == words[i].length && temp.points == words[i].points) {
                        exists = true;
                        break;
                    }
                }
                if (exists == false) {
                    words[wordsCount].cordX = temp.cordX;
                    words[wordsCount].cordY = temp.cordY;
                    words[wordsCount].direction = temp.direction;
                    words[wordsCount].length = temp.length;
                    words[wordsCount].points = temp.points;
                    wordsCount++;
                    //printf("Added to words[]\n");
                }
            }
        }
        printf("**");
        for (int i = 0; i < wordLength; i++) {
            printf("%c", currentWord[i].symbol);
        }
        printf("**\n");

    int totalPoints = 0;
    bool isLegit = false;
    char checkWord[15]; //max
    for (int i = 0; i < wordsCount; i++) {
        printf("____%d %d____\n", words[i].cordX, words[i].cordY);
        int x = 0, y = 0;
        for (int j = 0; j < words[i].length; j++) {
            printf("_%d %d_ %c\n", words[i].cordX+ x, words[i].cordY+ y, tileGrid[words[i].cordX + x][words[i].cordY + y].text[0]);
            checkWord[j] = tileGrid[words[i].cordX + x][words[i].cordY + y].text[0];
            if (words[i].direction == 0) x++;
            if (words[i].direction == 1) y++;
        }
        checkWord[words[i].length] = 0;
        printf("!!!!!%s\n", checkWord);
        isLegit = validateWord(root, checkWord); // ako vurne TRUE znachi dumata e legit, ako vurne FALSE znachi ne e
        totalPoints += words[i].points;
    }
    //printf("%d\n---------\n", totalPoints);
    if (isLegit == true) {
        return totalPoints;
    }
    else {
        return -1;
    }
}

int calculateWinner(struct tile* players, int numberOfPlayers){
    int winner = 0;
    int points = players[0].points;
    for (int i = 1; i < numberOfPlayers; i++) {
        if (players[i].points > points) {
            points = players[i].points;
            winner = i;
        }
    }
    return winner + 1;
}

int main() {
    srand(time(0));

    node_t* root = generateTree("Collins_Scrabble_Words_2019.txt");


    // Initialize Raylib
    InitWindow(1220, 1000, "Raylib Button");

    struct tile tileGrid[15][15]; // { corX, corY, sizeX, sizeY } sizeX = 50
    bool emptyBoard = true; // Za da proverka na purviq hod

    struct letter currentWord[15]; //maksimalnata duljina na dumata e 15 poleta
    int wordLength = 0;


    int letterPoints[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };

    int letters[26] = { 9, 2, 2, 4 ,12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1 };
    int numberOfLettersLeft = 98;


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

    int corX = 5;
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

    struct tile round;
    round.button = (Rectangle){ 980, 5, 210, 80 };
    round.buttonColor = BLACK;
    round.points = 1; // Koi rund sme
    round.text = (char*)malloc((sizeof(char) * 7) + 1); // "Round X" + null terminator
    sprintf(round.text, "Round %d", round.points);


    //Color playerColors[4] = {RED, GREEN, BLUE, PURPLE};

    struct tile* players;

    int numberOfPlayers = 4; // IGRACHITE OT TUK

    players = (struct tile*)malloc(sizeof(struct tile) * numberOfPlayers);

    corY = 95;
    for (int i = 0; i < numberOfPlayers; i++) {
        players[i].button = (Rectangle){ 980, corY, 210, 80 };
        players[i].buttonColor = GRAY;
        players[i].points = 0;
        players[i].text = (char*)malloc((sizeof(char) * 11) + 1); // "PlayerN XXX"
        sprintf(players[i].text, "PLayer%d %d", i+1, players[i].points);

        corY += 85;
    }
    for (int i = 0; i < numberOfPlayers; i++) {
        printf("%s\n", players[i].text);
    }

    struct tile **options; // numberOfPlayers, 7
    options = (struct tile**)malloc(sizeof(struct tile*) * numberOfPlayers);
    if (options == NULL) {
        printf("Nqma memory\n");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        options[i] = (struct tile*)malloc(sizeof(struct tile) * 7);
        if (options[i] == NULL) {
            printf("Nqma memory\n");
            return 1;
        }
    }


    for (int n = 0; n < numberOfPlayers; n++) {
        corX = 300;
        for (int i = 0; i < 7; i++) {
            options[n][i].button = (Rectangle){ corX, 55 * 15 + 5, 70, 60 };
            options[n][i].buttonColor = ORANGE;
            options[n][i].buttonPressed = false;
            options[n][i].text = (char*)malloc((sizeof(char) * 4) + 1); // bukva + space + tochki
            int letter = generateRandomLetter(letters, &numberOfLettersLeft, &numberOfLettersLeft);
            options[n][i].points = letterPoints[letter];
            sprintf(options[n][i].text, "%c %d", 'A' + letter, options[n][i].points);
            corX += 85;
        }
    }
    int pressedOptionsButton = -1;
    int turns = 0;
    struct tile winner;
    winner.button = (Rectangle){ 400, 370, 400, 200 };
    winner.buttonColor = GOLD;
    winner.points = 0; // Koi e pobeditel
    winner.text = (char*)malloc((sizeof(char) * 17) + 1); // "Winner:\nPlayer X" + null terminator
    sprintf(winner.text, " Winner:\nPlayer %d", winner.points);


    //Game Loop-a
    while (!WindowShouldClose()) {
        round.points = turns / numberOfPlayers + 1;
        sprintf(round.text, "Round %d", round.points);
        if (round.points == 6) {
            winner.points = calculateWinner(players, numberOfPlayers);
            sprintf(winner.text, " Winner:\nPlayer %d", winner.points);
        }


        // Ocvetqvame tozi koito e nared
        for (int i = 0; i < numberOfPlayers; i++) {
            if (i == turns % numberOfPlayers) {
                players[i].buttonColor = LIME;
            }
            else {
                players[i].buttonColor = GRAY;
            }
        }
        int currentPlayer = turns%numberOfPlayers;

        // Logika na opciite
        for (int i = 0; i < 7; i++) {
            
            if (isRayWhite(options[currentPlayer][i].buttonColor) == true) {
                options[currentPlayer][i].buttonColor = RAYWHITE;
            }else if (pressedOptionsButton == -1 || pressedOptionsButton != i)
                options[currentPlayer][i].buttonColor = ORANGE; // samo edin button moje da e ocveten/ clicknat
            if (CheckCollisionPointRec(GetMousePosition(), options[currentPlayer][i].button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
                && options[currentPlayer][i].buttonPressed == false && isRayWhite(options[currentPlayer][i].buttonColor) == false) {
                pressedOptionsButton = i;
                options[currentPlayer][i].buttonColor = (Color){ 250, 118, 30, 255 }; // red, green, blue, mislq che e opacity
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
                    currentWord[wordLength].points = options[currentPlayer][pressedOptionsButton].points;
                    currentWord[wordLength].symbol = options[currentPlayer][pressedOptionsButton].text[0]; // za checkValidWord ili tam kakto se kazva
                    sprintf(tileGrid[x][y].text, "%c %d", currentWord[wordLength].symbol, currentWord[wordLength].points);
                    wordLength++;
                    tileGrid[x][y].buttonPressed = true;
                    tileGrid[x][y].buttonColor = ORANGE;
                    tileGrid[x][y].points = options[currentPlayer][pressedOptionsButton].points;

                    options[currentPlayer][pressedOptionsButton].buttonColor = RAYWHITE;
                    pressedOptionsButton = -1;
                }
            }
        }

        //Return boton logika
        if (CheckCollisionPointRec(GetMousePosition(), returnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.buttonPressed == false) {
            clearGrid(tileGrid, currentWord, &wordLength, options[currentPlayer]);
        }

        //proverkata za duma
        if (CheckCollisionPointRec(GetMousePosition(), endTurnButton.button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && endTurnButton.buttonPressed == false) {
            if (wordLength != 0) { // Dumata vse pak trqbva da ima nqkakvi bukvi               

                bool isLegit = true; // Ako ima nqkude greshka stava false i ne se zachita dumata
                bool isLine = true;

                if (wordLength > 1) { // Proverqva dali sa v liniq
                    int xdiff = currentWord[1].cordX - currentWord[0].cordX;
                    int ydiff = currentWord[1].cordY - currentWord[0].cordY;

                    // Tova trqbva zashtoto ako wordLength = 2 vinagi priema che e vqrna dumata + ako sa po diagonal sushto go zachita kato duma
                    if (!(xdiff == 0 || ydiff == 0)) {
                        isLine = false;
                    }


                    for (int i = 2; i < wordLength; i++) {
                        int currXdiff = currentWord[i].cordX - currentWord[i - 1].cordX;
                        int currYdiff = currentWord[i].cordY - currentWord[i - 1].cordY;
                        
                        if (xdiff == 0) {
                            if (currXdiff != 0) {
                                printf("xdiff:%d currXdiff:%d i cord:%d  i-1 cord:%d\n", xdiff, currXdiff, currentWord[i].cordX, currentWord[i - 1].cordX);
                                isLine = false;
                            }
                        }

                        if (ydiff == 0) {
                            if (currYdiff != 0) {
                                printf("ydiff:%d currYdiff:%d i cord:%d  i-1 cord:%d\n", ydiff, currYdiff, currentWord[i].cordY, currentWord[i - 1].cordY);
                                isLine = false;
                            }
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
                        isLegit = false;
                    }
                    else {
                        isLegit = true;
                    }

                }



                int newPoints = calculatePoints(currentWord, tileGrid, wordLength, root);
                if (newPoints == -1) {
                    isLegit = false;
                }
                printf("Line:%d Legit:%d\n", isLine, isLegit);
                if (isLegit == true && isLine == true) {
                    newOptions(options[currentPlayer], letters, letterPoints, &numberOfLettersLeft);
                    if (emptyBoard == true && wordLength == 1) {
                        players[turns % numberOfPlayers].points += currentWord[0].points;
                    }
                    else {
                        players[turns % numberOfPlayers].points += newPoints;
                    }
                    sprintf(players[turns%numberOfPlayers].text, "PLayer%d %d", turns% numberOfPlayers + 1, players[turns % numberOfPlayers].points);
                    turns++;
                    emptyBoard = false;
                }
                else {
                    clearGrid(tileGrid, currentWord, &wordLength, options[currentPlayer]);
                }
                wordLength = 0;
                pressedOptionsButton = -1;
            }
            else {
                turns++; // Ako igrach ne moje da izmisli duma skipva redut si
            }
        }

        //Risuvame
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawGame(tileGrid, returnButton, endTurnButton, options[currentPlayer], players, numberOfPlayers, round, winner);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}