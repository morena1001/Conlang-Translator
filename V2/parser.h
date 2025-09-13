#ifndef INC_PARSER_H
#define INC_PARSER_H

#include <string>
#include <vector>

// FUNCTIONS NEEDED
/*
    OPTION DIVIDER FUNCTION:
        Each token is split by each option appended at the end of a word.
        The base word comes first, followed by all conjugations, case markers, and derivation markers
        All separated by a period

    WORD SEARCH FUNCTION:
        The base word for each divded token will be ran through the dictionary.
        If the word is found, the function will run another function based on the part of speech of the base word.
        Else, return an error and stop the program.

    Each part of speech will have its own function to parse its settings.
    The following is an sample template function.
    PARSER FUNCTION:
        A word's declensions and conjugations will be added to the word, building it as it parses the options.
            If a word changes its part of speech and there are still more options left to parse, it will try to parse the rest of the options following the structure of the new part of speech.
            
        If the parse cannot find the conjugation or declension for a given option, it will spit out an error value, that will be used to build the final sentence.
        The output will be a struct containing the final word/s and a value indicating where it will go in the final sentence.
        This value will be stuff like (main verb, subject noun 1, direct object, adverb, subject noun 1 adjective).
        The numbers in the example will be decided based on the order it is written in the console, so that is important.
*/

enum sentence_part { VERB, NOUN, ADJECTIVE, ADVERB, NUMBER, ADPOSITION, CONJUNCTION, CLAUSE, ERROR };

typedef struct {
    std::string phrase;
    enum sentence_part part;
    uint8_t order;
} word_token_t;

std::vector<std::string> option_divider (std::string &input);
bool dictionary_search (std::string &base);

word_token_t noun_parser (std::vector<std::string> &token);

#endif
