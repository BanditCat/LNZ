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

// This is the android specific code. Include this in os.cpp.

#include <ctime>

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

u64 OS::time( void ) noexcept{
  struct timespec ts;
  clock_gettime( CLOCK_MONOTONIC, &ts );
  return ts.tv_sec * 1000000000 + ts.tv_nsec;
}
u64 OS::cpuTime( void ) noexcept{
  struct timespec ts;
  clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts );
  return ts.tv_sec * 1000000000 + ts.tv_nsec;
}
u64 OS::timesPerSecond( void ) noexcept{
  struct timespec ts;
  if( clock_getres( CLOCK_MONOTONIC, &ts ) || ts.tv_sec )
    return 1;
  return 1000000000 / ( ts.tv_nsec );
}
u64 OS::cpuTimesPerSecond( void ) noexcept{
  struct timespec ts;
  if( clock_getres( CLOCK_PROCESS_CPUTIME_ID, &ts ) || ts.tv_sec )
    return 1;
  return 1000000000 / ( ts.tv_nsec );
}
