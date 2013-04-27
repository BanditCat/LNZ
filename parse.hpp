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

// Parsing class header. 

#include <string>
#include <unordered_map>
 
#ifndef LNZ_PARSE_H
#define LNZ_PARSE_H



// Maybe: implement a fast radix-4-trie.
class Parser{
public:
  Parser() noexcept;
  Parser( const Parser& ) noexcept;
  ~Parser() noexcept;

  void insert( const std::string& ) noexcept;
private:
  struct pParser;
  pParser* p;
};


#endif // LNZ_PARSE_H

