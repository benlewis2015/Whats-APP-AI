#include "Extract.h"
#include "Responder.h"
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;

bool get_convo_from_file(vector<string> &v);
void convo_driver (Responder &train);

int main() {
  vector<string> convo;
  if (!get_convo_from_file (convo))
    return 1;
  Responder train(convo);
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

  Extract ce (filename, v);
  if (!ce.is_open()) {
    cout << "ERROR: File \"" << filename << "\"";
    cout << "could not be open or invalid file format/type" << endl;
    return false;
  }
  return true;
}

void convo_driver (Responder &computer) {
  string text;
  cout << "Computer: Hello" << endl;
  while (true) {
    cout << "You: ";
    getline (cin, text);
    if (text == "Bye") 
      return;
    cout << "Computer: " << computer.likely_response (text) << endl;
  }
}
