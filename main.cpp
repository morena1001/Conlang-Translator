#include <iostream>
#include "Parser/C_code/fejord_parser.h"
#include "test.cpp"

using namespace std;

int main (int argc, char **argv) {
    clause_test::TEST_clauses ();

    noun_test::TEST_gender_change ();
    noun_test::TEST_to_other_speeches ();
    noun_test::TEST_article ();
    
    return 0;
}

/*
    // string input = "sub_cl rel_cl";
    // vector<string> words = word_divider (input);
    // vector<vector<string>> tokens = word_tokenizer (words);
    // vector<word_token_t> final_words;
    // int idx = 0;

    // for (vector<string> token : tokens) {
    //     final_words.push_back (clause_parser (token));
    //     final_words.back ().order = idx;
    // }

    // for (word_token_t word : final_words) {
    //     cout << word.phrase << " ";
    // }
    // cout << endl;
    
    // string input = "blue sky.def[.]";

    // vector<string> words = word_divider (input);
    // vector<vector<string>> tokens = word_tokenizer (words);

    // for (vector<string> token : tokens) {
    //     cout << token.size () << " : ";
    //     for (string word : token) { 
    //         cout << word << " ";
    //     }
    //     cout << endl;
    // }

    // string apple = "bluo";
    // cout << dictionary_search (apple) << endl;
*/