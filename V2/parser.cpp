#include <iostream>
#include "parser.h"

using namespace std;


vector<string> option_divider (string &input) {
    vector<string> token;

    if (input.empty ())    return token;

    uint16_t opt_pos = input.find_first_of(".");
    uint16_t prev_pos = 0;
    string base = input.substr (0, opt_pos);

    

    return token;
}

bool dictionary_search (string &base) {

}

word_token_t noun_parser (vector<string> &token) {

}