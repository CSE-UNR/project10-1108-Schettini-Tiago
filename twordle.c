#include <stdio.h>

#define MAX_GUESSES 6
#define WORD_LEN    5

//Reads the 5-letter mystery word from word.txt
void loadWord(char word[]) {
    FILE *fp;
    fp = fopen("word.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open word.txt\n");
        word[0] = '\0';
        return;
    }
    fscanf(fp, "%5s", word);
    fclose(fp);
}

//Returns 1 if c is an alphabetic letter, 0 otherwise 
int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


int isValidGuess(char guess[]) {
    int i;
    int len;
    len = 0;
    for (i = 0; guess[i] != '\0'; i++) {
        if (!isLetter(guess[i])) {
            return 0;
        }
        len++;
    }
    return len == WORD_LEN;
}

//Converts every letter in guess to lowercase in-place 
void toLowercase(char guess[]) {
    int i;
    for (i = 0; guess[i] != '\0'; i++) {
        if (guess[i] >= 'A' && guess[i] <= 'Z') {
            guess[i] = guess[i] + 32;
        }
    }
}


void checkGuess(char guess[], char word[], char result[]) {
    int i;
    int j;

    //start everything as not-in 
    for (i = 0; i < WORD_LEN; i++) {
        result[i] = 'N';
    }

    //first pass: mark exact matches 
    for (i = 0; i < WORD_LEN; i++) {
        if (guess[i] == word[i]) {
            result[i] = 'C';
        }
    }

    //mark wrong-position letters 
    for (i = 0; i < WORD_LEN; i++) {
        if (result[i] == 'C') {
            continue;
        }
        for (j = 0; j < WORD_LEN; j++) {
            if (guess[i] == word[j]) {
                
                if (guess[j] != word[j]) {
                    result[i] = 'W';
                }
                break;
            }
        }
    }
}

//Prints uppercase for correct spot, lowercase for wrong  
void printGuessRow(char guess[], char result[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (result[i] == 'C') {
            printf("%c", guess[i] - 32);   //uppercase 
        } else {
            printf("%c", guess[i]);         //owercase 
        }
    }
    printf("\n");
}


void printCaretRow(char result[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (result[i] == 'W') {
            printf("%c", '^');
        } else {
            printf("%c", ' ');
        }
    }
    printf("\n");
}

/* Prints all guesses made so far with their caret rows */
void printBoard(char guesses[][6], char results[][6], int numGuesses) {
    int i;
    for (i = 0; i < numGuesses; i++) {
        printGuessRow(guesses[i], results[i]);
        printCaretRow(results[i]);
    }
}

/* Returns 1 if all positions are 'C' (player won), 0 otherwise */
int isWin(char result[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (result[i] != 'C') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char word[WORD_LEN + 1];
    char guesses[MAX_GUESSES][6];
    char results[MAX_GUESSES][6];
    char guess[100];
    int numGuesses;
    int won;
    int i;

    loadWord(word);
    if (word[0] == '\0') {
        return 1;
    }

    numGuesses = 0;
    won = 0;

    while (numGuesses < MAX_GUESSES && !won) {
        printf("Guess %d/6 - Enter a 5-letter word: ", numGuesses + 1);
        scanf("%99s", guess);

        toLowercase(guess);

        if (!isValidGuess(guess)) {
            printf("Invalid! Must be exactly 5 letters, no numbers or symbols.\n");
            continue;
        }

        /* store guess and evaluate it */
        for (i = 0; i <= WORD_LEN; i++) {
            guesses[numGuesses][i] = guess[i];
        }
        checkGuess(guess, word, results[numGuesses]);
        numGuesses++;

        /* show the full board after each valid guess */
        printf("\n");
        printBoard(guesses, results, numGuesses);
        printf("\n");

        if (isWin(results[numGuesses - 1])) {
            won = 1;
        }
    }

    if (won) {
        printf("You got it in %d guess(es)! Nice work!\n", numGuesses);
    } else {
        printf("Out of guesses! The word was: %s\n", word);
    }

    return 0;
}


