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

// LNZ main header file.
// All global defines in this software are prefixed with LNZ_.

#ifndef LNZ_LNZ_HPP
#define LNZ_LNZ_HPP

#include <string>
#include <sstream>
#include <exception>

// Types with sizes.
using u8 = unsigned char;
using u16 = unsigned short int;
using u32 = unsigned int;
using u64 = unsigned long long int;
using s8 = signed char;
using s16 = signed short int;
using s32 = signed int;
using s64 = signed long long int;
using f32 = float;
using f64 = double;
 
#include <climits>

// Limit checks.
#if CHAR_BIT != 8 
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if INT_MAX != 2147483647 || INT_MIN != -2147483648
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if UINT_MAX != 4294967295 
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if SHRT_MAX != 32767 || SHRT_MIN != -32768 
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if USHRT_MAX != 65535 
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if SCHAR_MAX != 127 || SCHAR_MIN != -128
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if UCHAR_MAX != 255 
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if LLONG_MAX != 9223372036854775807ll
#error Alien build environment, check lnz.h for correct typedefs.
#endif 
#if ULLONG_MAX != 18446744073709551615lu
#error Alien build environment, check lnz.h for correct typedefs.
#endif 


// Types used by LNZ
using word = u64;

// Generic stringifier.
template< class T >
inline std::string asString( const T& s ){
  std::ostringstream ans;
  ans << s;
  return ans.str();
}

// Constants
constexpr size_t fileBufferSize = 1024 * 1024;


// Exceptions.
// lnzExcpetion is the base class and is thrown in circumstances that should
// basically never happen.
class lnzException : public std::exception{
public:
  inline lnzException( const std::string& s ) noexcept : message( s ){}
  inline virtual ~lnzException( void ) noexcept{}
  inline virtual const char* what( void ) const noexcept{ 
    return message.c_str(); 
  }
protected:
  std::string message;
};

class lnzFileException : public lnzException{
public:
  inline lnzFileException( const std::string& s ) noexcept : lnzException( s ){}
  inline virtual ~lnzFileException( void ) noexcept{}
};

// Include strings file.
#include "strings.hpp"


// The function that tests main gets declared here so it can get called in 
// Test::test().
const char* mainTest( void );

#endif //LNZ_LNZ_HPP
