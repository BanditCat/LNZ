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

// Parsing class. 



#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <stack>

#include "lnz.hpp"
#include "os.hpp"
#include "parser.hpp"

using namespace std;


struct Parser::pParser{
  unordered_map< string, word > symbols;
  deque< string > reverseSymbols;
  word refs;

  void insert( const string& name ) noexcept{
    symbols[ name ] = symbols.size();
    reverseSymbols.emplace_back( name );
  }
  pParser* deref( void ) noexcept{
    if( refs != 1 ){
      --refs;
      return new pParser( *this );
    }else
      return this;
  }
   

  pParser( void ) noexcept : symbols(), reverseSymbols(), refs( 0 ){
    OS::gerr() << "    Constructing pParser..." << endl;
  }
 
  pParser( const pParser& c ) noexcept : 
  symbols( c.symbols ), reverseSymbols( c.reverseSymbols ), refs( 0 ){
    OS::gerr() << "    Copy constructing pParser..." << endl;
  }
  ~pParser( void ) noexcept{
    OS::gerr() << "    Destructing pParser..." << endl;
  }
};


Parser::Parser( void ) noexcept{
  OS::gerr() << "  Constructing Parser..." <<endl;
  p = new pParser;
  ++p->refs;
}
Parser::Parser( const Parser& cp ) noexcept{
  OS::gerr() << "  Copy constructing Parser..." <<endl;
  p = cp.p;
  ++p->refs;
}
Parser::~Parser( void ) noexcept{
  if( !--p->refs ) delete p;
  OS::gerr() << "  Destructing Parser..." << endl;
}
void Parser::insert( const string& name ) noexcept{
  p = p->deref();
  p->insert( name );
}
const char* Parser::test( void ){
  return nullptr;
}
