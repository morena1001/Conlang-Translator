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
            token += ".nom.plur.defoifhoifhwe";
            assert (option_divider (token) == (vector<string> {"city", "nom", "plur", "defoifhoifhwe"}));

            cout << "Passed all option divide function tests" << endl;
        }
};

class translate_word_test {

};

class noun_test {
    public:
        static void TEST_case () {
            vector<string> token = {"city"};

            // Default case should be nominative
            assert (!noun_parser (token).phrase.compare ("libet"));

            // male indirect objects should have the "eh" suffix
            token.push_back ("dat");
            assert (!noun_parser (token).phrase.compare ("libeh"));

            // Make sure part of speech does not change
            assert (noun_parser (token).part == NOUN);

            cout << "Passed all noun function case tests" << endl;
        }

        static void TEST_gender () {
            vector<string> token = {"village"};

            // Default gender should be female 
            assert (!noun_parser (token).phrase.compare ("nerat"));

            // Male gendered nouns should have an "e" in their suffix
            token.push_back ("m");
            assert (!noun_parser (token).phrase.compare ("neret"));

            // Make sure part of speech does not change
            assert (noun_parser (token).part == NOUN);

            cout << "Passed all noun function gender test" << endl;
        }

        static void TEST_plurality () {
            vector<string> token = {"wolf"};

            // Default is singular
            assert (!noun_parser (token).phrase.compare ("kobet"));

            // Plural nouns should end in "s"
            token.push_back ("plur");
            assert (!noun_parser (token).phrase.compare ("kobets"));

            // Make sure part of speech does not change
            assert (noun_parser (token).part == NOUN);

            cout << "Passed all noun function plurality tests" << endl;
        }

        static void TEST_definiteness () {
            vector<string> token = {"tree"};

            // Default is generic
            assert (!noun_parser (token).phrase.compare ("denat"));

            // Definite and indefinite articles go before the noun
            token.push_back ("def");
            assert (!noun_parser (token).phrase.compare ("da denat"));

            token.back () = "indef";
            assert (!noun_parser (token).phrase.compare ("ain denat"));

            // Make sure part of speech does not change
            assert (noun_parser (token).part == NOUN);

            cout << "Passed all noun function definiteness tests" << endl;
        }

        static void TEST_polarity () {
            vector<string> token = {"it"};

            // Default has no polarity
            assert (!noun_parser (token).phrase.compare ("kamot"));

            // Affirmation and negation go before the noun
            token.push_back ("aff");
            assert (!noun_parser (token).phrase.compare ("rot kamot"));

            token.back () = "neg";
            assert (!noun_parser (token).phrase.compare ("fol kamot"));

            // Make sure part of speech does not change
            assert (noun_parser (token).part == NOUN);

            cout << "Passed all noun function polarity tests" << endl;
        }

        static void TEST_derivation () {
            vector<string> token = {"cloud"};

            // Adjective derivation replaces the case with the corresponding adjective vowel
            token.push_back ("adj");
            assert (!noun_parser (token).phrase.compare ("bjornì"));
            assert (noun_parser (token).part == ADJECTIVE);


            // make verbs add "it-" and transitive verb suffix
            token.back () = "m_v";
            assert (!noun_parser (token).phrase.compare ("itbjornan"));
            assert (noun_parser (token).part == VERB);

            // become verbs add "it-" and intransitive suffix
            token.back () = "b_v";
            assert (!noun_parser (token).phrase.compare ("itbjornìt"));
            assert (noun_parser (token).part == VERB);

            // act verbs add "ge-" and transtivie suffix
            token.back () = "a_v";
            assert (!noun_parser (token).phrase.compare ("gebjornan"));
            assert (noun_parser (token).part == VERB);

            cout << "Passed all noun function derivation tests" << endl;
        }

        static void TEST_multiple_options () {
            vector<string> token = {"friend", "acc", "sing", "def"};

            assert (!noun_parser (token).phrase.compare ("at vaďàr"));

            token.at (1) = "soc";
            token.at (2) = "plur";
            token.at (3) = "indef";
            token.push_back ("m");
            assert (!noun_parser (token).phrase.compare ("enìs vaďeiśìs"));

            token.push_back ("adj");
            assert (!noun_parser (token).phrase.compare ("vaďì"));

            cout << "Passed all noun function multiple options tets" << endl;
        }
};