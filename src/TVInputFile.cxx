#include "EoaCore.hxx"
#include "TND280Event.hxx"
#include "TVInputFile.hxx"

CP::TND280Event* CP::TVInputFile::FirstEvent() {throw CP::EoaCore();}
CP::TND280Event* CP::TVInputFile::NextEvent(int skip) {throw CP::EoaCore();}
int CP::TVInputFile::GetPosition() const {throw CP::EoaCore();}
bool CP::TVInputFile::IsOpen() {throw CP::EoaCore();}
bool CP::TVInputFile::EndOfFile() {throw CP::EoaCore();}
void CP::TVInputFile::CloseFile() {throw CP::EoaCore();}
const char* CP::TVInputFile::GetFilename() const {return "";}
