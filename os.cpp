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

// OS Specific interface code is isolated into this file.



#include <cstdlib>
#include <iostream>
#include <fstream>


#include "lnz.hpp"
#include "os.hpp"
#include "parser.hpp"

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
// This works on the domain of unsigned integers even with overflow. 
u64 OS::timeDifference( u64 start, u64 end ) noexcept{
  return end - start;
}



////////////////////////////////////////////////////////////////////////////////
// Platform dependent sections.

#ifdef WINDOWS
#include "os/windows.hpp"
#endif
#ifdef ANDROID
#include "os/android.hpp"
#endif

string OS::test( void ){
  OS& os = *theOS;
  u32 numTests = 0;

  // die.
  {
    bool suc = false;
    try{
      os.die( "foo" );
    }catch( const lnzException& le ){
      suc = true;
    }
    if( !suc )
      return Strings::gs({ "failed!", "OS::die" });
    ++numTests;
  }
      
  // get/setClip.
  {
    string storeClip = os.getClip();
    os.gout() << storeClip << endl;
    char tst[ 3 ][ 2049 ] = { 0 };
    for( int i = 0; i < 2048; ++i ){
      tst[ 0 ][ i ] = char( i );
      if( !tst[ 0 ][ i ] ) ++tst[ 0 ][ i ];
      tst[ 1 ][ i ] = 190 - char( i );
      if( !tst[ 1 ][ i ] ) ++tst[ 0 ][ i ];
      tst[ 2 ][ i ] = char( i ) + 55;
      if( !tst[ 2 ][ i ] ) ++tst[ 0 ][ i ];
    }
    for( int i = 0; i < 100; ++i ){
      for( int k = 0; k < 3; ++k ){
	string tststr( tst[ k ], 2048 );
	if( !os.setClip( tststr ) )
	  return Strings::gs({ "failed!", "OS::setClip" });
	if( os.getClip() != tststr )
	  return Strings::gs({ "failed!", "OS::getClip" });
      }
    }
    if( !os.setClip( storeClip ) )
      return Strings::gs({ "failed!", "OS::setClip" });
    if( os.getClip() != storeClip )
      return Strings::gs({ "failed!", "OS::getClip" });
    ++numTests;
  }

  // Timers and finish.
  {
    u64 startCpuTime = os.cpuTime();
    u64 startTime = os.time();
    u32 ticks;
    u32 tickbuf[ 1000 ] = { 0 };
    while( os.timeDifference( startCpuTime, os.cpuTime() ) < 
	   os.cpuTimesPerSecond() / 10 ){
      ticks = 0;
      while( ticks < 1000 )
	++tickbuf[ ticks++ ]; 
    }
    double cpuTimed = os.timeDifference( startCpuTime, os.cpuTime() ) / 
      double( os.cpuTimesPerSecond() );
    double timed = os.timeDifference( startTime, os.time() ) / 
      double( os.timesPerSecond() );
    ostringstream ans;
    ans << Strings::gs({ "osTestPassed", asString( numTests ), 
	  asString( tickbuf[ tickbuf[ 0 ] % 1000 ] / ( cpuTimed * 1000.0 ) ),
	  asString( 100.0 * cpuTimed / timed ) });
    return ans.str();
  }
}
