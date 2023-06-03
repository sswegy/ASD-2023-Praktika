#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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

int main()
{
    node_t* root = generateTree("Collins_Scrabble_Words_2019.txt");
    char word[] = "iwanegrovic";
    addWord(root, word);
    char word2[] = "tumoridiot";
    addWord(root, word2);
    writeToFile("output.txt", root);
    validateWord(root, "BELL");
    validateWord(root, "IVANMATEV");
    return 0;
}
