#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

/*                          README                            
****************************************************************
* A WhatsApp generated .txt file has several text identifying  *
* and distinguishing features. This code's purpose is to       *
* breakup and sort each line of the txt file into a C++        *
* readable vector designed to be read and processed by a       *
* machine learning class in order to allow the computer to     *
* respond to user entered strings with reasonable coherency    *
* based on the learned patterns found within the WhatsApp file *
*                                                              *
* The three main algorithms in this code are designed to break *
* the file up into chuncks of strings based on three key       *
* possibilities.                                               *
* 1. The text is one line sent by a different sender than the  *
*    previous text and is stored in a unique vector position   *
* 2. The text is sent by the same sender as a previous sender  *
*    and the string is stored in the same vector location as   *
*    the previous text.                                        *
* 3. The newline is a continuation of the same text message    *
*    and is identified by the 4th word in the text being !'-'  *
****************************************************************
*/

// REQUIRES: string str has a word with the last character ':' 
// Chat users have different distinguishable names in the file
// MODIFIES: str
// EFFECTS: removes the timestamp and name from the string
// groups messages from the same sender together
static bool optimize_string (std::string &str, std::string &last);

// REQUIRES: str is directly from txt file and hasn't been modified,
// if the third word is '-' then the string must be a new text
// EFFECTS: returns false if a new text is detected by the '-' symbol
// returns false if the text is a continuation of
// previous text with a newline 
static bool is_continued_text (const std::string str);

class ConvoExtractor {
public:
  // REQUIRES: string filename is a valid WhatsApp chat.txt file
  // MODIFIES: vector<string> convo
  // EFFECTS: fills data vector with strings read from the .txt file
  // calls helper funtions to optimize each line depending on the 
  // the previous sender as well as newlines within a single text message
  ConvoExtractor (std::string filename, std::vector<std::string> &convo) {
    std::string last_sender;
    infs.open (filename);
    if (is_open ()) {
      std::string text;
      while (getline(infs, text)) {
        if (optimize_string (text, last_sender)) {
          convo.push_back (text);
        }
        else convo.back () = convo.back () + '\n' + text;
      }
    }
  }

  // EFFECTS: returns true if the file is opened
  bool is_open () {
    if (infs.is_open ()) return true;
    else return false;
  }

private:
  std::fstream infs;
};


static bool optimize_string (std::string &str, std::string &last_sender) {
  // if the text doesn't have the 4th word being '-' the text is a
  // continuation of the previous text. returns to class constructor
  if (is_continued_text (str)) return false;
  std::string word;
  std::string sender;
  std::string optimized;
  std::istringstream stream(str);
  bool start_optimizing = false;

  while (stream >> word) {
    if (start_optimizing) {
      optimized = optimized + word + " ";
    }
    // code not needed once ran once optimization started
    // if word.back() == ':', signifies begining of text
    if (!start_optimizing && word.back () == ':') { 
      start_optimizing = true;
      sender = word;
    }

  }
  bool last_equal_name = sender == last_sender; // same sender as previous text
  last_sender = sender; // last is the name of current recently optimized text
  str = optimized;
  return !last_equal_name;
}

static bool is_continued_text (const std::string str) {
  // all new text strings should be > 18 in all cases
  if (str.length () < 18) return true;

  std::istringstream stream (str);
  std::string word;
  int counter = 0;

  while (stream >> word) {
    // word != "-" means it's a continued text
    if (counter == 3) return word != "-";
    counter++;
  }
  return true; // counter never reached 3, (continued text)
}