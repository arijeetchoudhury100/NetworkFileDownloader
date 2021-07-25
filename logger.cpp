#include<string>
#include "logger.h"
#include<fstream>

using std::string;
using std::ofstream;

string getCurrentDateTime( string s ){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return string(buf);
};

void logger( string logMsg, string log_type ){
    string filePath = log_type + ".txt";
    string cur_time = getCurrentDateTime("now");
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << cur_time << '\t' << logMsg << '\n';
    ofs.close();
}