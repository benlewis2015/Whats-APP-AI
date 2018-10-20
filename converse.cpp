#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <cmath>
#include <iterator>
#include <vector>
#include "ExtractConvo.h"

/*                          README
****************************************************************
* This file takes the optimized convo stored in a C++ vector   *
* and predicts the response based on data stored in several    *
* maps. This procedure is explained below.                     *
*                                                              *
* NOTE: The conversation is stored in a vector whose format is *
* explained in further detail in "ExtractConvo.h"              *
*                                                              *
* 1. The vectors strings are broken up in sets containing the  * 
*    unique words in each of the strings stored in the vector  *
* 2. These words are stored in several various maps used to  | *
*    predict the response later on. The maps are explained   v *
*      - word_count - will store the word and how many times   *
*        it's used throughout the vector. This will be used to *
*        calculate the "strength" of the word.                 *
*        ex. "and" would have a lower strength than "umbrella" *
*      - text_response - stores a text and the next text in the*
*        vector (the response to that text)                    *
*      - word_text - stores a unique word, and then stores the *
*        associated text in a vector. When a user entered word *
*        is found in this map, all the texts found in the      *
*        vector are given an associated double stored in the   *
*        next map (text_likelyhood)                            *
*      - text_likelyhood - stores each unique text associated  *
*        with each unique user entered word, and an associated *
*        double, that increases based on the strength of the   *
*        words found within that match with a user entered word*
* 3. Once the maps are filled the response will be determined  *
*    based on the highest double stored in text_likelyhood     *
****************************************************************
*/

using namespace std;

class ComputerMessenger {
public:
  ComputerMessenger (vector<string> convo) {
    convo_size = convo.size ();
    for (int i = 0; i < convo_size; ++i) {
      set<string> words = unique_words (convo.at (i));
      for (auto j: words) {
        word_count[j] += 1;
        word_text[j].insert (convo.at (i));
      }
      if (i < convo.size () - 1) {
        text_response[convo.at (i)] = convo.at (i + 1);
      }
      else text_response[convo.at (i)] = "Bye";
    }
  }

  string likely_response (string str) {
    auto it = text_response.find (closest_text (str));
    text_likelyhood.clear ();
    return it->second;
  }

  string closest_text (string str) {
    set<string> words = unique_words (str);
    for (auto i : words) {
      double strength = log(convo_size / (double)word_count[i]);
      auto it = word_text.find (i);
      if (it != word_text.end ()) {
        set<string> resp = it->second;
        for (auto it : resp) {
          text_likelyhood[it] += strength;
        }
      }
    }
    words.clear ();
    return find_max_count (str);
  }

  string find_max_count (string text) {
    string likely = " ";
    double max_count = -100000;
    for (auto i : text_likelyhood) {
      if (i.second - max_count > 0.0001) {
        likely = i.first;
        max_count = i.second;
      }
      else if (abs(i.second - max_count) < 0.0001) {
        likely = second_criteria (likely, i.first, text);
        max_count = text_likelyhood[likely];
      }
    }
    return likely;
  }

  string second_criteria (string likely, string challenger, string text) {
    unsigned int one = likely.length () - text.length ();
    unsigned int two = challenger.length () - text.length ();
    if (one > two) return challenger;
    if (two > one) return likely;
    srand (text.length ());
    int rand_choice = rand () % 2 + 1;
    if (rand_choice == 1) return likely;
    else if (rand_choice == 2) return challenger;
    assert (false);
  }

private:
  map<string, set<string>> word_text;
  map<string, int> word_count;
  map<string, string> text_response;
  map<string, double> text_likelyhood;
  int convo_size;

  set<string> unique_words (const string &str) {
    istringstream source{ str };
    return { istream_iterator<string>{source},
             istream_iterator<string>{} };
  }
};

bool get_convo_from_file(vector<string> &v);
void convo_driver (ComputerMessenger &train);

int main() {
  vector<string> convo;
  if(!get_convo_from_file(convo)) return 1;
  ComputerMessenger train(convo);
  convo_driver (train);
  return 0;
}

// REQUIRES: v is empty
// MODIFIES: v
// EFFECTS: Fills v with the conversation from filename
// returns true if the file name is valid, returns false if the 
// filename isn't valid
bool get_convo_from_file(vector<string> &v) {
  string filename;  
  cout << "Enter the name of the WhatsApp Chat: ";
  getline(cin, filename);

  ConvoExtractor ce (filename, v);
  if (!ce.is_open()) {
    cout << "Error opening " << filename;
    return false;
  }
  return true;
}

void convo_driver (ComputerMessenger &train) {
  string text;
  cout << "Computer: Hello\n" << endl;
  while (text != "Bye") {
    cout << "You: ";
    getline (cin, text);
    if (text == "Bye") break;
    cout << "\nComputer: " << train.likely_response (text) << endl << endl;
  }
}
