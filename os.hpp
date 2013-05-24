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

// OS header. 

#ifndef LNZ_OS_HPP
#define LNZ_OS_HPP

#include <string>


// Override all these to just die on an out of memory condition.
// This is to fix compatibility with libstdc++ 4.7.2 etc.
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

// This is a singleton class: you can and must only instantiate once.
class OS{
public:
  // The os specific init can throw.
  OS() noexcept( false );
  ~OS() noexcept;
  // Throws a lnzException.
  void die( const std::string& ) throw( lnzException );
  // Sets the contents of the clipboard.  Returns false on failure.  Embedded
  // nulls are not supported.
  bool setClip( const std::string& ) noexcept;
  // Returns the contents of the clipboard or "" on error.
  std::string getClip( void ) noexcept;

  // This gets standard in in one big chunk, buffering by fileBufferSize bytes.
  // Throws a lnzFileException on error (which should never happen).
  std::string getStandardIn( void ) throw( lnzFileException );
  // Loads a file in one chunk, buffering by fileBufferSize bytes.  Throws a 
  // lnzFileExpection on failure.
  std::string getFile( const std::string& ) throw( lnzFileException );
  // Saves a file in one operation, throwing a lnzFileException on error or if 
  // the file already exists.
  void putFile( const std::string& filename, const std::string& data )
    throw( lnzFileException );
  // Writes to OS::gout() in one operation, throwing a lnzFileException on 
  // error.
  void putStandardOut( const std::string& data ) 
    throw( lnzFileException );

  // Timer functions.
  u64 time( void ) noexcept;
  u64 cpuTime( void ) noexcept;
  u64 timesPerSecond( void ) noexcept;
  u64 cpuTimesPerSecond( void ) noexcept;
  u64 timeDifference( u64 start, u64 end ) noexcept;
  
  // These functions must be safe to call even with no OS.  
  static void message( const std::string& msg ) noexcept;
  static bool yesOrNo( const std::string& qstn ) noexcept;

  // These are allocation wrapper functions to enable cheesy malloc counting.
  static void* lnzmalloc( size_t ) noexcept;
  static void lnzfree( void* ) noexcept;
  

  // Accessors.
  static inline OS& gos( void ){ 
    return *theOS;
  }
  static inline std::ostream& gout( void ){ return *out; }
  static inline std::istream& gin( void ){ return *in; }
  static inline bool gexistent( void ){ return theOS != nullptr; }

  static std::string test( void ) noexcept( false );
#ifdef DEBUG
  static inline const size_t& getMallocCount( void ) noexcept{ 
    return mallocCount; 
  }
  static inline const size_t& getFreeCount( void ) noexcept{ 
    return freeCount; 
  }
#endif

private:
  // These are the platform specific initialization and cleanup routines.
  void init( void ) noexcept( false );
  void destroy( void ) noexcept;
  // This is a pointer to the single instance of OS.
  static OS* theOS;
  static std::ostream* out;
  static std::istream* in;
#ifdef DEBUG
  static size_t mallocCount;
  static size_t freeCount;
  static bool mallocCounting;
#endif
  
};
    
  
 
 
#endif //LNZ_OS_HPP
