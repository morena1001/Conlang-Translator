#include <iostream>
#include "parser.h"

using namespace std;


vector<string> option_divider (string &input) {
    vector<string> token;

    if (input.empty ())    return token;

    // Separate base word 
    uint16_t delim_pos = input.find_first_of(".");
    uint16_t prev_pos = 0;
    token.push_back (input.substr (prev_pos, delim_pos));
    prev_pos = delim_pos + 1;

    // Separate each option
    while (prev_pos != 0) {
        delim_pos = input.find_first_of (".", prev_pos);
        token.push_back (input.substr (prev_pos, delim_pos == string::npos ? delim_pos : (delim_pos - prev_pos)));
        prev_pos = delim_pos + 1;
    }    

    return token;
}

bool dictionary_search (string &base) {

}

word_token_t noun_parser (vector<string> &token) {

}