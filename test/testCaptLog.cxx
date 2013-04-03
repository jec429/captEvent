#include <iostream>
#include "TCaptLog.hxx"
#include "HEPUnits.hxx"

// Test CP::TCaptLog and friends.
int main (int argc, char **argv) {
    CP::TCaptLog::Configure();

    CaptError("An Error Message " << "with a streamed integer " << 42);
    CaptLog("A Log Message " << "with a streamed integer " << 42);
    
    ////////////////////////////////////////////////////////
    // Test the debugging levels.
    ////////////////////////////////////////////////////////

    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::SilentLevel);
    CaptError("An error message that should be printed");

    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::ErrorLevel);
    CaptError("An error message that should be printed");
    CaptSevere("A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::SevereLevel);
    CaptSevere("A severe message that should be printed");
    CaptWarn("A WARN MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::WarnLevel);
    CaptWarn("A warn message that should be printed");
    CaptDebug("A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::DebugLevel);
    CaptDebug("A debug message that should be printed");
    CaptTrace("A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");
    
    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::TraceLevel);
    CaptTrace("A trace message that should be printed");

    ////////////////////////////////////////////////////////
    // Test the named debugging levels.
    ////////////////////////////////////////////////////////

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::SilentLevel);
    CaptNamedError("test","AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::ErrorLevel);
    CaptNamedError("test","An error message that should be printed");
    CaptNamedSevere("test","A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::SevereLevel);
    CaptNamedSevere("test","A severe message that should be printed");
    CaptNamedWarn("test","A WARN MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::WarnLevel);
    CaptNamedWarn("test","A warn message that should be printed");
    CaptNamedDebug("test","A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::DebugLevel);
    CaptNamedDebug("test","A debug message that should be printed");
    CaptNamedTrace("test","A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");
    
    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::TraceLevel);
    CaptNamedTrace("test","A trace message that should be printed");

    /////////////////////////////////////////////////////////
    // Test the logging levels
    /////////////////////////////////////////////////////////

    CP::TCaptLog::SetLogLevel(CP::TCaptLog::QuietLevel);
    CaptLog("A LOG MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel(CP::TCaptLog::LogLevel);
    CaptLog("A log message that should be printed");
    CaptInfo("AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel(CP::TCaptLog::InfoLevel);
    CaptInfo("An info message that should be printed");
    CaptVerbose("A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel(CP::TCaptLog::VerboseLevel);
    CaptVerbose("A verbose message that should be printed");

    /////////////////////////////////////////////////////////
    // Test the named logging levels
    /////////////////////////////////////////////////////////

    CP::TCaptLog::SetLogLevel("test",CP::TCaptLog::QuietLevel);
    CaptNamedLog("test","A LOG MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel("test",CP::TCaptLog::LogLevel);
    CaptNamedLog("test","A log message that should be printed");
    CaptNamedInfo("test","AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel("test",CP::TCaptLog::InfoLevel);
    CaptNamedInfo("test","An info message that should be printed");
    CaptNamedVerbose("test","A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel("test",CP::TCaptLog::VerboseLevel);
    CaptNamedVerbose("test","A verbose message that should be printed");
    
    CaptNamedLog("test","A kilogram is " << unit::kg 
                   << " MeV s^2 m^-2 e_si^-1");

    //////////////////////////////////////////////////////////
    // Check the conditional compilation

#ifdef CAPT_ERROR_OUTPUT 
#undef CAPT_ERROR_OUTPUT 
#endif
#define CAPT_ERROR_OUTPUT false
    CP::TCaptLog::SetDebugLevel(CP::TCaptLog::TraceLevel);
    CaptError("AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptSevere("A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptWarn("A WARN MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptDebug("A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptTrace("A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetDebugLevel("test",CP::TCaptLog::TraceLevel);
    CaptNamedError("test","AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedSevere("test","A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedWarn("test","A WARN MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedDebug("test","A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedTrace("test","A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");

    /////////////////////////////////////////////////////////
    // Test the logging levels
    /////////////////////////////////////////////////////////

#ifdef CAPT_LOG_OUTPUT 
#undef CAPT_LOG_OUTPUT 
#endif
#define CAPT_LOG_OUTPUT false

    CP::TCaptLog::SetLogLevel(CP::TCaptLog::VerboseLevel);
    CaptLog("A LOG MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptInfo("AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptVerbose("A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    CP::TCaptLog::SetLogLevel("test",CP::TCaptLog::VerboseLevel);
    CaptNamedLog("test","A LOG MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedInfo("test","AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");
    CaptNamedVerbose("test","A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    return 0;
}
