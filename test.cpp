#include <iostream>
#include <assert.h>
#include "Parser/C_code/fejord_parser.h"

using namespace std;

class clause_test {
    public:
        static void TEST_clauses () {
            vector<string> token;

            // Check if sub_cl translates to "fah"
            token.push_back ("sub_cl");
            assert (clause_parser (token).phrase == "fah");

            // Check if rel_cl translates to "vjen"
            token.at (0) = "rel_cl";
            assert (clause_parser (token).phrase == "vjen");

            // Check if sm_cl translates to ""
            token.at (0) = "sm_cl";
            assert (clause_parser (token).phrase == "");

            cout << "Passed all clause tests." << endl;
        }
};

class noun_test {
    public:
        static void TEST_gender_change () {
            vector<string> token;

            // Check that masculine sky translates to "aďet"
            token.push_back ("sky");
            token.push_back ("M");
            assert (noun_parser (token).phrase == "aďet");

            // Check that neuter dog translates to "gomot"
            token.at (0) = "dog";
            token.at (1) = "N";
            assert (noun_parser (token).phrase == "gomot");

            // Check that female you translates to uìntat 
            token.at (0) = "you";
            token.at (1) = "F";
            assert (noun_parser (token).phrase == "uìnat");
            cout << "Passed all gender change tests." << endl;
        }

        static void TEST_to_other_speeches () {
            vector<string> token;
            // TEST ADJECTIVE DECLENSION
            // Check if cloudy translates to "bjornà"
            token.push_back ("cloud");
            token.push_back ("adj");
            assert (noun_parser (token).phrase == "bjornà");

            // Check if city-like translates to "libe"
            token.at (0) = "city";
            assert (noun_parser (token).phrase == "libe");

            // // Check if it-like translates to "kamì"
            token.at (0) = "it";
            assert (noun_parser (token).phrase == "kamì");

            // TEST MAKE VERB DECLENSION
            // Check if "to adventure" translates to "itbajor"
            token.at (0) = "adventure";
            token.at (1) = "m_verb";
            assert (noun_parser (token).phrase == "itbajor");

            // TEST ACT VERB DECLENSION
            // Check if "to act like a cloud" translates to "gebjorn"
            token.at (0) = "cloud";
            token.at (1) = "a_verb";
            assert (noun_parser (token).phrase == "gebjorn");

            cout << "Passed all noun to other speeches tests." << endl;
        }

        static void TEST_article () {
            vector<string> token;

            // Check that a definitive article for sky is "at"
            token.push_back ("sky");
            token.push_back ("def");
            assert (noun_parser (token).phrase == "at aďat");

            // Check that an indefinite article for afternoon is "en"
            token.at (0) = "afternoon";
            token.at (1) = "ind";
            assert (noun_parser (token).phrase == "en lidéret");

            cout << "Passed all article tests" << endl;
        }
        
        static void TEST_plurality () {
            vector<string> token;

            // Check that a feminine nominative plural noun has the suffix "atìs"
            token.push_back ("day");
            token.push_back ("plur");
            assert (noun_parser (token).phrase == "ridatìs");

            cout << "Passed all plurality test" << endl;
        }

        static void TEST_case () {
            vector<string> token;

            // Check that a feminite comitative noun has the suffix "auf"
            token.push_back ("person");
            token.push_back ("com");
            assert (noun_parser (token).phrase == "vìrnauf");
            
            // Check that a masculine dative noun has the suffix "eh"
            token.at (0) = "city";
            token.at (1) = "dat";
            assert (noun_parser (token).phrase == "libeh");
            
            // Check that a neuter ablative noun has the suffix "vom"
            token.at (0) = "you";
            token.at (1) = "abl";
            assert (noun_parser (token).phrase == "uìnvom");

            cout << "Passed all case tests" << endl;
        }

        static void TEST_positivity () {
            vector<string> token;

            // TEST AFFIRMATION DECLENSION
            // Check if affirmed day translates to "retridat"
            token.push_back ("day");
            token.push_back ("aff");
            assert (noun_parser (token).phrase == "retridat");
            
            // Check if negated cloud translates to "folbjornat"
            token.at (0) = "cloud";
            token.at (1) = "neg";
            assert (noun_parser (token).phrase == "folbjornat");

            cout << "Passed all positivity tests" << endl;
        }

        static void TEST_complex_nouns () {
            vector<string> token;

            // Check if definite dative plural netated cloud translates to "at folbjornahes"
            token.push_back ("cloud");
            token.push_back ("def");
            token.push_back ("plur");
            token.push_back ("dat");
            token.push_back ("neg");
            assert (noun_parser (token).phrase == "at folbjornahes");

            cout << "Passed all complex noun tests" << endl;
        }
};
