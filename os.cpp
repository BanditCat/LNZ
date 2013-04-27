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
// with LNZ, located in the file GPL.txt.  If not, see                        //
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

using namespace std;

// Statics
#ifdef DEBUG
size_t OS::mallocCount = 0;
size_t OS::freeCount = 0;
#endif



void* OS::lnzmalloc( size_t sz ) noexcept{
  void* ans = malloc( sz );
  if( ans == nullptr ){
    cerr << strings::memoryError;
    terminate();
  }
#ifdef DEBUG
  ++OS::mallocCount;
#endif
  return ans;
}

void OS::lnzfree( void* p ) noexcept{ 
#ifdef DEBUG
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
void* operator new( size_t sz, const nothrow_t& ) noexcept{ 
  return OS::lnzmalloc( sz ); 
}
void operator delete( void* p ) noexcept{ 
  OS::lnzfree( p ); 
}


////////////////////////////////////////////////////////////////////////////////
// Platform independant section.

void OS::die( const string& msg ) throw( lnzException ){
  throw lnzException( msg );
}

// This theoretically handles embedded '\0's.
string OS::getFile( const string& filename ) throw( lnzFileException ){
  if( !filename.size() )
    throw lnzFileException( strings::emptyFileName );
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
      throw lnzFileException( strings::fileReadError + filename );
    else
      return oss.str();
  }else
    throw lnzFileException( strings::fileOpenError + filename );
}
string OS::getStandardIn( void ) throw( lnzFileException ){
  ostringstream oss;
  char buf[ fileBufferSize + 1 ];
  while( cin.good() ){
    cin.read( buf, fileBufferSize );
    buf[ cin.gcount() ] = '\0';                      
    oss << string( buf, cin.gcount() );
  }
  if( cin.bad() )
    throw lnzFileException( string( strings::fileReadError ) +
                            "<" + strings::standardInput + ">" );
  return oss.str();
}
void OS::putFile( const string& filename, const string& data ) 
  throw( lnzFileException ){
  if( !filename.size() )
    throw lnzFileException( strings::emptyFileName );
  if( bool( ifstream( filename ) ) )
    throw lnzFileException( strings::fileExistsError + filename );
  ofstream ofs( filename );
  if( ofs.is_open() ){
    ofs.write( data.c_str(), data.size() );
    if( ofs.bad() )
      throw lnzFileException( strings::fileWriteError + filename );
  }else
    throw lnzFileException( strings::fileOpenError + filename );
}
void OS::putStandardOut( const string& data ) throw( lnzFileException ){
  if( cout.good() ){
    cout.write( data.c_str(), data.size() );
    if( cout.bad() )
      throw lnzFileException( string( strings::fileWriteError ) + 
                              "<" + strings::standardOutput + ">" );
  }else
    throw lnzFileException( string( strings::fileOpenError ) + 
                            "<" + strings::standardOutput + ">" );
}


////////////////////////////////////////////////////////////////////////////////
// Platform dependant sections.

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
  cout << "\n\n\t" << header << endl << question << endl <<
    strings::yesOrNoPrompt;
  char c = cin.get();
  while( c != strings::yesChar && c != strings::noChar ){
    cout << endl << strings::yesOrNoPrompt;
    c = cin.get();
  }
  return c == strings::yesChar;
}
bool OS::setClip( const string& ) noexcept{
  return false;
}
#endif