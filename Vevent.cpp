//
// Created by hukq1 on 2018/12/17.
//

#include "Vevent.h"

//void Vevent::clear() {
//    summary = "";
//    location = "";
//    description = "";
//    rulereeq = "";
//    uid = "";
//    transp = "";
//    status = "";
//    rulecount = 0;
//    sequence = 0;
//    dstart.SetTime(0,0,0,0,0,0);
//    dend.SetTime(0,0,0,0,0,0);
//}
//
//std::istream &operator >> (std::istream &in, Vevent &obj) {
//    char *cbuffer = new char[512];
//    std::string stemp;
//    std::string sbuffer;
//    in >> sbuffer;
//    std::stringstream sstream;
//    if(sbuffer == "BEGIN:VEVENT")
//    {
//        in >> sbuffer;
//        obj.summary = sbuffer.substr(sbuffer.find(':')+1);
////            std::cout << temp.summary << std::endl;
//        in >> sbuffer;
//        obj.location = sbuffer.substr(sbuffer.find(':')+1);
////            std::cout << temp.location << std::endl;
//        in >> sbuffer;
//        obj.description = sbuffer.substr(sbuffer.find(':')+1);
////            std::cout << temp.description << std::endl;
//        in >> sbuffer;
//        strcpy(cbuffer,sbuffer.substr(sbuffer.find(':')+1).c_str());
//        obj.dstart.SetTime(cbuffer);
//        in >> sbuffer;
//
//        strcpy(cbuffer,sbuffer.substr(sbuffer.find(':')+1).c_str());
//        obj.dend.SetTime(cbuffer);
//        in >> sbuffer;
//        obj.rulereeq = sbuffer.substr(sbuffer.find('=')+1,sbuffer.find_first_of(';'));
//        sstream << sbuffer.substr(sbuffer.find_last_of('=')+1);
//        sstream >> obj.rulecount;
//        in >> sbuffer;
//        sstream << sbuffer.substr(sbuffer.find(':')+1);
//        sstream >> obj.sequence;
//        in >> sbuffer;
//        obj.uid = sbuffer.substr(sbuffer.find(':')+1);
//        in >> sbuffer;
//        obj.transp = sbuffer.substr(sbuffer.find(':')+1);
//        in >> sbuffer;
//        obj.status = sbuffer.substr(sbuffer.find(':')+1);
//        in >> sbuffer;
//    }
//}
