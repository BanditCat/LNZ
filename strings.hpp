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

// Just a real short dummy header with the actual strings in the cpp to help 
// mitigate recompiles.  This file is meant to be directly included in lnz.hpp

#ifndef LNZ_STRINGS_HPP
#define LNZ_STRINGS_HPP

#include <unordered_map>
#include <initializer_list>

class Strings{
public:
  // This takes the first string argument and checks to see if there is a string
  // with that name: if so, it substitutes in the named string.  Afterward, the
  // string has all occurences of %1 replaced with the second argument, %2 
  // replaced by the third etc.  %% becomes % and %n disappears unless n is a 
  // valid reference for the number of arguments supplied.
  // DONT CALL WITH ZERO ARGS!
  static std::string gs( std::initializer_list< std::string > ) noexcept;
  // These are const char* because by the time they are needed strings are not
  // safe.
  static const char* memoryError;
  static const char* fatalOSError;

  static const char* test( void ) noexcept( false );
private:
  static const std::unordered_map< std::string, std::string > strings;
};
 
#endif // LNZ_STRINGS_HPP

