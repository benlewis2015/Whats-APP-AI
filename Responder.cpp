#include "Responder.h"

static double absolute (double a);

Responder::Responder (std::vector<std::string> &conversation) {
  convo = std::move (conversation);
  convo_size = (int)convo.size ();
  randomNum = 0;

  for (int i = 0; i < convo_size; ++i) {
    unique_words (convo.at (i), i);
  }
  numUniqueWords = (int)word_count_text.size ();
}

std::string Responder::likely_response (const std::string &str) {
  userText = str;
  int i = closest_text ();
  text_strength.clear ();
  if (i == -1)
    return "What?";
  return i == convo_size - 1 ? "Bye" : convo.at (i + 1);
}

int Responder::closest_text () {
  std::set<std::string> words = unique_words_set (userText);
  for (auto i : words) {
    auto foundWord = word_count_text.find (i);
    if (foundWord != word_count_text.end ()) {
      int size = (int)foundWord->second.second.size ();
      std::vector<int> &indexes = foundWord->second.second;
      double strength = (double)foundWord->second.first / (double)numUniqueWords;

      for (int i = 0; i < size; ++i) {
        text_strength[indexes[i]] += strength;
      }

    }
  }
  words.clear ();
  return find_max_count ();
}

int Responder::find_max_count () {
  int likely = -1;
  double maxStrength = -1.0;
  for (auto i : text_strength) {
    if (i.second - maxStrength > 0.0001) {
      likely = i.first;
      maxStrength = i.second;
    } else if (absolute (i.second - maxStrength) < 0.001) {
      likely = second_criteria (likely, i.first);
      maxStrength = text_strength[likely];
    }
  }
  return likely;
}

int Responder::second_criteria (const int current, const int challenger) {
  srand ((int)userText.length () + randomNum);
  int rand_choice = rand () % 2 + 1;
  rand_choice == 1 ? ++randomNum : randomNum *= randomNum;
  if (rand_choice == 1)
    return current;
  return challenger;
}

void Responder::unique_words (const std::string &str, const int index) {
  std::istringstream source{ str };
  std::string word;
  while (source >> word) {
    word_count_text[word].first += 1;
    auto i = word_count_text.find (word);
    if (!(int)i->second.second.size ())
      word_count_text[word].second.push_back (index);
    else if (i->second.second.back () != index)
      word_count_text[word].second.push_back (index);
  }
}

std::set<std::string> Responder::unique_words_set (const std::string &str) {
  std::istringstream source{ str };
  return { std::istream_iterator<std::string>{source},
           std::istream_iterator<std::string>{} };
}

static double absolute (double a) {
  if (a < 0)
    return a * -1;
  else
    return a;
}


