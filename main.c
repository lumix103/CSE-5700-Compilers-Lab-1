// =================================================
//  Name:    Fabian Torres
//  Class:   CSE-5700 01
//  Project: Lab 1
//  Project Description: Given a text file, output
//  the number of characters, words, and lines.
// =================================================

#include <stdio.h>
#include <errno.h>
#include <string.h>

struct _INFO
{
    int char_count;
    int word_count;
    int line_count;
};

typedef struct _INFO INFO;

INFO make_info(int, int, int);
INFO make_default_info();

INFO scan(FILE *);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: expected \"./main <file_path>\"\n");
        return 1;
    }

    FILE *text = fopen(argv[1], "r");

    if (text == NULL)
    {
        printf("Error: %s\n", strerror(errno));
        return errno;
    }

    INFO text_info = scan(text);

    fclose(text);

    printf("Number of characters: %d\nNumber of words: %d\nNumber of lines: %d\n",
           text_info.char_count, text_info.word_count, text_info.line_count);
    return 0;
}

// Returns a struct _INFO with the given values.
INFO make_info(int cc, int wc, int lc)
{
    INFO info = {.char_count = cc, .word_count = wc, .line_count = lc};
    return info;
}

// Returns a struct _INFO with the values
// char_count = 0, word_count = 0, line_count = 0
INFO make_default_info()
{
    return make_info(0, 0, 1);
}

// Scans a text document and returns the amount of characthers, words, and lines.
// Notes: A newline and carriage return does not count towards the character count.
INFO scan(FILE *f)
{
    INFO info = make_default_info();
    char ch;
    char prev;

    ch = fgetc(f);

    prev = ch;

    while (ch != EOF)
    {
        ch = fgetc(f);
        info.char_count++;
        if (ch == prev && ch == ' ') // If the text has a repeated amount of whitespace
        {                            // then skip and do not consider the 'space' as the
            continue;                // start of a new word. e.g 'a  word' -> 2 words not 3.
        }
        if (ch == ' ' || ((prev == '\n' || prev == '\r') && ch != prev)) // Every word with a single space in between
        {                                                                // or a newline is considered a word.
            info.word_count++;
        }
        if (ch == '\n' || ch == '\r')
        {
            info.char_count = info.char_count - 1 >= 0 ? info.char_count - 1 : 0; // Makes sure that the amount of lines is not negative
            info.line_count++;                                                    // but the code might be reduntant.
        }
        prev = ch;
    }
    if (ch == EOF && (prev != ' ' || prev != '\r' || prev != '\n')) // Covers the edge case, where the last word
        info.word_count++;                                          // is not recognized as a word
    return info;
}