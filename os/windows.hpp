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

// This is the windows specific code. Include this in os.cpp.

#include <windows.h>



u64 OS::time( void ) noexcept{
  LARGE_INTEGER li;
  QueryPerformanceCounter( &li );
  return li.QuadPart;
}
u64 OS::cpuTime( void ) noexcept{
  FILETIME ct, et, kt, ut;
  GetProcessTimes( GetCurrentProcess(), &ct, &et, &kt, &ut );
  return ( u64( ut.dwHighDateTime ) << 32 ) + ut.dwLowDateTime +
    ( u64( kt.dwHighDateTime ) << 32 ) + kt.dwLowDateTime;
}
u64 OS::timesPerSecond( void ) noexcept{
  LARGE_INTEGER li;
  QueryPerformanceFrequency( &li );
  return li.QuadPart;
}
u64 OS::cpuTimesPerSecond( void ) noexcept{
  return 10000000;
}

bool OS::yesOrNo( const string& question, const string& header ) noexcept{
  return MessageBoxA( nullptr, question.c_str(), header.c_str(), 
                      MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO )
    == IDYES;
}

bool OS::setClip( const string& msg ) noexcept{
  void* hnd;
  u8* tp = nullptr;
  if( !OpenClipboard( nullptr ) )
    return false;
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
  if( SetClipboardData( CF_TEXT, tp ) == nullptr ){
    CloseClipboard(); 
    return false;
  }
  CloseClipboard(); 
  return true;
}
string OS::getClip( void ) noexcept{
  if ( !IsClipboardFormatAvailable( CF_TEXT ) ) 
    return ""; 
  if( !OpenClipboard( nullptr ) )
    return "";
  void* hnd = GetClipboardData( CF_TEXT );
  if( hnd == nullptr  ){
    CloseClipboard();
    return "";
  }
  const char* clp = (const char*)( GlobalLock( hnd ) );
  string ans = "";
  if( clp != nullptr && GlobalSize( hnd ) >= 2 ){
    // Not sure why this is weird like this, but it is.
    if( clp[ GlobalSize( hnd ) - 2 ] != '\0' )
      ans = string( clp, GlobalSize( hnd ) - 1 );
    else
      ans = string( clp, GlobalSize( hnd ) - 2 );
    GlobalUnlock( hnd );
  }
  CloseClipboard();
  return ans;
}
