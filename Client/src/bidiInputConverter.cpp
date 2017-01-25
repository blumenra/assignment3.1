#include "../include/bidiInputConverter.h"
#include <stdio.h>
#include <string.h>
#include <sstream>	

//1
BidiMessage BidiInputConverter::convertInput(string input) {



	BidiMessage completeMessage;

	if(!splitToCommandAndArg(input))
		completeMessage = BidiMessage();

	if(command == "LOGRQ") {

        if(arg == "") {

            completeMessage = BidiMessage();
        }
        else {

            completeMessage = BidiMessage::createLoginMessage(arg);
        }
	}
	else if(command == "DELRQ") {

		completeMessage = BidiMessage::createDeleteMessage(arg);
	}
	else if(command == "RRQ") {

		completeMessage = BidiMessage::createRRQMessage(arg);
	}
	else if(command == "WRQ") {

		completeMessage = BidiMessage::createWRQMessage(arg);
	}
	else if(command == "DIRQ") {

		completeMessage = BidiMessage::createDirMessage();
	}
	else if(command == "DISC") {

		completeMessage = BidiMessage::createDiscMessage();
	}
	else
		completeMessage = BidiMessage();

	return completeMessage;

}

bool BidiInputConverter::splitToCommandAndArg(string input) {

	vector<string> inputVec = split(input, ' ');

	if((inputVec.size() == 0) || (inputVec.size() > 2)){

        return false;
    }

	this->command = inputVec[0];

	if(inputVec.size() == 2)
		this->arg = inputVec[1];

	return true;
}

vector<string> BidiInputConverter::split(const string &s, char delim) {

	stringstream ss(s);
    string item;

    vector<string> tokens;

    while (getline(ss, item, delim)) {

		tokens.push_back(item);
    }
    return tokens;
}
