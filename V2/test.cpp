#include <iostream>
#include <assert.h>
#include "parser.h"

using namespace std;

class option_divider_test {
    public:
        static void TEST_divider () {
            string token = "";

            // An empty token returns nothing
            assert (option_divider (token).empty ());

            // A token with no options returns vector with only the base word
            token += "city";
            assert (option_divider (token) == (vector<string> {"city"}));

            // A token with options returns vectorized base word and options
            token += ".nom.plur.def";
            assert (option_divider (token) == (vector<string> {"city", "nom", "plur", "def"}));

            cout << "Passed all option divide function tests" << endl;
        }
};

class dictionary_search_test {

};

class noun_test {

};