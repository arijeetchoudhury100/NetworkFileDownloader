#ifndef LOGGER_H
#define LOGGER_H

#include<string>
using std::string;

string getCurrentDateTime(string);
void logger(string logmsg, string logtype);

#endif