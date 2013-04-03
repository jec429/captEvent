#include <vector>
#include <sstream>
#include <fstream>
#include <time.h>

#include "TCaptLog.hxx"

CP::TCaptLog::ErrorPriority CP::TCaptLog::fErrorPriority = CP::TCaptLog::ErrorLevel;
CP::TCaptLog::LogPriority CP::TCaptLog::fLogPriority = CP::TCaptLog::LogLevel;
std::ostream* CP::TCaptLog::fDebugStream = NULL;
std::ostream* CP::TCaptLog::fLogStream = NULL;
std::map<std::string,CP::TCaptLog::ErrorPriority> CP::TCaptLog::fErrorTraces;
std::map<std::string,CP::TCaptLog::LogPriority> CP::TCaptLog::fLogTraces;
int CP::TCaptLog::fIndentation = 0;

CP::TCaptLog::TCaptLog() { }
CP::TCaptLog::~TCaptLog() { }

void CP::TCaptLog::SetDebugLevel(const char* trace, 
                              CP::TCaptLog::ErrorPriority level) {
    fErrorTraces[trace] = level;
}

CP::TCaptLog::ErrorPriority CP::TCaptLog::GetDebugLevel(const char* trace) {
    std::map<std::string,ErrorPriority>::iterator elem = fErrorTraces.find(trace);
    if (elem == fErrorTraces.end()) return fErrorPriority;
    return elem->second;
}

namespace {
    std::string MakeTimeStamp() {
        std::string stamp = "Unknown Time";
        time_t t = time(NULL);
        struct tm *local = localtime(&t);
        if (!local) return stamp;
        char localTime[80];
        if (!strftime(localTime,sizeof(localTime),"%c",local)) return stamp;
        struct tm *utc = gmtime(&t);
        if (!utc) return stamp;
        char utcTime[80];
        if (!strftime(utcTime,sizeof(utcTime),"%Y-%m-%d %H:%M (UTC)",utc)) 
            return stamp;
        stamp = localTime;
        stamp += " [";
        stamp += utcTime;
        stamp += "]";
        return stamp;
    }
}

void CP::TCaptLog::SetDebugStream(std::ostream* err) {
    CP::TCaptLog::fDebugStream = err;
    if (!fDebugStream) return;
    std::ofstream* ofile = dynamic_cast<std::ofstream*>(err);
    if (ofile && !(ofile->is_open())) {
        fDebugStream = NULL;
        CaptSevere("Debug stream is not open.");
    }
    *fDebugStream << std::endl
                  << "##################################################" 
                  << std::endl
                  << "# ERROR LOG STARTS AT: " << MakeTimeStamp()
                  << std::endl
                  << "##################################################" 
                  << std::endl
                  << std::endl;
}

std::ostream& CP::TCaptLog::GetDebugStream() {
    if (!CP::TCaptLog::fDebugStream ) return GetLogStream();
    return *CP::TCaptLog::fDebugStream;
}
    
void CP::TCaptLog::SetLogLevel(const char* trace, 
                            CP::TCaptLog::LogPriority level) {
    fLogTraces[trace] = level;
}

CP::TCaptLog::LogPriority CP::TCaptLog::GetLogLevel(const char* trace) {
    std::map<std::string,LogPriority>::iterator elem = fLogTraces.find(trace);
    if (elem == fLogTraces.end()) return fLogPriority;
    return elem->second;
}

void CP::TCaptLog::SetLogStream(std::ostream* log) {
    CP::TCaptLog::fLogStream = log;
    if (!fLogStream) return;
    std::ofstream* ofile = dynamic_cast<std::ofstream*>(log);
    if (ofile && !(ofile->is_open())) {
        fLogStream = NULL;
        CaptSevere("Log stream is not open.");
    }
    *fLogStream << std::endl
                << "##################################################" 
                << std::endl
                << "# LOG STARTS AT: " << MakeTimeStamp()
                << std::endl
                << "##################################################" 
                << std::endl
                << std::endl;
}

std::ostream& CP::TCaptLog::GetLogStream() {
    if (!CP::TCaptLog::fLogStream) return std::cout;
    return *CP::TCaptLog::fLogStream;
}

void CP::TCaptLog::SetIndentation(int i) {
    CP::TCaptLog::fIndentation = std::max(i,0);
}

void CP::TCaptLog::IncreaseIndentation() {
    ++CP::TCaptLog::fIndentation;
}

void CP::TCaptLog::DecreaseIndentation() {
    if (CP::TCaptLog::fIndentation>0) --CP::TCaptLog::fIndentation;
}
    
void CP::TCaptLog::ResetIndentation() {
    CP::TCaptLog::fIndentation = 0;
}


std::string CP::TCaptLog::MakeIndent() {
    if (fIndentation<1) return "";
    std::string indent = "";
    for (int i=0; i<fIndentation; ++i) {
        indent += "..";
    }
    indent += " ";
    return indent;
}

namespace {
    bool TranslateLogLevel(const std::string& name, 
                           CP::TCaptLog::LogPriority& level) {
        if (name == "QuietLevel") {
            level = CP::TCaptLog::QuietLevel;
            return true;
        }
        if (name == "LogLevel") {
            level = CP::TCaptLog::LogLevel;
            return true;
        }
        if (name == "InfoLevel") {
            level = CP::TCaptLog::InfoLevel;
            return true;
        }
        if (name == "VerboseLevel") {
            level = CP::TCaptLog::VerboseLevel;
            return true;
        }
        return false;
    }

    bool TranslateErrorLevel(const std::string& name, 
                             CP::TCaptLog::ErrorPriority& level) {
        if (name == "SilentLevel") {
            level = CP::TCaptLog::SilentLevel;
            return true;
        }
        if (name == "ErrorLevel") {
            level = CP::TCaptLog::ErrorLevel;
            return true;
        }
        if (name == "SevereLevel") {
            level = CP::TCaptLog::SevereLevel;
            return true;
        }
        if (name == "WarnLevel") {
            level = CP::TCaptLog::WarnLevel;
            return true;
        }
        if (name == "DebugLevel") {
            level = CP::TCaptLog::DebugLevel;
            return true;
        }
        if (name == "TraceLevel") {
            level = CP::TCaptLog::TraceLevel;
            return true;
        }
        return false;
    }

    std::ostream* StreamPointer(const std::string& name) {
        if (name == "STDCOUT") return &std::cout;
        if (name == "STDCERR") return &std::cerr;
        if (name[0] != '"') return NULL;
        if (name[name.size()-1] != '"') return NULL;
        std::string file = name.substr(1,name.size()-2);
        std::ofstream* output = new std::ofstream(file.c_str(),
                                                  std::ios::out|std::ios::app);
        if (output->is_open()) return output;
        return NULL;
    }

    bool ReadConfigurationFile(const char* config) {
        std::ifstream input(config);
        if (!input.is_open()) return false;

        int inputLine = 0;
        for (;;) {
            std::string line;
            std::getline(input,line);
            if (input.eof()) break;

            // Save the current line number and cache the value so error
            // messages can be printed later.
            std::string cache(line);
            ++inputLine;
            
            // Strip the comments out of the file.
            std::string::size_type position = line.find("#");
            if (position != std::string::npos) line.erase(position);

            // Strip the white space at the beginning of the line.
            line.erase(0,line.find_first_not_of("\t "));

            // Skip lines that are too short.
            if (line.size()==0) continue;

            // Split the line into fields and a value.
            position = line.find("=");
            if (position == std::string::npos) {
                // Houston, we have a problem... There isn't a value.
                std::cerr << "WARNING: " << config << ":" << inputLine << ": "
                          << "Configuration line missing an '='"
                          << std::endl;
                std::cerr << "  Line: <" << cache << ">"
                          << std::endl;
                std::cerr << "  Configuration line has been skip" << std::endl;
                continue;
            }

            // Split the value off the end of the line.
            std::string value = line.substr(position+1);
            line.erase(position);

            // Strip the white space at the beginning of the value.
            value.erase(0,value.find_first_not_of("\t "));

            // Strip the white space at the end of the value.
            position = value.find_last_not_of("\t ");
            if (position != std::string::npos) value.erase(position+1);
            
            // Strip the white space at the end of the fields.
            position = line.find_last_not_of("\t ");
            if (position != std::string::npos) line.erase(position+1);
            
            // Split the remaining line in to fields.
            std::vector<std::string> fields;
            for (;;) {
                position = line.find(".");
                if (position == std::string::npos) {
                    fields.push_back(line);
                    break;
                }
                fields.push_back(line.substr(0,position));
                line.erase(0,position+1);
            } 
            
            // Process the fields and value.
            if (fields.size() == 2
                && fields[0] == "log"
                && fields[1] == "file") {
                // Set the log file name.
                std::ostream* str = StreamPointer(value);
                if (!str) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Cannot open log stream."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetLogStream(str);
            }
            else if (fields.size() == 2
                     && fields[0] == "error"
                     && fields[1] == "file") {
                // Set the error file name.
                std::ostream* str = StreamPointer(value);
                if (!str) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Cannot open error stream."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetDebugStream(str);
            }
            else if (fields.size() == 3
                     && fields[0] == "log"
                     && fields[1] == "default"
                     && fields[2] == "level") {
                // Set the default log level.
                CP::TCaptLog::LogPriority level;
                if (!TranslateLogLevel(value,level)) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Unknown log level name."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetLogLevel(level);
            }
            else if (fields.size() == 3
                     && fields[0] == "error"
                     && fields[1] == "default"
                     && fields[2] == "level") {
                // Set the default error level.
                CP::TCaptLog::ErrorPriority level;
                if (!TranslateErrorLevel(value,level)) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Unknown error level name."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetDebugLevel(level);
            }
            else if (fields.size() == 3
                     && fields[0] == "log"
                     && fields[2] == "level") {
                // Set the log level.
                CP::TCaptLog::LogPriority level;
                if (!TranslateLogLevel(value,level)) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Unknown log level name."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetLogLevel(fields[1].c_str(),level);
            }
            else if (fields.size() == 3
                     && fields[0] == "error"
                     && fields[2] == "level") {
                // Set the error level.
                CP::TCaptLog::ErrorPriority level;
                if (!TranslateErrorLevel(value,level)) {
                    std::cerr << "WARNING: " << config << ":" 
                              << inputLine << ": "
                              << "Unknown error level name."
                              << std::endl;
                    std::cerr << "  Line: <" << cache << ">"
                              << std::endl;
                    std::cerr << "  Configuration line has been skip" 
                              << std::endl;
                    continue;
                }
                CP::TCaptLog::SetDebugLevel(fields[1].c_str(),level);
            }
            else {
                std::cerr << "WARNING: " << config << ":" << inputLine << ": "
                          << "Unknown command."
                          << std::endl;
                std::cerr << "  Line: <" << cache << ">"
                          << std::endl;
                std::cerr << "  Configuration line has been skip" << std::endl;
            }                
        }

        return true;
    }
}

void CP::TCaptLog::Configure(const char* conf) {
    // Try to read a local configuration file.  
    ReadConfigurationFile("./captainlog.config");
    if (conf) {
        bool success = ReadConfigurationFile(conf);
        if (!success) CaptLog("CaptLog configuration file was not read.");
    }
}
