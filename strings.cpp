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

#include "lnz.hpp"

using namespace std;


const unordered_map< string, string > Strings::strings = {
  { "expressionFlag", "-e" },
  { "outFileFlag", "-o" },
  { "helpFlag", "-h" },
  { "expressionFlagError", 
    "Error: expected an expression to follow -e." },
  { "outFileFlagError", 
    "Error: expected a filename to follow -o." },
  { "commandLineExpression", 
    "<command line expression %1>" },
  { "usageMessage", 
    "LNZ Copyright (C) 2013 Jon DuBois.\n"
    "This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n"
    "and you are welcome to redistribute it under certain conditions; See the\n"
    "file LICENSE for details.\n\n" 
    "Each argument must be in one of the following forms:\n"
    "  <filename>      loads file and parses it.\n"
    "  -o <filename>   writes output to file, usable more than once.\n"
    "  -e <expression> evaluates an expression, usable more than once.\n"
    "  -h              display this help.\n"
    "                  If no expressions or input files are specified, or -\n"
    "                  is specified as an input file, the program reads from\n"
    "                  standard input until EOF (ctrl-d). If no output files\n"
    "                  are specified or - is specified as an output file, the\n"
    "                  program will output to standard output.\n" },
  { "fileOpenError", "Failed to open file: %1" },
  { "fileReadError", "Failed to read file: %1" },
  { "fileWriteError", 
    "Failed to write file (are you out of disk space?): %1" },
  { "fileExistsError", 
    "A file operation was attempted on file that already exists: %1" },
  { "memoryError", 
    "Memory error! You are probably out of RAM!" },
  { "emptyFileName", 
    "Error: a file operation was attempted with an empty filename." },
  { "standardInput", "<standard input>" },
  { "standardOutput", "<standard output>" },
  { "yesOrNoPrompt", "y/n?: " },
  { "yesChar", "y" },
  { "noChar", "n" } };

string Strings::getString( initializer_list< string > args ) noexcept{
  string base;
  ostringstream ans;
  // I know this is ugly, but how the heck do we get an index while being 
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
      }	
    }else
      ans << *b++;
  }
    
  return ans.str();
}
