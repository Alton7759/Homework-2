#pragma once

// need to include something from the standard library so _LIBCPP_VERSION gets defined, so don't move
// these #includes under that conditional compile
#include <compare>          // strong_oredering
#include <chrono>           // hh_mm_ss<>, year_month_day
#include <iostream>         // ostream
#include <string>

#include <array>
#include <forward_list>
#include <list>
#include <vector>

// C++20 Transition Workarounds
//    _LIBCPP_VERSION is set if using LLVM's libc++ library
//    __GLIBCXX__     is set if using GNU's libstdc++ library (set to date of release)
//           (__GLIBC__ * 1'000 + __GLIBC_MINOR__)  gives libstdc++ version
//    __GNUC__        is defined if using GCC, but also sometimes when using Clang
//          (__GNUC__ * 1'000'000 + __GNUC_MINOR__ * 1'000 + __GNUC_PATCHLEVEL__) give gcc version

// In version 15 and below _LIBCPP_VERSION is encoded using 5 digits (e.g., 15.0.6 was encoded as 15006)
// Starting with version 16, _LIBCPP_VERSION is encoded with 6 digits (e.g., 16.0.1 was encoded as 160001)
// Why?? Is this a bug?  let's keep an eye on it

// Version:  "23.04.12

#if defined( _LIBCPP_VERSION )
  namespace std
  {
    #if _LIBCPP_VERSION < 16'000
       inline strong_ordering operator<=>( const string & lhs, const string & rhs) noexcept
       {
         int result = lhs.compare( rhs );
         return result == 0  ?  strong_ordering::equivalent
              : result  < 0  ?  strong_ordering::less
              :                 strong_ordering::greater;
       }
    #else
      // Fixed in clang version 16.0.1
      // #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
    #endif




    #if _LIBCPP_VERSION <= 17'000'0
      template <typename T>
      inline strong_ordering compare3way_helper( const T & lhs, const T & rhs )
      {
        auto lhsCurrent = cbegin( lhs ),  rhsCurrent = cbegin( rhs );
        auto lhsEnd     = cend  ( lhs ),  rhsEnd     = cend  ( rhs );

        while( lhsCurrent != lhsEnd  &&  rhsCurrent != rhsEnd )
        {
          auto result = compare_weak_order_fallback( *lhsCurrent++, *rhsCurrent++ );
          if( result < 0 ) return strong_ordering::less;
          if( result > 0 ) return strong_ordering::greater;
        }

        if( lhsCurrent == lhsEnd  &&  rhsCurrent == rhsEnd ) return strong_ordering::equivalent;
        if( lhsCurrent == lhsEnd                           ) return strong_ordering::less;
        else                                                 return strong_ordering::greater;
      }


      template< typename T>
      inline strong_ordering operator<=>( const std::vector<T> & lhs, const std::vector<T> & rhs ) noexcept
      {
        return compare3way_helper( lhs, rhs );
      }


      template<typename T>
      inline strong_ordering operator<=>( const std::list<T> & lhs, const std::list<T> & rhs ) noexcept
      {
        return compare3way_helper( lhs, rhs );
      }

      template<typename T>
      inline strong_ordering operator<=>( const std::forward_list<T> & lhs, const std::forward_list<T> & rhs ) noexcept
      {
        return compare3way_helper( lhs, rhs );
      }

      template<typename T, size_t N, size_t M>
      inline strong_ordering operator<=>( const std::array<T, N> & lhs, const std::array<T, M> & rhs ) noexcept
      {
        return compare3way_helper( lhs, rhs );
      }
    #else
      #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
    #endif


  } // namespace std
#endif // defined( _LIBCPP_VERSION )




#if( ( defined( _LIBCPP_VERSION ) && (_LIBCPP_VERSION                      <   17'000'0 )) || \
     ( defined( __GLIBCXX__     ) && ( __GLIBC__ * 1'000 + __GLIBC_MINOR__ <=   2'036   )) )      // ldd 2.37 and gcc 13.0.1 (run "ldd --version" and "gcc --version" at the command line)
  namespace std::chrono                                                                           // Check lib version and not gcc version, clang may use gnu's libstdc++ library
  {
    template<class Duration>
    inline std::ostream & operator<<( std::ostream & os, const std::chrono::hh_mm_ss<Duration> & t )
    {
      return os << t.hours     ().count() << ':'
                << t.minutes   ().count() << ':'
                << t.seconds   ().count() << "."
                << t.subseconds().count();
    }
  }    // namespace std::chrono
#else
  // gcc: fixed in ldd 2.37, but not ldd 2.35.  Don't know about 2.36
  #ifdef _LIBCPP_VERSION
    #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it" )
  #endif
#endif    // ( _LIBCPP_VERSION || __GLIBCXX__ )




#if( ( defined( _LIBCPP_VERSION ) && (_LIBCPP_VERSION                      <   17'000'0 )) || \
     ( defined( __GLIBCXX__     ) && ( __GLIBC__ * 1'000 + __GLIBC_MINOR__ <=   2'036   )) )      // ldd 2.37 and gcc 13.0.1  (run "ldd --version" and "gcc --version" at the command line)
  namespace std::chrono                                                                           // Check lib version and not gcc version, clang may use gnu's libstdc++ library
  {
    inline std::ostream & operator<<( std::ostream & os, const std::chrono::year_month_day & date )
    {
      if( date.ok() ) os << static_cast<int>( date.year() ) << '-' << static_cast<unsigned>( date.month() ) << '-' << static_cast<unsigned>( date.day() );
      else            os << "\"" << std::string{ reinterpret_cast<const char *>( &date ), sizeof( date ) } << "\" is not a valid date";

      return os;
    }
  }    // namespace std::chrono
#else
  // gcc: fixed in ldd 2.37, but not ldd 2.35.  Don't know about 2.36
  #ifdef _LIBCPP_VERSION
    #pragma message( "A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it" )
  #endif
#endif    // ( _LIBCPP_VERSION || __GLIBCXX__ )
