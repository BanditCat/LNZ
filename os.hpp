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

// OS Specific header.

#include <string>


// We override all these to just die on an out of memory condition.
// This is to fix compptibility with libstdc++ 4.7.2 etc.
#ifdef _GLIBCXX_THROW
void* operator new( size_t ) _GLIBCXX_THROW( std::bad_alloc );
void* operator new[]( size_t ) _GLIBCXX_THROW( std::bad_alloc ); 
#else
void* operator new( size_t ) throw( std::bad_alloc );
void* operator new[]( size_t ) throw( std::bad_alloc );
#endif // _GLIBCXX_THROW

void* operator new( size_t, const std::nothrow_t& ) noexcept;
void* operator new[]( size_t, const std::nothrow_t& ) noexcept;
void operator delete( void* ) noexcept;
void operator delete[]( void* ) noexcept;

class OS{
public:
  // Throws a lnzException.
  static void die( const std::string& ) throw( lnzException );
  // This intercativley prompts the user with a yes or no question. 
  static bool yesOrNo( const std::string& question, const std::string& header )
    noexcept;
  // Sets the contents of the clipboard.  Returns false on failure.
  static bool setClip( const std::string& ) noexcept;

  // This gets standard in in one big chunk, buffering by fileBufferSize bytes.
  // Throws a lnzFileException on error (which should never happen).
  static std::string getStandardIn( void ) throw( lnzFileException );
  // Loads a file in one chunk, buffering by fileBufferSize bytes.  Throws a 
  // lnzFileExpection on failure.
  static std::string getFile( const std::string& ) throw( lnzFileException );
  // Saves a file in one operation, throwing a lnzFileException on error or if 
  // the file already exists.
  static void putFile( const std::string& filename, const std::string& data )
    throw( lnzFileException );
  // Writes to cout in one operation, throwing a lnzFileException on error.
  static void putStandardOut( const std::string& data ) 
    throw( lnzFileException );

  // These are allocation wrapper functions to enable cheesy malloc counting.
  static void* lnzmalloc( size_t ) noexcept;
  static void lnzfree( void* ) noexcept;
#ifdef DEBUG
  static inline const size_t& getMallocCount( void ) noexcept{
    return mallocCount; 
  }
  static inline const size_t& getFreeCount( void ) noexcept{ 
    return freeCount; 
  }
#endif
private:
#ifdef DEBUG
  static size_t mallocCount;
  static size_t freeCount;
#endif

  OS() noexcept;
  ~OS() noexcept;
};
    
  
 
 
