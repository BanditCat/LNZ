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
#include "parse.hpp"
#include "os.hpp"

using namespace std;


int main( int argc, char* argv[] ) noexcept{
  int ret = EXIT_SUCCESS;

  try{
    string name = "<unknown name>";
    deque< string > args;
    
    //Parse command line arguments.
    if( argv != nullptr ){
      if( argc && ( *argv != nullptr ) )
        name = *argv;
      for( int i = 1; i < argc; ++i )
        if( argv[ i ] != nullptr )
          args.emplace_back( argv[ i ] );
      bool help = false;
      deque< string > outFiles;
      deque< string > expressions;
      deque< string > expressionNames;
      int expressionCommandCount = 0;
      for( size_t i = 0; i < args.size(); ++i ){
        // Ignore empty arguments.
        if( args[ i ].size() ){
          if( args[ i ] == Strings::getString({ "expressionFlag" }) ){
            if( i + 1 >= args.size() || !args[ i + 1 ].size() ){
              cerr << Strings::getString({ "expressionFlagError" }) << endl;
              help = true;
              ret = EXIT_FAILURE;
              break;
            } else {
              ++i;
              expressions.emplace_back( args[ i ] );
              expressionNames.emplace_back
                ( Strings::getString({ "commandLineExpression", 
		      asString( ++expressionCommandCount ) }) );
            }
          }else if( args[ i ] == Strings::getString({ "outFileFlag" }) ){
            if( i + 1 >= args.size() || !args[ i + 1 ].size() ){
              cerr << Strings::getString({ "outFileFlagError" }) << endl;
              help = true;
              ret = EXIT_FAILURE;
              break;
            } else {
              ++i;
              outFiles.emplace_back( args[ i ] );
            }
          }else if( args[ i ] == Strings::getString({ "helpFlag" }) ||
                    ( args[ i ].size() >= 2 && args[ i ][ 0 ] == '-' ) ){
            if( args[ i ] != Strings::getString({ "helpFlag" }) )
              cerr << Strings::getString({ "flagError", args[ i ] }) << endl;
            help = true;
            break;
          }else{
            // We put the name in expressionNames and an empty string in 
            // expressions.  We load the string later.
            expressionNames.emplace_back( args[ i ] );
            expressions.emplace_back( "" );
          }
        }
      } // Done collecting arguments, now either show help or parse.
      if( help ){
        cout << Strings::getString({ "usageMessage" });
      }else{
        // First check for empty file/expression lists and load files.
        if( !expressions.size() ){
          expressionNames.emplace_back( "-" );
          expressions.emplace_back( "" );
        }
        if( !outFiles.size() )
          outFiles.emplace_back( "-" );
        // Now we read in files.
        try{
          for( size_t i = 0; i < expressions.size(); ++i ){
            if( !expressions[ i ].size() ){
              if( expressionNames[ i ] == "-" )
                expressions[ i ] = OS::getStandardIn();
              else
                expressions[ i ] = OS::getFile( expressionNames[ i ] );
            }
          }
        }catch( const lnzFileException& lfe ){
          cerr << lfe.what() << endl;
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
              OS::putStandardOut( out.str() );
            else
              OS::putFile( outFiles[ i ], out.str() );
          }catch( const lnzFileException& lfe ){
            cerr << lfe.what() << endl;
            ret = EXIT_FAILURE;
          }
        }
      }
    }
  }catch( const exception& e ){
    bool copy = 
      OS::yesOrNo( string( e.what() ) + "\n\n" +
                   Strings::getString({ "clipboardQuestion" }) + "\n",
                   Strings::getString({ "fatalError" }) );
    if( copy )
      OS::setClip( e.what() );
    ret = EXIT_FAILURE;
  }
 
#ifdef DEBUG 
  cerr << "Memory allocations: " << OS::getMallocCount() << endl;
  cerr << "Memory frees: " << OS::getFreeCount() << endl;
  cerr << "Memory leaks: " << OS::getMallocCount() - OS::getFreeCount() << endl;
#endif
  
  return ret;
}


 
