//
// Created by hukq1 on 2018/12/19.
//

#include "VeventRepeat.h"

VeventRepeat::VeventRepeat(char *s) {
    this->repeat = true;
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
                    if (key == "TZID") {
                        this->DTSTART.TZID = valuepos;
                        this->DTSTART.STIME.SetTime(pos);
                    }
                } else if (summary == "DTEND") {
                    if (key == "TZID") {
                        this->DTEND.TZID = valuepos;
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
                } else {
                    *(valuepos++) = '\0';
                    char *valuevaluepos1 = std::strchr(pos, '=');
                    *(valuevaluepos1++) = '\0';
                    char *valuevaluepos2 = std::strchr(valuepos, '=');
                    *(valuevaluepos2++) = '\0';
                    std::string summary = cbuffer;
                    std::string key = pos;
                    std::string value = valuevaluepos1;
                    std::string key2 = valuepos;
                    std::string value2 = valuevaluepos2;
                    if (summary == "RRULE") {
                        if (key == "FREQ") {
                            this->RRULE.FREQ = value;
                        }
                        if (key2 == "COUNT") {
                            std::stringstream temp(value2);
                            temp >> this->RRULE.COUNT;
                        }
                    }
                }
            }
        }
    }
}

VeventRepeat::VeventRepeat() {

}
