#include <iostream>
#include "parser.h"

using namespace std;

template <typename T1, typename T2>
inline void check_noun_possibilites (bool &found, T1 &declension, T2 &options, string &opt);


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
        token.push_back (input.substr (prev_pos, delim_pos - prev_pos));
        prev_pos = delim_pos + 1;
    }    

    return token;
}

string translate_word (string &base) {
    if (!base.compare ("city"))         return "libet";
    if (!base.compare ("village"))      return "nerat";
    if (!base.compare ("wolf"))         return "kobet";
    if (!base.compare ("tree"))         return "denat";
    if (!base.compare ("it"))           return "kamot";
    if (!base.compare ("cloud"))        return "bjornat";
    if (!base.compare ("friend"))       return "vaďat";
}

word_token_t noun_parser (vector<string> &token) {
    word_token_t word;
    word.part = NOUN;
    word.order = 0;
    
    enum noun_case curr_case = NOMINATIVE; 
    enum noun_gender curr_gender;
    enum noun_plurality curr_plur = SINGULAR;
    enum noun_definiteness curr_def = GENERIC;
    enum noun_polarity curr_pol = NONE;

    bool case_found = false;
    bool gender_found = false;
    bool plurality_found = false;
    bool definitness_found = false;
    bool polarity_found = false;

    // Translate the base word
    word.phrase = translate_word (token.at (0));
    
    switch (word.phrase.at (word.phrase.length() - 2)) {
        case 'a':   curr_gender = FEMALE; break;
        case 'e':   curr_gender = MALE; break;
        case 'o':   curr_gender = NEUTER; break;
    };

    for (vector<string>::iterator it = token.begin() + 1; it != token.end(); it++) {
        check_noun_possibilites (case_found, curr_case, noun_case_options, *it);
        check_noun_possibilites (gender_found, curr_gender, noun_gender_options, *it);
        check_noun_possibilites (plurality_found, curr_plur, noun_plurality_options, *it);
        check_noun_possibilites (definitness_found, curr_def, noun_definiteness_options, *it);
        check_noun_possibilites (polarity_found, curr_pol, noun_polarity_options, *it);

        if (!(*it).compare ("adj")) {
            word.part = ADJECTIVE;
            word.phrase = word.phrase.substr (0, word.phrase.length () - 2) + "ì";
            return word;
        }

        if (!(*it).substr (1).compare ("_v")) {
            word.part = VERB;
            switch ((*it).at (0)) {
                case 'm': word.phrase = "it" + word.phrase.substr (0, word.phrase.length () - 2) + "an"; break;
                case 'b': word.phrase = "it" + word.phrase.substr (0, word.phrase.length () - 2) + "ìt"; break;
                case 'a': word.phrase = "ge" + word.phrase.substr (0, word.phrase.length () - 2) + "an"; break;
            }

            return word;
        }
    }

    string definite = noun_definiteness_word.at (curr_gender).at (curr_def).at (curr_plur);
    // Change the singular definite article depending if the base word begins with a dental stop or bilabial plosive 
    if (curr_def == DEFINITE && string ("tdpb").find(word.phrase.at(0)) != string::npos) {
        switch (curr_gender) {
            case FEMALE:    definite = "da"; break;
            case MALE:    definite = "de"; break;
            case NEUTER:    definite = "don"; break;
        }
    }

    // Apply all options
    word.phrase = (curr_def == GENERIC ? "" : (definite + " ")) + 
                (curr_pol == NONE ? "" : (noun_polarity_word.at (curr_gender).at (curr_pol)) + " ") + 
                word.phrase.substr (0, word.phrase.length () - 2) + noun_case_suffix.at(curr_case).at(curr_gender).at(curr_plur);

    return word;
}



template <typename T1, typename T2>
inline void check_noun_possibilites (bool &found, T1 &declension, T2 &options, string &opt) {
    if (!found) {
        auto option = options.find (opt);
        if (option != options.end ()) {
            declension = option->second;
            found = true;
        }
    }
}



void parser_init () {
    // NOUN CASE MAP
    noun_case_options["nom"] = NOMINATIVE;
    noun_case_options["acc"] = ACCUSATIVE;
    noun_case_options["dat"] = DATIVE;
    noun_case_options["voc"] = VOCATIVE;
    noun_case_options["com"] = COMITATIVE;
    noun_case_options["ins"] = INSTRUMENTAL;
    noun_case_options["soc"] = SOCIATIVE;
    noun_case_options["loc"] = LOCATIVE;
    noun_case_options["abl"] = ABLATIVE;
    noun_case_options["inn"] = INESSIVE;
    noun_case_options["add"] = ADDESSIVE;
    noun_case_options["gen"] = GENITIVE;
    noun_case_options["ex_erg"] = EX_ERGATIVE;
    noun_case_options["erg"] = ERGATIVE;
    noun_case_options["cau"] = CAUSATIVE;
    noun_case_options["adj"] = ADJUNCTIVE;

    // NOUN GENDER MAP
    noun_gender_options["f"] = FEMALE;
    noun_gender_options["m"] = MALE;
    noun_gender_options["n"] = NEUTER;

    // NOUN PLURALITY MAP
    noun_plurality_options["sing"] = SINGULAR;
    noun_plurality_options["plur"] = PLURAL;

    // NOUN CASE SUFFIX MAP
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"at", "atìs"}, vector<string> {"et", "ets"}, vector<string> {"ot", "ots"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"àr", "àrs"}, vector<string> {"er", "erìs"}, vector<string> {"oŕ", "oŕìs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"ah", "ahìs"}, vector<string> {"eh", "ehìs"}, vector<string> {"oh", "ohìs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"jal", "jalìs"}, vector<string> {"jel", "jelìs"}, vector<string> {"jol", "jolìs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"auf", "aufìs"}, vector<string> {"ef", "efìs"}, vector<string> {"of", "ofs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"auď", "auďs"}, vector<string> {"eď", "eďs"}, vector<string> {"oď", "oďs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"aiś", "aiśìs"}, vector<string> {"eiś", "eiśìs"}, vector<string> {"oś", "ośìs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"lak", "laks"}, vector<string> {"lek", "leks"}, vector<string> {"ok", "oks"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"vam", "vamìs"}, vector<string> {"vem", "vemìs"}, vector<string> {"vom", "vomìs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"ná", "nás"}, vector<string> {"ní", "nís"}, vector<string> {"nó", "nós"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"auz", "auzes"}, vector<string> {"ez", "ezes"}, vector<string> {"oz", "ozes"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"ìn", "ìns"}, vector<string> {"ìn", "ìns"}, vector<string> {"ìn", "ìns"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"ìnds", "ìnds"}, vector<string> {"ìnds", "ìnds"}, vector<string> {"ìnds", "ìnds"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"gen", "gens"}, vector<string> {"gen", "gens"}, vector<string> {"gen", "gens"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"ag", "ags"}, vector<string> {"eg", "egs"}, vector<string> {"og", "ogs"}});
    noun_case_suffix.push_back (vector<vector<string>> {vector<string> {"av", "avìs"}, vector<string> {"ev", "evìs"}, vector<string> {"ov", "ovìs"}});

    // NOUN DEFINITENESS MAP
    // static std::map<std::string, noun_definiteness> noun_definiteness_options;
    noun_definiteness_options["gen"] = GENERIC;
    noun_definiteness_options["def"] = DEFINITE;
    noun_definiteness_options["indef"] = INDEFINITE;
    
    // NOUN DEFINITENESS WORD MAP
    noun_definiteness_word.push_back (vector<vector<string>> {vector<string> {"", ""}, vector<string> {"at", "ats"}, vector<string> {"ain", "ains"}});
    noun_definiteness_word.push_back (vector<vector<string>> {vector<string> {"", ""}, vector<string> {"et", "etìs"}, vector<string> {"en", "enìs"}});
    noun_definiteness_word.push_back (vector<vector<string>> {vector<string> {"", ""}, vector<string> {"not", "nots"}, vector<string> {"ou", "ous"}});

    // NOUN POLARITY MAP
    noun_polarity_options["nop"] = NONE;
    noun_polarity_options["aff"] = AFFIRMATION;
    noun_polarity_options["neg"] = NEGATION; 

    // NOUN POLARITY WORD MAP
    noun_polarity_word.push_back (vector<string> {"", "rut", "fal"});
    noun_polarity_word.push_back (vector<string> {"", "reit", "fìl"});
    noun_polarity_word.push_back (vector<string> {"", "rot", "fol"});
}
