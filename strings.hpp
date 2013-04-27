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
// with LNZ, located in the file GPL.txt.  If not, see                        //
// <http://www.gnu.org/licenses/>.                                            //
//                                                                            //
// Alternative licensing terms may be available for a fee, contact Jon DuBois //
// (bcj1980@gmail.com) for details.                                           //
////////////////////////////////////////////////////////////////////////////////

// All user facing strings go here, collected in one place to ease in e.g. 
// translation or spell correction. This file is meant to be directly included
// in lnz.hpp

#ifndef LNZ_STRINGS_HPP
#define LNZ_STRINGS_HPP

struct strings{
  static constexpr const char* expressionFlag = "-e";
  static constexpr const char* outFileFlag = "-o";
  static constexpr const char* helpFlag = "-h";
  static constexpr const char* expressionFlagError = 
    "Error: expected an expression to follow -e.";
  static constexpr const char* outFileFlagError = 
    "Error: expected a filename to follow -o.";
  static constexpr const char* commandLineExpression = 
    "command line expression";
  static constexpr const char* usageMessage = 
    "Each argument must be in one of the following forms:\n"
    "  <filename>      loads file and parses it.\n"
    "  -o <filename>   writes output to file, usable more than once.\n"
    "  -e <expression> evaluates an expression, usable more than once.\n"
    "  -h              display this help.\n"
    "                  If no expressions or input files are specified, or -\n"
    "                  is specified as an input file, the program reads from\n"
    "                  standard input until EOF (ctrl-d). If no output files\n"
    "                  are specified or - is specified as an output file, the\n"
    "                  program will output to standard output.\n";
  static constexpr const char* fileOpenError = "Failed to open file: ";
  static constexpr const char* fileReadError = "Failed to read file: ";
  static constexpr const char* fileWriteError = 
    "Failed to write file (are you out of disk space?): ";
  static constexpr const char* fileExistsError = 
    "A file operation was attempted on file that already exists: ";
  static constexpr const char* memoryError = 
    "Memory error! You are probably out of RAM!";
  static constexpr const char* emptyFileName = 
    "Error: a file operation was attempted with an empty filename.";
  static constexpr const char* standardInput = "standard input";
  static constexpr const char* standardOutput = "standard output";
  static constexpr const char* yesOrNoPrompt = "y/n?: ";
  static constexpr const char yesChar = 'y';
  static constexpr const char noChar = 'n';
};
 
#endif // LNZ_STRINGS_HPP

