//
//  56_massive_funs.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/16/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <string>
#include <string_view>

using namespace std;

void massiveFunction(string_view text, int country_id,
                     int max_word_count, int max_word_length, int max_form_count_per_word,
                     bool allow_minus_words, bool keep_stop_words, bool case_sensitive) { }

////////////////////////////////////////////////////////////////////////////////////

struct Params {
    int max_word_count;
    int max_word_length;
    int max_form_count_per_word;
    bool allow_minus_words;
    bool keep_stop_words;
    bool case_sensitive;
};

void massiveFunctionSolution1(string_view text, int country_id, const Params& p) { }

////////////////////////////////////////////////////////////////////////////////////

enum class StopWordsMode {
    IGNORE,
    KEEP,
    KEEP_FIRST
};

void massiveFunctionSolution2(string_view text, int country_id,
                              int max_word_count, int max_word_length, int max_form_count_per_word,
                              bool allow_minus_words, StopWordsMode stop_words, bool case_sensitive) { }

////////////////////////////////////////////////////////////////////////////////////

enum class WordsFlag {
    IGNORE      = 1 << 0, //001
    KEEP        = 1 << 1, //010
    KEEP_FIRST  = 1 << 2  //100
};

WordsFlag operator&(const WordsFlag& l, const WordsFlag& r) {
    return static_cast<WordsFlag>(static_cast<int>(l) & static_cast<int>(r));
}

WordsFlag operator|(const WordsFlag& l, const WordsFlag& r) {
    return static_cast<WordsFlag>(static_cast<int>(l) | static_cast<int>(r));
}

void massiveFunctionSolution3(string_view text, int country_id,
                              int max_word_count, int max_word_length, int max_form_count_per_word,
                              WordsFlag wordConfig) {
    if ((wordConfig & WordsFlag::IGNORE) == WordsFlag::IGNORE) {

    }
    if ((wordConfig & WordsFlag::KEEP) == WordsFlag::KEEP) {

    }
    if ((wordConfig & WordsFlag::KEEP_FIRST) == WordsFlag::KEEP_FIRST) {

    }
}

////////////////////////////////////////////////////////////////////////////////////

struct CountryId {
    int value;
    explicit CountryId(int v) : value { v } { }
    operator int() const { return value; }
};

struct MaxWordsCount {
    int value;
    explicit MaxWordsCount(int v) : value { v } { }
    operator int() const { return value; }
};

//eliminate boilerplate code with a macros for int params
#define DECLARE_INT_PARAM(name) \
struct name { \
int value; \
explicit name(int v) : value { v } { } \
operator int() const { return value; } \
}

//eliminate boilerplate code with a universal macros for any type of params
#define DECLARE_PARAM(name, type) \
struct name { \
type value; \
explicit name(type v) : value { v } { } \
operator type() const { return value; } \
}

//declare params with macros
DECLARE_INT_PARAM(MaxWordLength);
DECLARE_PARAM(MaxFormCountPerWord, double);

MaxWordLength operator""_max_length(unsigned long long value) {
    return MaxWordLength(value);
}

void massiveFunctionSolution4(string_view text, CountryId country_id,
                                MaxWordsCount max_word_count, MaxWordLength max_word_length,
                                MaxFormCountPerWord max_form_count_per_word,
                                bool allow_minus_words, bool keep_stop_words, bool case_sensitive) {
    //with th function 'operator type() const { return value; }'
    //params are automatically cast to underlying type
    int countryId = country_id;
    int maxWordCount = max_word_count;
    double maxFormCountPerWord = max_form_count_per_word;
}

int main56() {
    //Approach 1 - comments
    //very unreadable call
    massiveFunction("hello world", 10, 100500, 200, 30, false, false, true);
    
    //better, but still bad
    massiveFunction(/* text */                    "hello world",
                    /* country_id */              10,
                    /* max_word_count */          100500,
                    /* max_word_length */         200,
                    /* max_form_count_per_word */ 30,
                    /* allow_minus_words */       false,
                    /* keep_stop_words */         false,
                    /* case_sensitive */          true);
    
    
    //Approach 2 - params as a struct
    Params params {
        .max_word_count = 100500,
        .max_word_length = 200,
        .max_form_count_per_word = 30,
        .allow_minus_words = false,
        .keep_stop_words = false,
        .case_sensitive = true
    };
    massiveFunctionSolution1("hello world", 10, params);
    
    
    //Approach 3 - use enums instead of bools
    massiveFunctionSolution2(/* text */                    "hello world",
                             /* country_id */              10,
                             /* max_word_count */          100500,
                             /* max_word_length */         200,
                             /* max_form_count_per_word */ 30,
                             /* allow_minus_words */       false,
                             /* stop_words */              StopWordsMode::IGNORE, //more readable
                             /* case_sensitive */          true);
    
    
    //Approach 4 - use bit flags for bool values
    massiveFunctionSolution3(/* text */                   "hello world",
                            /* country_id */              10,
                            /* max_word_count */          100500,
                            /* max_word_length */         200,
                            /* max_form_count_per_word */ 30,
                            /* word flags */              WordsFlag::IGNORE | WordsFlag::KEEP | WordsFlag::KEEP_FIRST);
    
    WordsFlag flag = WordsFlag::IGNORE | WordsFlag::KEEP | WordsFlag::KEEP_FIRST;
    cout << static_cast<int>(flag) << endl;
    WordsFlag operationResult = flag & WordsFlag::IGNORE;
    cout << static_cast<int>(operationResult) << endl;
    
    
    //Approach 5 - use separate type for each parameter AND user-defined literals
    massiveFunctionSolution4("", CountryId(10), MaxWordsCount(100500), 200_max_length,
                             MaxFormCountPerWord(1.1), false, false, true);
    
    return 0;
}
