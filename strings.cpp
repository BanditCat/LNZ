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

// All user facing strings go here, collected in one place to ease in e.g. 
// translation or spell correction.

#include <unordered_map>
#include <sstream>
#include <cctype>
#include <stdexcept>

#include "lnz.hpp"

using namespace std;

const char* Strings::memoryError = "Memory error! You are probably out of RAM!";
const char* Strings::fatalOSError = "Fatal OS error! Irrecoverable!";

const unordered_map< string, string > Strings::strings = {
  { "failed!", "%1 failed!" },
  { "osTestPassed", "All %1 tests passed. %2 bogomips at %3%% realtime." },
  { "nTestsPassed", "All %1 tests passed." },
  { "unknownName", "<unknown name>" },
  { "nullTest", "Failure: test does not exist!" },
  { "expressionFlag", "-e" },
  { "outFileFlag", "-o" },
  { "helpFlag", "-h" },
  { "testFlag", "-t" },
  { "expressionFlagError", 
    "Error: expected an expression to follow -e." },
  { "outFileFlagError", 
    "Error: expected a filename to follow -o." },
  { "commandLineExpression", 
    "<command line expression %1>" },
  // Usage information.
  { "usageMessage", 
    "\nLNZ Copyright (C) 2013 Jon DuBois.\n"
    "This program comes with ABSOLUTELY NO WARRANTY. This is free software, "
    "and you are welcome to redistribute it under certain conditions; See the "
    "file LICENSE for details.\n\n" 

    "Each argument must be in one of the following forms:\n"
    "  <filename>      Loads file and parses it.\n"
    "  -o <filename>   Writes output to file, usable more than once.\n"
    "  -e <expression> Evaluates an expression, usable more than once.\n"
    "  -t              Explicitly run sanity tests.\n"
    "  -h              Display this help.\n\n"
    
    "  If no expressions or input files are specified, the default behavior is "
    "to run tests. If - as given as a file, standard input is read from until "
    "EOF (ctrl-d). If no output files are specified or - is specified as an "
    "output file, the program will output to standard output.\n\n" },

  { "fileOpenError", "Failed to open file: %1" },
  { "fileReadError", "Failed to read file: %1" },
  { "fileWriteError", 
    "Failed to write file (are you out of disk space?): %1" },
  { "fileExistsError", 
    "A file operation was attempted on file that already exists: %1" },
  { "emptyFileName", 
    "Error: a file operation was attempted with an empty filename." },
  { "standardInput", "<standard input>" },
  { "standardOutput", "<standard output>" },
  { "yesOrNoPrompt", "y/n?: " },
  { "yesChar", "y" },
  { "noChar", "n" },
  { "flagError", "Unrecognized flag %1" },
  { "fatalError", "Fatal Error!" },
  { "clipboardQuestion", "Copy to clipboard?" },
  { "commandLineOSError", "argv was null!  ARGV WAS NULL!!!! NUULLLLLLL!" },
#ifdef DEBUG
  { "memoryAllocations", "There were %1 allocations." },
  { "memoryFrees", "There were %1 frees." },
  { "memoryLeaks", "There were %1 memory leaks." },
#endif
  { "testException", 
  "\n\nAn exception occurred while testing with this message:\n%1\n\n" },
  { "testUnknownException", 
  "\n\nAn unknown exception occurred while testing!\n\n" },
  { "testing", "Testing %1... %2" },
};

string Strings::gs( initializer_list< string > args ) noexcept{
  string base;
  ostringstream ans;
  // I know this is ugly, but how the heck do I get an index while being 
  // const and exception safe?
  try{ 
    base = strings.at( *args.begin() );
  }catch( const out_of_range& ){
    base = *args.begin();
  }
  auto b = base.begin();
  auto e = base.end();
  size_t argn = 0;
  while( b != e ){
    if( *b == '%' ){
      ++b;
      if( b != e && !isdigit( *b ) ){
	ans << *b++;
	continue;
      }
      while( b != e && isdigit( *b ) ){
	argn *= 10;
	argn += ( *b++ - '0' );
      }
      if( argn && argn < args.size() ){
	auto argsb = args.begin();
	while( argn-- )
	  ++argsb;
	ans << *argsb;
	argn = 0;
      }	
    }else
      ans << *b++;
  }
    
  return ans.str();
}
string Strings::test( void ){
  int numTests = 0;
  if( gs({ "failed!", "foo" }) == "failed!" )
    return "BORK!BORK!";
  ++numTests;
  if( gs({ "foo", "bar" }) != "foo" )
    return gs({ "failed!", "Strings::gs0" });
  ++numTests;
  if( gs({ "foo%1", "bar" }) != "foobar" )
    return gs({ "failed!", "Strings::gs1" });
  ++numTests;
  if( gs({ "foo%1" }) != "foo" )
    return gs({ "failed!", "Strings::gs2" });
  ++numTests;
  if( gs({ "foo" }) != "foo" )
    return gs({ "failed!", "Strings::gs3" });
  ++numTests;
  if( gs({ "%2foo", "bar" }) != "foo" )
    return gs({ "failed!", "Strings::gs4" });
  ++numTests;
  if( gs({ "%2%5%1%3%4", "A", "B", "C", "D", "E" }) != "BEACD" )
    return gs({ "failed!", "Strings::gs5" });
  ++numTests;
  
  return gs({ "nTestsPassed", asString( numTests ) });
}
