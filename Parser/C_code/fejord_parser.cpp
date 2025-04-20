#include <iostream>
#include <fstream>
#include "fejord_parser.h"

using namespace std;

int file_length (ifstream &file);

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
    ifstream dict ("Fejord_dictionary/dictionary.json");
    
    if (!dict) {
        cout << "ERROR: could not open dictionary.json." << endl;
        return false;
    }

    int length = file_length (dict);
    char *buffer = new char[length];
    dict.read (buffer, length);
    dict.close ();

    cJSON *json = cJSON_Parse (buffer);
    cJSON *dict_array = cJSON_GetObjectItem (json, "dictionary");
    cJSON *iterator = NULL;
    cJSON_ArrayForEach (iterator, dict_array) {
        cJSON *eng = cJSON_GetObjectItem (iterator, "english");
        if (!base.compare (eng->valuestring))    return true;

        // cJSON* fej = cJSON_GetObjectItem (iterator, "fejord");
        // cJSON* pos = cJSON_GetObjectItem (iterator, "part of speech");
        // cout << "fejord : " << fej->valuestring << "    english : " << eng->valuestring << "    pos : " << pos->valuestring << endl; 
    }

    return false;
}

word_token_t clause_parser (std::vector<std::string> &token) {
    ifstream clauses_file ("Fejord_dictionary/clauses.json");
    word_token_t clause;

    if (!clauses_file) {
        cout << "ERROR: could not open dictionary.json." << endl;
        return clause;
    }

    int length = file_length (clauses_file);
    char *buffer = new char[length];
    clauses_file.read (buffer, length);
    clauses_file.close ();

    cJSON *json = cJSON_Parse (buffer);
    cJSON *array = cJSON_GetObjectItem (json, "clauses");
    cJSON *iterator = NULL;   
    cJSON_ArrayForEach (iterator, array) { 
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




int file_length (ifstream &file) {
    file.seekg (0, file.end);
    int length = file.tellg ();
    file.seekg (0, file.beg);
    return length;
}
 