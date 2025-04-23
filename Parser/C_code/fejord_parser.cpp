#include <iostream>
#include <fstream>
#include <cstring>
#include "fejord_parser.h"

using namespace std;

int file_length (ifstream &file);
void get_json (enum json_type file_type);
string get_word (string base);
string noun_suffix (enum noun_case &word_case, enum word_gender &gender, enum word_plurality &plurality);
int noun_suffix_length (enum noun_case &word_case, enum word_gender &gender, enum word_plurality &plurality);
void add_prefix (string &phrase, string &prefix);

vector<string> word_divider (string &input) {
    vector<string> words;

    // Dividing characters are ' ' and '[' ( utilized for punctuation marks )
    int ws_pos = input.find_first_of (" [");
    int prev_pos = 0;

    // For every time we encounter any dividing characters
    while (ws_pos != string::npos) {
        // Only add that word if is 1 more characters long
        if (ws_pos > prev_pos) 
            words.push_back (input.substr (prev_pos, ws_pos - prev_pos));
        
        // If the next character is a punctuation mark 
        if (input.at (ws_pos) == '[') {
            int end_pos = input.find_first_of (']', ws_pos);
            
            // Check that the braket closes
            if (end_pos != string::npos) {
                // Check that there is at least one character between the brakets
                if (end_pos - ws_pos > 1) 
                    words.push_back (input.substr (ws_pos + 1, end_pos - ws_pos - 1));

                // Move whitespace position index to end of braket
                ws_pos = end_pos;
            }
        }

        // The next dividing character search will not include the one that was found
        prev_pos = ws_pos + 1;
        ws_pos = input.find_first_of (" [", prev_pos);
    }

    // Make sure that all characters are searched in input string
    if (prev_pos != input.length ()) {
        words.push_back (input.substr (prev_pos));
    }

    return words;
}

vector<vector<string>> word_tokenizer (vector<string> &words) {
    vector<vector<string>> tokens;
    uint8_t token_idx = 0;

    // For each word, grab the base word
    for (string word : words) {
        int tok_pos = word.find_first_of (".");
        int prev_pos = 0;
        string base = word.substr (0, tok_pos);
        
        // Check that the base word is found in the fejord dictionary
        // if (!dictionary_search (base)) {
        //     cout << "ERROR: could not find " << base << "in fejord dictionary." << endl;
        //     return tokens;
        // }

        // Add the base word to current token vector ( if statement checks for period punctuation )
        if (tok_pos == 0) {
            tokens.push_back (vector<string> (1, "."));
            tok_pos++;
        } else  
            tokens.push_back (vector<string> (1, base));
        prev_pos = tok_pos + 1;

        // If a word only has a base or it was a punctuation mark, continue to the next word
        if (prev_pos <= 0 || prev_pos >= word.length ()) {
            token_idx++;
            continue;
        }
        
        // for each option, add it to the current token vector
        tok_pos = word.find_first_of (".", prev_pos);
        while (tok_pos != string::npos) {
            tokens.at (token_idx).push_back (word.substr (prev_pos, tok_pos - prev_pos));
            prev_pos = tok_pos + 1;
            tok_pos = word.find_first_of (".", prev_pos);
        } 
        
        // Make sure that all options are searched
        if (prev_pos != word.length ()) {
            tokens.at (token_idx).push_back (word.substr (prev_pos));
        }

        token_idx++;
    }

    return tokens;
}

bool dictionary_search (string &base) {
    get_json (DICT_JSON);
    cJSON *iterator = NULL;

    cJSON_ArrayForEach (iterator, dict_json) {
        cJSON *eng = cJSON_GetObjectItem (iterator, "english");
        if (!base.compare (eng->valuestring))    return true;

        // cJSON* fej = cJSON_GetObjectItem (iterator, "fejord");
        // cJSON* pos = cJSON_GetObjectItem (iterator, "part of speech");
        // cout << "fejord : " << fej->valuestring << "    english : " << eng->valuestring << "    pos : " << pos->valuestring << endl; 
    }

    return false;
}

word_token_t clause_parser (std::vector<std::string> &token) {
    word_token_t clause;
    get_json (CLAUSE_JSON);
    cJSON *iterator = NULL;   

    cJSON_ArrayForEach (iterator, clause_json) { 
        cJSON *key = cJSON_GetObjectItem (iterator, "key");
        if (!token.at (0).compare (key->valuestring)) {
            clause.phrase = cJSON_GetObjectItem (iterator, "word")->valuestring;
            clause.part = CLAUSE;
            return clause;
        }
    }

    clause.part = ERROR;
    return clause;
}

word_token_t noun_parser (vector<string> &token) {
    word_token_t noun;
    noun.part = SUBJECT;
    get_json (NOUN_JSON);
    string word = get_word (token.at (0));
    noun.phrase = word;

    // Check that word is not an empty string 
    if (!noun.phrase.empty ()) {
        // Derive the gender from the dictionary form of the word
        enum word_gender gender;
        enum word_plurality plurality = SING;
        enum noun_case word_case = NOMINATIVE;
        switch (noun.phrase.at (word.length () - 2)) {
            case 'a':       gender = F; break;
            case 'e':       gender = M; break;
            default:        gender = N; break;
        }

        // iterate through each token
        for (vector<string>::iterator it = token.begin () + 1; it != token.end(); it++) {
            if (!(*it).compare ("F")) {
                // Grab the length of the current suffix
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality);
                
                // Grab the new suffix
                gender = F;
                string new_suffix = noun_suffix (word_case, gender, plurality);
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + new_suffix;
            }
            
            else if (!(*it).compare ("M")) {
                // Grab the length of the current suffix
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality);
                
                // Grab the new suffix
                gender = M;
                string new_suffix = noun_suffix (word_case, gender, plurality);
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + new_suffix;
            }
            
            else if (!(*it).compare ("N")) {
                // Grab the length of the current suffix
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality);
                
                // Grab the new suffix
                gender = N;
                string new_suffix = noun_suffix (word_case, gender, plurality);
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + new_suffix;
            }
            
            

            else if (!(*it).compare ("adj")) {
                // Grab the array that holds the gendered adjective suffixes
                cJSON *adj_array = cJSON_GetObjectItem (cJSON_GetObjectItem (noun_json->child->next->next->next, "declensions")->child, "word")->child;

                // Grab the correct suffix based dictionary word's gender
                for (uint8_t i = 0; i < gender; i++) adj_array = adj_array->next;
                string suffix = cJSON_GetObjectItem (adj_array, "word")->valuestring;

                // Grab the lenght of the current noun's suffix
                uint8_t curr_suffix_length = noun_suffix_length (word_case, gender, plurality);

                // Grab the gendered adjective suffix, and replace the base suffix with it
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + suffix;
                noun.part = ADJ;
            } 
            
            else if (!(*it).compare ("m_verb")) {
                // Grab the prefix for the make verb declensions
                string prefix = cJSON_GetObjectItem (cJSON_GetObjectItem (noun_json->child->next->next->next, "declensions")->child->next, "word")->valuestring;

                // Grab the lenght of the current noun's suffix
                uint8_t curr_suffix_length = noun_suffix_length (word_case, gender, plurality);

                // Add the prefix and remove the suffix
                add_prefix (noun.phrase, prefix);
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length);

                noun.part = VERB;
            }

            else if (!(*it).compare ("a_verb")) {
                // Grab the prefix for the make verb declensions
                string prefix = cJSON_GetObjectItem (cJSON_GetObjectItem (noun_json->child->next->next->next, "declensions")->child->next->next, "word")->valuestring;

                // Grab the lenght of the current noun's suffix
                uint8_t curr_suffix_length = noun_suffix_length (word_case, gender, plurality);

                // Add the prefix and remove the suffix
                add_prefix (noun.phrase, prefix);
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length);

                noun.part = VERB;
            }



            else if (!(*it).compare ("def")) {
                // Grab the correct definite article based on the noun's gender
                cJSON *art_array = cJSON_GetObjectItem (noun_json->child->next, "declensions")->child;
                for (uint8_t i = 0; i < gender; i++) art_array = art_array->next;

                string art = cJSON_GetObjectItem (art_array, "word")->valuestring;
                noun.phrase = art + " " + noun.phrase;
            }

            else if (!(*it).compare ("ind")) {
                // Grab the correct indefinite article based on the noun's gender
                cJSON *art_array = cJSON_GetObjectItem (noun_json->child->next, "declensions")->child->next->next->next;
                for (uint8_t i = 0; i < gender; i++) art_array = art_array->next;

                string art = cJSON_GetObjectItem (art_array, "word")->valuestring;
                noun.phrase = art + " " + noun.phrase;
            }



            else if (!(*it).compare ("plur")) {
                // Grab the length of the current declension
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality); 

                // Change the plurality and get the new suffix
                plurality = PLUR;
                string suffix = noun_suffix (word_case, gender, plurality);

                // Grab the suffixes and exchange it
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + suffix;
            }

            else if (!(*it).compare ("sing")) {
                // Grab the length of the current declension
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality); 

                // Change the plurality and get the new suffix
                plurality = SING;
                string suffix = noun_suffix (word_case, gender, plurality);

                // Grab the suffixes and exchange it
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + suffix;
            }



            else if (!(*it).compare ("aff")) {
                string prefix = cJSON_GetObjectItem (cJSON_GetObjectItem (noun_json->child->next->next, "declensions")->child, "word")->valuestring;
                add_prefix (noun.phrase, prefix);
            }

            else if (!(*it).compare ("neg")) {
                string prefix = cJSON_GetObjectItem (cJSON_GetObjectItem (noun_json->child->next->next, "declensions")->child->next, "word")->valuestring;
                add_prefix (noun.phrase, prefix);
            }



            else {
                // Grab the length of the case suffix of current noun
                uint8_t curr_suffix_length = (uint8_t) noun_suffix_length (word_case, gender, plurality);

                // Get the new case for the noun
                if      (!(*it).compare ("nom"))    word_case = NOMINATIVE;
                else if (!(*it).compare ("acc"))    word_case = ACCUSATIVE;
                else if (!(*it).compare ("dat"))    word_case = DATIVE;
                else if (!(*it).compare ("voc"))    word_case = VOCATIVE;
                else if (!(*it).compare ("com"))    word_case = COMITATIVE;
                else if (!(*it).compare ("int"))    word_case = INSTRUMENTAL;
                else if (!(*it).compare ("soc"))    word_case = SOCIATIVE;
                else if (!(*it).compare ("loc"))    word_case = LOCATIVE;
                else if (!(*it).compare ("abl"))    word_case = ABLATIVE;
                else if (!(*it).compare ("inv"))    word_case = INESSIVE;
                else if (!(*it).compare ("ads"))    word_case = ADDESSIVE;
                else if (!(*it).compare ("gen"))    word_case = GENITIVE;
                else if (!(*it).compare ("erg"))    word_case = ERGATIVE;
                
                // Grab the suffix for the new case
                string new_suffix = noun_suffix (word_case, gender, plurality);

                // Remove the previous suffix and replace it with the new suffix
                noun.phrase = noun.phrase.substr (0, noun.phrase.length () - curr_suffix_length) + new_suffix;
            }
        }
    } else {
        noun.phrase = "";
        noun.part = ERROR;
    }
    
    return noun;
}




int file_length (ifstream &file) {
    file.seekg (0, file.end);
    int length = file.tellg ();
    file.seekg (0, file.beg);
    return length;
}

void get_json (enum json_type file_type) {
    // Find the path to the correct json file given an enum of the file type
    // At the same time, check that the json variable that has not been populated yet
    string path = "Fejord_dictionary/";
    switch (file_type) {
        case ADPOS_JSON: 
            if (adpos_json != NULL)        return;
            path += "adpos_declensions.json";
            break;

        case CLAUSE_JSON: 
            if (clause_json != NULL)        return;
            path += "clauses.json";
            break;

        case DICT_JSON: 
            if (dict_json != NULL)        return;
            path += "dictionary.json";
            break;

        case NOUN_JSON: 
            if (noun_json != NULL)        return;
            path += "noun_declensions.json";
            break;

        case OTHER_JSON: 
            if (other_json != NULL)        return;
            path += "other_declensions.json";
            break;

        case VERB_JSON: 
            if (verb_json != NULL)        return;
            path += "verb_conjugations.json";
            break;

        default: 
            cout << "ERROR: could not find file with enum type '" << file_type << "'." << endl;
            return;
    }  

    // Attempt to open the file
    ifstream file (path);
    if (!file) {
        cout << "ERROR: could not open '" << path << "'." << endl;
        return;
    }

    // Grab the length of the file, create a buffer of that size, 
    // and the data from the file onto the buffer
    int length = file_length (file);
    char *buffer = new char[length];
    file.read (buffer, length);
    file.close ();    

    // Parse the file data into the corresponding json variable
    switch (file_type) {
        case ADPOS_JSON:
            adpos_json = cJSON_Parse (buffer);
            adpos_json = cJSON_GetObjectItem (adpos_json, "declensions");
        case CLAUSE_JSON: 
            clause_json = cJSON_Parse (buffer);
            clause_json = cJSON_GetObjectItem (clause_json, "clauses");
            break;

        case DICT_JSON: 
            dict_json = cJSON_Parse (buffer);
            dict_json = cJSON_GetObjectItem (dict_json, "dictionary");
            break;

        case NOUN_JSON: 
            noun_json = cJSON_Parse (buffer);
            noun_json = cJSON_GetObjectItem (noun_json, "declensions");
            break;

        case OTHER_JSON: 
            other_json = cJSON_Parse (buffer);
            other_json = cJSON_GetObjectItem (other_json, "declensions");
            break;

        case VERB_JSON: 
            verb_json = cJSON_Parse (buffer);
            verb_json = cJSON_GetObjectItem (verb_json, "declensions");
            break;
    }
}

string get_word (string base) {
    get_json (DICT_JSON);
    cJSON *iterator = NULL;

    cJSON_ArrayForEach (iterator, dict_json) {
        cJSON *eng = cJSON_GetObjectItem (iterator, "english");
        if (!base.compare (eng->valuestring))    return cJSON_GetObjectItem (iterator, "fejord")->valuestring;
    }

    cout << "ERROR: could not find base translation for word '" << base << "'." << endl; 
    return "";
}

string noun_suffix (enum noun_case &word_case, enum word_gender &gender, enum word_plurality &plurality) {
    // Grab the case
    cJSON *case_array = cJSON_GetObjectItem (noun_json->child, "declensions")->child;
    for (uint8_t i = 0; i < word_case; i++) case_array = case_array->next; 

    // Grab the gende
    case_array = cJSON_GetObjectItem (case_array, "suffixes")->child;
    for (uint8_t i = 0; i < gender; i++) case_array = case_array->next->next;

    // Grab the plurality
    if (plurality == PLUR) case_array = case_array->next; 

    // return the suffix
    return cJSON_GetObjectItem (case_array, "word")->valuestring;
}

int noun_suffix_length (enum noun_case &word_case, enum word_gender &gender, enum word_plurality &plurality) {
    return noun_suffix (word_case, gender, plurality).length ();
}

void add_prefix (string &phrase, string &prefix) {
    int idx = phrase.find_first_of (" ");
    if (idx != string::npos) {
        phrase = phrase.substr (0, idx + 1) + prefix + phrase.substr (idx + 1);                 
    } else {
        phrase = prefix + phrase;
    }
}
 