//
// Created by hukq1 on 2018/12/19.
//

#include "VeventNoRepeat.h"

VeventNoRepeat::VeventNoRepeat(char *s) {
    this->repeat = false;
    char cbuffer[1024];
    std::stringstream sstream;
    std::string text = s;
    sstream << text;
    while (!sstream.eof()) {
        sstream.getline(cbuffer, 1024);
        char *pos = std::strchr(cbuffer, ':');
        if (pos) {
            *(pos++) = '\0';
            char *keypos = std::strchr(cbuffer, ';');
            if (keypos) {
                *(keypos++) = '\0';
                char *valuepos = std::strchr(keypos, '=');
                *(valuepos++) = '\0';
                std::string summary = cbuffer;
                std::string key = keypos;
                std::string value = valuepos;
                if (summary == "DTSTART") {
                    if (key == "VALUE") {
                        this->DTSTART.VALUE = valuepos;
                        this->DTSTART.STIME.SetTime(pos);
                    }
                } else if (summary == "DTEND") {
                    if (key == "TZID") {
                        this->DTEND.VALUE = valuepos;
                        this->DTEND.STIME.SetTime(pos);
                    }
                }
            } else {
                char *valuepos = std::strchr(pos, ';');
                if (!valuepos) {
                    std::string key = cbuffer;
                    std::string value = pos;
                    if (key == "SUMMARY") {
                        this->SUMMARY = value;
                    } else if (key == "LOCATION") {
                        this->LOCATION = value;
                    } else if (key == "DESCRIPTION") {
                        this->DESCRIPTION = value;
                    } else if (key == "SEQUENCE") {
                        std::stringstream temp(value);
                        temp >> this->SEQUENCE;
                    } else if (key == "UID") {
                        this->UID = value;
                    } else if (key == "TRANSP") {
                        this->TRANSP = value;
                    } else if (key == "STATUS") {
                        this->STATUS = value;
                    }
                }
            }
        }
    }
}

VeventNoRepeat::VeventNoRepeat() {

}
