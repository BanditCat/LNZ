////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Jonathan(Jon) DuBois 2013. This file is part of LNZ.         //
//                                                                            //
// LNZ is free software: you can redistribute it and/or modify it under the   //
// terms of the GNU General Public License as published by the Free Software  //
// Foundation, either version 3 of the License, or (at your option) any later //
// version.                                                                   //
//                                                                            //
// LNZ is distributed in the hope that it will be useful, but WITHOUT ANY     //
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  //
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more     //
// details.                                                                   //
//                                                                            //
// You should have received a copy of the GNU General Public License along    //
// with LNZ, located in the file LICENSE.  If not, see                        //
// <http://www.gnu.org/licenses/>.                                            //
//                                                                            //
// Alternative licensing terms may be available for a fee, contact Jon DuBois //
// (bcj1980@gmail.com) for details.                                           //
////////////////////////////////////////////////////////////////////////////////

// OS Specific interface code.



#include <cstdlib>
#include <iostream>
#include <fstream>


#include "lnz.hpp"
#include "os.hpp"
#include "parse.hpp"

using namespace std;

// Statics
#ifdef DEBUG
size_t OS::mallocCount = 0;
size_t OS::freeCount = 0;
bool OS::mallocCounting = false;
#endif
OS* OS::theOS = nullptr;
ostream* OS::out = nullptr;
ostream* OS::err = nullptr;
istream* OS::in = nullptr;

// NO ALLOCATION!
OS::OS( void ) noexcept{
  if( theOS == nullptr ){
    theOS = this;
    out = &cout;
    in = &cin;
    err = &cerr;
  }else{
    cerr << Strings::fatalOSError << endl;
    terminate();
  }
#ifdef DEBUG
  OS::mallocCounting = true;
#endif
}
OS::~OS( void ) noexcept{
#ifdef DEBUG
  OS::mallocCounting = false;
#endif
}

void* OS::lnzmalloc( size_t sz ) noexcept{
  void* ans = malloc( sz );
  if( ans == nullptr ){
    if( err != nullptr )
      *err << Strings::memoryError << endl;
    terminate();
  }
#ifdef DEBUG
  if( OS::mallocCounting )
    ++OS::mallocCount;
#endif
  return ans;
}

void OS::lnzfree( void* p ) noexcept{ 
#ifdef DEBUG
  if( OS::mallocCounting )
    ++OS::freeCount;
#endif
  free( p ); 
}

#ifdef _GLIBCXX_THROW
void* operator new( size_t sz ) _GLIBCXX_THROW( std::bad_alloc ){ 
  return OS::lnzmalloc( sz ); 
}
void* operator new[]( size_t sz ) _GLIBCXX_THROW( std::bad_alloc ){
  return OS::lnzmalloc( sz ); 
}
#else
void* operator new( size_t sz ) throw( std::bad_alloc ){
  return OS::lnzmalloc( sz ); 
}
void* operator new[]( size_t sz ) throw( std::bad_alloc ){ 
  return OS::lnzmalloc( sz ); 
}
#endif // _GLIBCXX_THROW

void* operator new[]( size_t sz, const nothrow_t& ) noexcept{ 
  return OS::lnzmalloc( sz ); 
}
void operator delete[]( void* p ) noexcept{ 
  OS::lnzfree( p ); 
}
void operator delete[]( void* p, const nothrow_t& ) noexcept{ 
  OS::lnzfree( p ); 
}
void* operator new( size_t sz, const nothrow_t& ) noexcept{ 
  return OS::lnzmalloc( sz ); 
}
void operator delete( void* p ) noexcept{ 
  OS::lnzfree( p ); 
}
void operator delete( void* p, const nothrow_t& ) noexcept{ 
  OS::lnzfree( p ); 
}


////////////////////////////////////////////////////////////////////////////////
// Platform independent section.

void OS::die( const string& msg ) throw( lnzException ){
  throw lnzException( msg );
}

// This theoretically handles embedded '\0's.
string OS::getFile( const string& filename ) throw( lnzFileException ){
  if( !filename.size() )
    throw lnzFileException( Strings::gs({ "emptyFileName" }) );
  ifstream ifs( filename );
  if( ifs.is_open() ){
    ostringstream oss;
    char buf[ fileBufferSize + 1 ];
    while( ifs.good() ){
      ifs.read( buf, fileBufferSize );
      buf[ ifs.gcount() ] = '\0';                            
      oss << string( buf, ifs.gcount() );
    }
    if( ifs.bad() )
      throw lnzFileException( Strings::gs({ "fileReadError", 
	      filename }) );
    else
      return oss.str();
  }else
    throw lnzFileException( Strings::gs({ "fileOpenError", 
	    filename }) );
}
string OS::getStandardIn( void ) throw( lnzFileException ){
  ostringstream oss;
  char buf[ fileBufferSize + 1 ];
  while( OS::gin().good() ){
    OS::gin().read( buf, fileBufferSize );
    buf[ OS::gin().gcount() ] = '\0';                      
    oss << string( buf, OS::gin().gcount() );
  }
  if( OS::gin().bad() )
    throw lnzFileException( Strings::gs({ "fileReadError", 
	    Strings::gs({ "standardInput" }) }) );
  return oss.str();
}
void OS::putFile( const string& filename, const string& data ) 
  throw( lnzFileException ){
  if( !filename.size() )
    throw lnzFileException( Strings::gs({ "emptyFileName" }) );
  if( bool( ifstream( filename ) ) )
    throw lnzFileException( Strings::gs({ "fileExistsError", filename }) );
  ofstream ofs( filename );
  if( ofs.is_open() ){
    ofs.write( data.c_str(), data.size() );
    if( ofs.bad() )
      throw lnzFileException( Strings::gs({ "fileWriteError", filename }) );
  }else
    throw lnzFileException( Strings::gs({ "fileOpenError", filename }) );
}
void OS::putStandardOut( const string& data ) throw( lnzFileException ){
  if( OS::gout().good() ){
    OS::gout().write( data.c_str(), data.size() );
    if( OS::gout().bad() )
      throw lnzFileException( Strings::gs({ "fileWriteError", 
	      Strings::gs({ "standardOutput" }) }) );
  }else
    throw lnzFileException( Strings::gs({ "fileOpenError", 
	    Strings::gs({ "standardOutput" }) }) );
}


////////////////////////////////////////////////////////////////////////////////
// Platform dependent sections.

#ifdef WINDOWS

#include <windows.h>

bool OS::yesOrNo( const string& question, const string& header ) noexcept{
  return MessageBoxA( nullptr, question.c_str(), header.c_str(), 
                      MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO )
    == IDYES;
}

bool OS::setClip( const string& msg ) noexcept{
  void* hnd;
  u8* tp = nullptr;
  if( !OpenClipboard( nullptr ) ){
    return false;
  }
  if( !EmptyClipboard() ){
    CloseClipboard();
    return false;
  }
  if( ( ( hnd = GlobalAlloc( GMEM_MOVEABLE, msg.size() + 2 ) ) == nullptr ) ||
      ( ( tp = (u8*)GlobalLock( hnd ) ) == nullptr ) ){
    if( hnd )
      GlobalFree( hnd );
    CloseClipboard();
    return false;
  }
  
  memcpy( tp, msg.c_str(), msg.size() );
  tp[ msg.size() ] = '\0';
  GlobalUnlock( hnd );
  if( SetClipboardData( CF_TEXT, tp ) == nullptr ){;
    CloseClipboard(); 
    return false;
  }
  CloseClipboard(); 
  return true;
}

#endif
// BUGBUG implement.
#ifdef ANDROID
bool OS::yesOrNo( const string& question, const string& header ) noexcept{
  OS::gout() << "\n\n\t" << header << endl << question << endl <<
    Strings::gs({ "yesOrNoPrompt" });
  char cs[ 2 ] = { 0, 0 };
  cs[ 0 ] = OS::gin().get();
  string c = cs;
  while( c != Strings::gs({ "yesChar" }) && c != Strings::gs({ "noChar" }) ){
    OS::gout() << endl << Strings::gs({ "yesOrNoPrompt" });
    cs[ 0 ] = OS::gin().get();
    c = cs;
  }
  return c == Strings::gs({ "yesChar" });
}
bool OS::setClip( const string& ) noexcept{
  return false;
}
#endif

// This is the central test function that calls all of the unit tests.
void OS::test( void ){
  if( out == nullptr || err == nullptr || in == nullptr || theOS == nullptr ){
    // I know this isn't informative, but it is uniquely identifying.
    cerr << "BORK!BORK!BORK!" << endl;
    terminate();
  }
  try{
    const char* t = nullptr;

    // BUGBUG test OS here. Set t to an error message on error.
    t = "Doesn't exit.";

    *out << Strings::gs({ "testing", "OS", 
	  t == nullptr ? Strings::gs({ "success" }) : t }) << endl;
    t = mainTest();
    *out << Strings::gs({ "testing", "main", 
	  t == nullptr ? Strings::gs({ "success" }) : t }) << endl;
    t = Strings::test();
    *out << Strings::gs({ "testing", "Strings", 
	  t == nullptr ? Strings::gs({ "success" }) : t }) << endl;
    t = Parser::test();
    *out << Strings::gs({ "testing", "Parser", 
	  t == nullptr ? Strings::gs({ "success" }) : t }) << endl;

  }catch( const exception& e ){
    *out << Strings::gs({ "testException", e.what() }) << endl;
  }catch( ... ){
    *out << Strings::gs({ "testUnknownException" }) << endl;
  }
}
