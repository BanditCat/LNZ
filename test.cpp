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

// Test class. 

#include <iostream>

#include "lnz.hpp"
#include "test.hpp"
#include "os.hpp"
#include "parser.hpp"

using namespace std;



void Test::test( void ) noexcept{
  if( !OS::gexistent() ){
    // I know this isn't informative, but it is uniquely identifying.
    OS::message( "BORK!BORK!BORK!" );
    terminate();
  }
  try{
    string t = "";
    
    t = OS::test();
    OS::gout() << Strings::gs({ "testing", "OS", 
	  t == "" ? Strings::gs({ "nullTest" }) : t }) << endl;
    t = mainTest();
    OS::gout() << Strings::gs({ "testing", "main", 
	  t == "" ? Strings::gs({ "nullTest" }) : t }) << endl;
    t = Strings::test();
    OS::gout() << Strings::gs({ "testing", "Strings", 
	  t == "" ? Strings::gs({ "nullTest" }) : t }) << endl;
    t = Parser::test();
    OS::gout() << Strings::gs({ "testing", "Parser", 
	  t == "" ? Strings::gs({ "nullTest" }) : t }) << endl;

  }catch( const exception& e ){
    OS::gout() << Strings::gs({ "testException", e.what() }) << endl;
  }catch( ... ){
    OS::gout() << Strings::gs({ "testUnknownException" }) << endl;
  }
}
