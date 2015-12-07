#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 128

typedef struct sChecker {
    struct sChecker * letters[LEN];
    char end;
} checker;

checker * newChecker() {
    checker * spellChecker = (checker*) malloc(sizeof(checker));
    for (int i = 0; i < LEN; i++) {
        spellChecker->letters[i] = NULL;
    }
    spellChecker->end = 0;
    return spellChecker;
}

void addWord(checker * spellChecker, char * word) {
    char offset;
    int wordlen = strlen(word) - 1;
    checker * pChecker = spellChecker;

    for (int i = 0; i < wordlen; i++) {
        offset = word[i];
        if (pChecker->letters[offset] == NULL)
            pChecker->letters[offset] = newChecker();
        pChecker = pChecker->letters[offset];
    }
    pChecker->end = 1;
}

int checkWord(checker * spellChecker, char * word) {
    char offset;
    int wordlen = strlen(word);
    checker * pChecker = spellChecker;
    for (int i = 0; i < wordlen; i++) {
        offset = word[i];
        if (pChecker->letters[offset] == NULL)
            return 0;
        pChecker = pChecker->letters[offset];
    }
    return pChecker->end;
}

void loadDictionary(checker * spellChecker) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE * fp = fopen("/usr/share/dict/words", "r");
    if (fp == NULL)
        exit(1);

    while ((read = getline(&line, &len, fp)) != -1)
        addWord(spellChecker, line);

    fclose(fp);
    if (line)
        free(line);
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Usage: spell [word]\n");
        exit(1);
    }
    char str[32];
    checker * spellChecker = newChecker();

    printf("Loading Spell Checker...\n");
    loadDictionary(spellChecker);
    printf("Spellchecker loaded.\n");

    while (1) {
        printf("Enter word to test: ");
        scanf("%s", str);
        if (checkWord(spellChecker, str))
            printf("Valid\n");
        else
            printf("Invalid\n");
    }
}
