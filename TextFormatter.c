#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void justify(FILE* in, FILE * out, int columns) {
    char words[1024];
    char * next_word = words;

    size_t total_length = 0;
    size_t num_words = 0;

    while (fscanf(in, "%s", next_word) == 1) {
        size_t word_length = strlen(next_word);
        next_word += word_length + 1;
        num_words++;
        total_length += word_length;

        if (total_length + num_words > columns) {

            size_t spaces_needed = columns - (total_length - word_length);

            int minspaces = 1;
            if (num_words > 2) // avoids nasty floating point exception if no two words fit on a line
                minspaces = spaces_needed / (num_words - 2);


            char * word_print = words;

            size_t chars_printed = fprintf(out, "%s",word_print);
            word_print += strlen(word_print) + 1;
            size_t spaces_printed = 0;
            ssize_t words_to_print = num_words - 2;

            fflush(out);

            while (words_to_print > 0) {
                int spaces_to_print = minspaces;
                if (((spaces_needed - spaces_printed) % words_to_print) * 2 >= words_to_print) // spreads spaces out along the line
                    spaces_to_print++;
                spaces_printed += spaces_to_print;
                words_to_print--;
                chars_printed += fprintf(out, "%*c%s", spaces_to_print, ' ', word_print);
                word_print += strlen(word_print) + 1;
                fflush(out);
            }
            fprintf(out, "\n");

            memmove(words, word_print, (total_length = strlen(word_print)) + 1);
            num_words = 1;
            next_word = words + total_length + 1;
        }

    }

    char * word_print = words;
    while (word_print != next_word) {
        word_print += fprintf(out, "%s ", word_print);
    }

    fprintf(out, "\n");
}

int main(int argc, char ** argv) {

    int columns = atoi(argv[1]);

    FILE * in = stdin;

    if (argc >= 3 && argv[2]) {
        in = fopen(argv[2], "r");
        if (!in) {
            perror("fopen");
            return -1;
        }
    }

    justify(in, stdout, columns);

}
