#include <iostream>
#include "TND280Log.hxx"
#include "HEPUnits.hxx"

// Test ND::TND280Log and friends.
int main (int argc, char **argv) {
    ND::TND280Log::Configure();

    ND280Error("An Error Message " << "with a streamed integer " << 42);
    ND280Log("A Log Message " << "with a streamed integer " << 42);
    
    ////////////////////////////////////////////////////////
    // Test the debugging levels.
    ////////////////////////////////////////////////////////

    ND::TND280Log::SetDebugLevel(ND::TND280Log::SilentLevel);
    ND280Error("An error message that should be printed");

    ND::TND280Log::SetDebugLevel(ND::TND280Log::ErrorLevel);
    ND280Error("An error message that should be printed");
    ND280Severe("A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel(ND::TND280Log::SevereLevel);
    ND280Severe("A severe message that should be printed");
    ND280Warn("A WARN MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel(ND::TND280Log::WarnLevel);
    ND280Warn("A warn message that should be printed");
    ND280Debug("A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel(ND::TND280Log::DebugLevel);
    ND280Debug("A debug message that should be printed");
    ND280Trace("A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");
    
    ND::TND280Log::SetDebugLevel(ND::TND280Log::TraceLevel);
    ND280Trace("A trace message that should be printed");

    ////////////////////////////////////////////////////////
    // Test the named debugging levels.
    ////////////////////////////////////////////////////////

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::SilentLevel);
    ND280NamedError("test","AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::ErrorLevel);
    ND280NamedError("test","An error message that should be printed");
    ND280NamedSevere("test","A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::SevereLevel);
    ND280NamedSevere("test","A severe message that should be printed");
    ND280NamedWarn("test","A WARN MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::WarnLevel);
    ND280NamedWarn("test","A warn message that should be printed");
    ND280NamedDebug("test","A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::DebugLevel);
    ND280NamedDebug("test","A debug message that should be printed");
    ND280NamedTrace("test","A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");
    
    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::TraceLevel);
    ND280NamedTrace("test","A trace message that should be printed");

    /////////////////////////////////////////////////////////
    // Test the logging levels
    /////////////////////////////////////////////////////////

    ND::TND280Log::SetLogLevel(ND::TND280Log::QuietLevel);
    ND280Log("A LOG MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel(ND::TND280Log::LogLevel);
    ND280Log("A log message that should be printed");
    ND280Info("AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel(ND::TND280Log::InfoLevel);
    ND280Info("An info message that should be printed");
    ND280Verbose("A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel(ND::TND280Log::VerboseLevel);
    ND280Verbose("A verbose message that should be printed");

    /////////////////////////////////////////////////////////
    // Test the named logging levels
    /////////////////////////////////////////////////////////

    ND::TND280Log::SetLogLevel("test",ND::TND280Log::QuietLevel);
    ND280NamedLog("test","A LOG MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel("test",ND::TND280Log::LogLevel);
    ND280NamedLog("test","A log message that should be printed");
    ND280NamedInfo("test","AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel("test",ND::TND280Log::InfoLevel);
    ND280NamedInfo("test","An info message that should be printed");
    ND280NamedVerbose("test","A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel("test",ND::TND280Log::VerboseLevel);
    ND280NamedVerbose("test","A verbose message that should be printed");
    
    ND280NamedLog("test","A kilogram is " << unit::kg 
                   << " MeV s^2 m^-2 e_si^-1");

    //////////////////////////////////////////////////////////
    // Check the conditional compilation

#ifdef ND280_ERROR_OUTPUT 
#undef ND280_ERROR_OUTPUT 
#endif
#define ND280_ERROR_OUTPUT false
    ND::TND280Log::SetDebugLevel(ND::TND280Log::TraceLevel);
    ND280Error("AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Severe("A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Warn("A WARN MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Debug("A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Trace("A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetDebugLevel("test",ND::TND280Log::TraceLevel);
    ND280NamedError("test","AN ERROR MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedSevere("test","A SEVERE MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedWarn("test","A WARN MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedDebug("test","A DEBUG MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedTrace("test","A TRACE MESSAGE THAT SHOULD NOT BE PRINTED");

    /////////////////////////////////////////////////////////
    // Test the logging levels
    /////////////////////////////////////////////////////////

#ifdef ND280_LOG_OUTPUT 
#undef ND280_LOG_OUTPUT 
#endif
#define ND280_LOG_OUTPUT false

    ND::TND280Log::SetLogLevel(ND::TND280Log::VerboseLevel);
    ND280Log("A LOG MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Info("AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280Verbose("A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    ND::TND280Log::SetLogLevel("test",ND::TND280Log::VerboseLevel);
    ND280NamedLog("test","A LOG MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedInfo("test","AN INFO MESSAGE THAT SHOULD NOT BE PRINTED");
    ND280NamedVerbose("test","A VERBOSE MESSAGE THAT SHOULD NOT BE PRINTED");

    return 0;
}
