#ifndef RESPONDER_H
#define RESPONDER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <utility>
#include <set>
#include <cmath>
#include <limits>
#include <iterator>
#include <vector>

#define COUNT_TEXT std::pair<int, std::vector<int>> 

/*                          README
****************************************************************
* This file takes the optimized convo stored in a C++ vector   *
* and predicts the response based on data stored in several    *
* maps. This procedure is explained below.                     *
*                                                              *
* NOTE: The conversation is stored in a vector whose format is *
* explained in further detail in "ExtractConvo.h"              *
* NOTE: CODE UPDATED TO USE INDICES INSTEAD OF COPYING STRINGS *
*                                                              *
* 1. The vectors strings are broken up in sets containing the  *
*    unique words in each of the strings stored in the vector  *
* 2. These words are stored in several various maps used to  | *
*    predict the response later on. The maps are explained   v *
*      - word_count_text - will store the word and how many    *
*        times it's used throughout the vector. This will be   *
*        used to calculate the "strength" of the word.         *
*        ex. "and" would have a lower strength than "umbrella" *
*      - The "_text" part - stores a unique word, and then     *
*        stores the associated text in a vector.               *
*        When a user entered word is found in this map, all    *
*        the texts found in the vector are given an associated *
*        double stored in the next map (text_strength)         *
*      - text_strength - stores each unique text associated    *
*        with each unique user entered word, and an associated *
*        double, that increases based on the strength of the   *
*        words found within that match with a user entered word*
* 3. Once the maps are filled the response will be determined  *
*    based on the highest double stored in text_likelyhood     *
****************************************************************
*/

class Responder {
private:

  // Reads words from the user entered text, finds all the texts in 
  // convo that contain the words, stores the amount of times a text
  // contains a word from the userText
  int closest_text ();

  // Picks the convo Text that contained the most strength relative to
  // the userText
  int find_max_count ();

  // Tie Breaker when strengths are similar
  int second_criteria (const int current, const int challenger);

  // Adds unique words and indices to word_count_text
  void unique_words (const std::string &str, const int index);

  // returns a set containing unique words from usertext
  std::set<std::string> unique_words_set (const std::string &str);

  std::unordered_map<std::string, COUNT_TEXT> word_count_text;
  std::unordered_map<int, int> text_response;
  std::unordered_map<int, double> text_strength;
  std::vector<std::string> convo;
  std::string userText;
  int convo_size;
  int numUniqueWords;
  int randomNum;

public:
  Responder (std::vector<std::string> &conversation);

  std::string likely_response (const std::string &str);
};

#endif // RESPONDER_H
