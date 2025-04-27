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

            // Check if it-like translates to "kamì"
            token.at (0) = "it";
            assert (noun_parser (token).phrase == "kamì");

            // TEST MAKE VERB DECLENSION
            // Check if "to adventure" translates to "etbajorìt"
            token.at (0) = "adventure";
            token.at (1) = "m_verb";
            assert (noun_parser (token).phrase == "etbajorìt");

            // TEST ACT VERB DECLENSION
            // Check if "to act like a cloud" translates to "gabjornìt"
            token.at (0) = "cloud";
            token.at (1) = "a_verb";
            assert (noun_parser (token).phrase == "gabjornìt");

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
            assert (noun_parser (token).phrase == "ats folbjornahes");

            // Check that a change in the order of options does not change the final outcome
            token.at (3) = "dat";
            token.at (2) = "sing";
            token.at (4) = "neg";
            token.at (1) = "def";
            assert (noun_parser (token).phrase == "at folbjornah");

            // Check that the article follows the plurality of its noun
            token.pop_back ();
            token.pop_back ();
            token.at (1) = "plur";
            token.at (2) = "ind";
            assert (!noun_parser (token).phrase.compare ("ains bjornatìs"));

            // Check that the article can be removed
            token.at (1) = "def";
            token.at (2) = "abs";
            assert (!noun_parser (token).phrase.compare ("bjornat"));
            
            // Check that the affirmation can be removed
            token.at (1) = "aff";
            token.at (2) = "no_con";
            assert (!noun_parser (token).phrase.compare ("bjornat"));

            cout << "Passed all complex noun tests" << endl;
        }
};

class adjective_test {
    public:
        static void TEST_gender_change () {
            vector<string> token;

            // Check if masculine happy adjective is "pire"
            token.push_back ("happy");
            token.push_back ("M");
            assert (!adjective_parser (token).phrase.compare ("pire"));

            cout << "Passed all adjective gender change tests" << endl;
        }

        static void TEST_plurality_change () {
            vector<string> token;

            // Check if plural good adjective is "indìs"
            token.push_back ("good");
            token.push_back ("plur");
            assert (!adjective_parser (token).phrase.compare ("indìs"));

            cout << "Passed all adjective plurality change tests" << endl;
        }

        static void TEST_comparatives () {
            vector<string> token;

            // Check to see if blue is "ledvìb"
            token.push_back ("blue");
            token.push_back ("comp");
            assert (!adjective_parser (token).phrase.compare ("ledvìb"));

            // Check to see if bluest is "ledlìf"
            token.at (1) = "super";
            assert (!adjective_parser (token).phrase.compare ("ledlìf"));

            cout << "Passed all adjective comparative tests" << endl;
        }

        static void TEST_to_other_speeches () {
            vector<string> token;

            // Check to see if blue turned into a quality abstract noun is "ìrledot"
            token.push_back ("blue");
            token.push_back ("qual_n");
            assert (!adjective_parser (token).phrase.compare ("ìrledot"));

            // Check to see if blue turned into a condition abstract noun is "ìnledot"
            token.at (1) = "cond_n";
            assert (!adjective_parser (token).phrase.compare ("ìnledot"));

            // Check to see if blue turned into a condition abstract noun is "ledot"
            token.at (1) = "char_n";
            assert (!adjective_parser (token).phrase.compare ("ledot"));
            
            // Check to see if blue turned into a "make" verb is "ìtledìt"
            token.at (1) = "make_v";
            assert (!adjective_parser (token).phrase.compare ("ìtledìt"));

            cout << "Passed all adjective to other speeches tests" << endl;
        }

        static void TEST_complex_adjectives () {
            vector <string> token;

            // Check to see if a comparitive characteristic noun turned female blue is "vàledat"
            token.push_back ("blue");
            token.push_back ("F");
            token.push_back ("comp");
            token.push_back ("char_n");
            assert (!adjective_parser (token).phrase.compare ("vàledat"));

            cout << "Passed all complex adjective tests" << endl;
        }
};

class adverb_test {
    public:
        static void TEST_comparitives () {
            vector<string> token;

            // Check to see if a comparitive forever is "jordbeih"
            token.push_back ("forever");
            token.push_back ("comp");
            assert (!adverb_parser (token).phrase.compare ("jordbeih"));

            cout << "Passed all comparitive adverb tests" << endl;
        }

        static void TEST_to_other_speeches () {
            vector<string> token;

            // Check to see if here as an adposition is "menetkor"
            token.push_back ("here");
            token.push_back ("adpos");
            assert (!adverb_parser (token).phrase.compare ("menetkor"));

            // Check to see if here as a noun is "menetot"
            token.at (1) = "noun";
            assert (!adverb_parser (token).phrase.compare ("menetot"));

            cout << "Passed all adverb to other speeches tests" << endl;
        }
};

class number_test {
    public:
        static void TEST_to_other_speeches () {
            vector<string> token;

            // Check to see if 1st is "1hur"
            token.push_back ("1");
            token.push_back ("rank");
            assert (!number_parser (token).phrase.compare ("1hur"));

            // Check to see if 1 as a nounr is "1ot"
            token.at (1) = "noun";
            assert (!number_parser (token).phrase.compare ("1ot"));

            cout << "Passed all number to other speeches tests" << endl;
        }
};