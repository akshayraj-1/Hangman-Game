#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "words.txt"
#define MAX_WORD_SIZE 35
#define MAX_HINT_SIZE 200
#define MAX_LINE_SIZE 250
#define MAX_ATTEMPTS 6

int getNumberofWords(FILE *file) {
    fseek(file, 0, SEEK_SET);
    int wordsCount = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) if (ch == '\n') wordsCount++;
    return wordsCount;
}

void getLine(FILE *file, char *word, char *hint, int line) {
    fseek(file, 0, SEEK_SET);
    char buffer[MAX_LINE_SIZE];
    int curLine = 0;

    do {
        fgets(buffer, MAX_LINE_SIZE, file);
        curLine++;
        if (curLine == line) {
            int size_word = 0;
            for (int i = 0; i < strlen(buffer); i++) {
                word[i] = buffer[i];
                size_word++;
                if (buffer[i + 1] == ',') break;
            }
            for (int i = size_word + 2; i < strlen(buffer) - 1; i++) {
                hint[i - (size_word + 2)] = buffer[i];
            }
            return;
        }
    } while (!feof(file));
}

void showHangman(int attempts) {
    printf("  _______\n");
    printf("  |     |\n");

    switch (attempts) {
        case 1:
            printf("  O     |\n");
            printf("        |\n");
            printf("        |\n");
            printf("        |\n");
            break;
        case 2:
            printf("  O     |\n");
            printf("  |     |\n");
            printf("        |\n");
            printf("        |\n");
            break;
        case 3:
            printf("  O     |\n");
            printf(" \\|     |\n");
            printf("        |\n");
            printf("        |\n");
            break;
        case 4:
            printf("  O     |\n");
            printf(" \\|/    |\n");
            printf("        |\n");
            printf("        |\n");
            break;
        case 5:
            printf("  O     |\n");
            printf(" \\|/    |\n");
            printf("  |     |\n");
            printf("        |\n");
            break;
        case 6:
            printf("  O     |\n");
            printf(" \\|/    |\n");
            printf("  |     |\n");
            printf(" / \\    |\n");
            break;
        default:
            printf("        |\n");
            printf("        |\n");
            printf("        |\n");
            printf("        |\n");
            break;
    }

    printf("________|\n");
}

void showGuessedWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        printf("%c ", word[i]);
    }
    printf("\n");
}

int checkGuess(char letter, char *guessedWord, char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (tolower(word[i]) == tolower(letter) && tolower(guessedWord[i]) != tolower(letter)) {
            guessedWord[i] = tolower(letter);
            return 1;
        }
    }
    return 0;
}

int main() {

    srand(time(NULL));

    FILE *file = fopen(FILE_NAME, "r");
    int WORDS_COUNT = 0;

    if (file == NULL || (WORDS_COUNT = getNumberofWords(file)) < 1) {
        perror("Sorry! ");
        if (file != NULL) fclose(file);
        exit(1);
    }

    int play = 1;
    char word[MAX_WORD_SIZE];
    char hint[MAX_HINT_SIZE];
    char guessedWord[MAX_WORD_SIZE];

    while (play) {
        int attempt = 0;
        int correctGuess = 0;
        getLine(file, word, hint, rand() % WORDS_COUNT + 1);

        printf("Word : %s\n", word);
        
        for (int i = 0; i < strlen(word); i++) {
            guessedWord[i] = '_';
        }
        showHangman(attempt);
        printf("Hint : %s\n", hint);
        printf("Guess Word : ");
        showGuessedWord(guessedWord);

        while (attempt < MAX_ATTEMPTS) {
            char letter;
            printf("Enter a letter : ");
            scanf("%c", &letter);

            if (checkGuess(letter, guessedWord, word)) {
                printf("-> Good Guess!\n");
                correctGuess++;
                if (correctGuess == strlen(guessedWord)) {
                    printf("-> Congrats! You won.\n");
                    break;
                }
            } else {
                printf("-> Wrong Guess! Try again.\n");
                attempt++;
                showHangman(attempt);
                if (attempt == MAX_ATTEMPTS) {
                    printf("-> Oops! You lost.\n");
                    break;
                }
            }
            printf("Guess Word : ");
            showGuessedWord(guessedWord);

            getchar();

        }

        printf("\n---------------\n");
        printf("Do you wan to play again?\n(Press 1 to play or 0 to exit) : ");
        scanf("%d", &play);
        printf("\n\n");
        getchar();

    }

    return 0;
}