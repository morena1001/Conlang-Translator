#ifndef INC_FEJORD_PARSER_H_
#define INC_FEJORD_PARSER_H_

#include <string>
#include <vector>

#include "lib/cJSON.h"

// FUNCTIONS NEEDED
/*
    TOKENIZER FUNCTION:
        Receives the input from the console, and splits each word into a token with all its options.

    OPTION DIVIDER FUNCTION:
        Each token is split by each option appended at the end of a word.

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
        
    SENTENCE BUILDER FUNCTION:
        This is the final stop, and is where the parsed words will be connected.
        This function will take into account the value of each parsed word struct, as well as the natrual SVO order of fejord.
        The final output will be printed back onto the console as well as an extra text document so that user can have a copy of it.         
*/

enum sentence_part { VERB, ADVERB, NOUN, SUBJECT, DIR_OBJECT, IND_OBJECT, ADJ, SUB_CL, REL_CL, CLAUSE, ADPOS, ERROR };
enum json_type { ADPOS_JSON, CLAUSE_JSON, DICT_JSON, NOUN_JSON, ADJ_JSON, ADV_JSON, NUM_JSON, VERB_JSON };

enum word_gender { F, M, N };
enum word_plurality { SING, PLUR };
enum noun_case { NOMINATIVE, ACCUSATIVE, DATIVE, VOCATIVE, COMITATIVE, INSTRUMENTAL, SOCIATIVE, LOCATIVE, ABLATIVE, INESSIVE, ADDESSIVE, GENITIVE, ERGATIVE };
enum noun_article { DEFINITIVE, INDEFINITIVE, NONE };
enum affirmation { NO_CONFIRMATION, AFFIRMATION, NEGATION };

enum adj_comparitive { NORMAL, COMPARITIVE, SUPERLATIVE };

typedef struct {
    std::string phrase;
    enum sentence_part part;
    uint8_t order;
} word_token_t;

typedef struct {
    char punct;
    uint8_t order;
} punct_token_t;

static cJSON *adpos_json = NULL;
static cJSON *clause_json = NULL;
static cJSON *dict_json = NULL;
static cJSON *noun_json = NULL;
static cJSON *adj_json = NULL;
static cJSON *adverb_json = NULL;
static cJSON *number_json = NULL;
static cJSON *verb_json = NULL;



std::vector<std::string> word_divider (std::string &input);
std::vector<std::vector<std::string>> word_tokenizer (std::vector<std::string> &words);
bool dictionary_search (std::string &base);
word_token_t adpos_parser (std::vector<std::string> &token);
word_token_t clause_parser (std::vector<std::string> &token);
word_token_t noun_parser (std::vector<std::string> &token);
word_token_t adjective_parser (std::vector<std::string> &token);
word_token_t adverb_parser (std::vector<std::string> &token);
word_token_t number_parser (std::vector<std::string> &token);
word_token_t verb_parser (std::vector<std::string> &token);
std::string sentence_builder (std::vector<word_token_t> &tokens, uint8_t num_tokens);

#endif /* INC_FEJORD_PARSER_H_ */
