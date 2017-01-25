
#ifndef BIDI_CONVERTER_H
#define BIDI_CONVERTER_H

#include "BidiMessage.h"
#include <string>
#include <iostream>
#include <vector>

//1
using namespace std;

class BidiInputConverter {

private:

    string input = "";
    string command = "";
    string arg = "";

    vector<string> split(const string& s, char delim);
    bool splitToCommandAndArg(string input);

public:

    BidiMessage convertInput(string input);

 };

#endif //CLIENT_BIDIMESSAGE_H