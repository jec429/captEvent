#include "ECore.hxx"
#include "TEvent.hxx"
#include "TVInputFile.hxx"

CP::TEvent* CP::TVInputFile::FirstEvent() {throw CP::ECore();}
CP::TEvent* CP::TVInputFile::NextEvent(int skip) {throw CP::ECore();}
CP::TEvent* CP::TVInputFile::PreviousEvent(int skip) {return NULL;}
int CP::TVInputFile::GetPosition() const {throw CP::ECore();}
bool CP::TVInputFile::IsOpen() {throw CP::ECore();}
bool CP::TVInputFile::EndOfFile() {throw CP::ECore();}
void CP::TVInputFile::CloseFile() {throw CP::ECore();}
const char* CP::TVInputFile::GetFilename() const {return "";}
