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

// Main file.


#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <deque>
#include <fstream>



#include "lnz.hpp"
#include "parser.hpp"
#include "os.hpp"
#include "test.hpp"

using namespace std;

//BUGBUG
#include <time.h>
int main( int argc, char* argv[] ) noexcept{
  OS os;
  int ret = EXIT_SUCCESS;
  try{
    string name = Strings::gs({ "unknownName" });
    deque< string > args;

    // Barf on null argv.
    if( argv == nullptr )
      throw lnzException( Strings::gs({ "nullCommandLineOSError" }) );
    //Parse command line arguments.
    if( argc && ( *argv != nullptr ) && *argv[ 0 ] != '\0' )
      name = *argv;
    for( int i = 1; i < argc; ++i )
      if( argv[ i ] != nullptr )
	args.emplace_back( argv[ i ] );
    bool help = false;
    bool test = false;
    deque< string > outFiles;
    deque< string > expressions;
    deque< string > expressionNames;
    int expressionCommandCount = 0;
    for( size_t i = 0; i < args.size(); ++i ){
      // Ignore empty arguments.
      if( args[ i ].size() ){
	// Parse -e
	if( args[ i ] == Strings::gs({ "expressionFlag" }) ){
	  if( i + 1 >= args.size() || !args[ i + 1 ].size() ){
	    OS::gerr() << Strings::gs({ "expressionFlagError" }) << endl;
	    help = true;
	    ret = EXIT_FAILURE;
	    break;
	  } else {
	    ++i;
	    expressions.emplace_back( args[ i ] );
	    expressionNames.emplace_back
	      ( Strings::gs({ "commandLineExpression", 
		    asString( ++expressionCommandCount ) }) );
	  }
	  // Parse -o
	}else if( args[ i ] == Strings::gs({ "outFileFlag" }) ){
	  if( i + 1 >= args.size() || !args[ i + 1 ].size() ){
	    OS::gerr() << Strings::gs({ "outFileFlagError" }) << endl;
	    help = true;
	    ret = EXIT_FAILURE;
	    break;
	  } else {
	    ++i;
	    outFiles.emplace_back( args[ i ] );
	  }
	  // Parse -t
	}else if( args[ i ] == Strings::gs({ "testFlag" }) ){
	  test = true;
	  break;
	  // Parse -h and unrecognized flags.	  
	}else if( args[ i ] == Strings::gs({ "helpFlag" }) ||
		  ( args[ i ].size() >= 2 && args[ i ][ 0 ] == '-' ) ){
	  if( args[ i ] != Strings::gs({ "helpFlag" }) )
	    OS::gerr() << Strings::gs({ "flagError", args[ i ] }) << endl;
	  help = true;
	  break;
	}else{
	  // Parse filenames.  Put the name in expressionNames and an empty
	  // string in expressions: this causes a load from file later.
	  expressionNames.emplace_back( args[ i ] );
	  expressions.emplace_back( "" );
	}
      }
    } // Done collecting arguments, now either show help or parse.
    if( help ){
      OS::gout() << Strings::gs({ "usageMessage" });
    }else if( test || !expressions.size() ){
      Test::test();
    }else{
      if( !outFiles.size() )
	outFiles.emplace_back( "-" );
      // Now read in files.
      try{
	for( size_t i = 0; i < expressions.size(); ++i ){
	  if( !expressions[ i ].size() ){
	    if( expressionNames[ i ] == "-" )
	      expressions[ i ] = os.getStandardIn();
	    else
	      expressions[ i ] = os.getFile( expressionNames[ i ] );
	  }
	}
      }catch( const lnzFileException& lfe ){
	OS::gerr() << lfe.what() << endl;
	return EXIT_FAILURE;
      }
        
      ostringstream out;
      // BUGBUG do actual parsing here.
      {
	Parser p;
	for( size_t i = 0; i < expressions.size(); ++i )
	  out << expressionNames[ i ] << " : " << 
	    expressions[ i ].size() << endl;
      }

      // Write out results.
      for( size_t i = 0; i < outFiles.size(); ++i ){
	try{
	  if( outFiles[ i ] == "-" )
	    os.putStandardOut( out.str() );
	  else
	    os.putFile( outFiles[ i ], out.str() );
	}catch( const lnzFileException& lfe ){
	  OS::gerr() << lfe.what() << endl;
	  ret = EXIT_FAILURE;
	}
      }
    }
  }catch( const exception& e ){
    bool copy = os.yesOrNo( string( e.what() ) + "\n\n" +
			    Strings::gs({ "clipboardQuestion" }) + "\n",
			    Strings::gs({ "fatalError" }) );
    if( copy )
      os.setClip( e.what() );
    ret = EXIT_FAILURE;
  }
 
#ifdef DEBUG 
  {
    size_t ac = os.getMallocCount();
    size_t fc = os.getFreeCount();
    size_t lc = ac - fc;
    OS::gerr() << Strings::gs({ "memoryAllocations", asString( ac ) }) << endl;
    OS::gerr() << Strings::gs({ "memoryFrees", asString( fc ) }) << endl;
    OS::gerr() << Strings::gs({ "memoryLeaks", asString( lc ) }) << endl;
  }
#endif
  
  return ret;
}


string mainTest( void ){
  return "";
}
