#pragma once
#include <cmath>          // abs()
#include <iostream>       // unitbuf
#include <string>
#include <type_traits>    // is_floating_point, common_type_t

namespace Regression
{
  struct CheckResults
  {
    enum class ReportingPolicy{ FAILED_ONLY, BRIEF, ALL };
    CheckResults( std::ostream & stream = std::clog ) : testResults( stream )
    { testResults << std::unitbuf; } // flush the buffer after each insertion - sometimes having that little bit extra info helps if the program crashes

                                     bool is_true                    ( const std::string & nameOfTest,                     bool      actual );
    template<typename T, typename U> bool is_equal                   ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_not_equal               ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_less_than               ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_less_than_or_equal_to   ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_greater_than            ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_greater_than_or_equal_to( const std::string & nameOfTest, const T & expected, const U & actual );

    template<typename T, typename U >
    constexpr bool equal( T const & lhs,  U const & rhs) noexcept
    {
      if constexpr( std::is_floating_point_v<std::common_type_t<T, U>> ) return std::abs( lhs - rhs ) < EPSILON;
      else                                                               return lhs == rhs;
    }

    unsigned        testCount   = 0;
    unsigned        testsPassed = 0;
    double          EPSILON     = 1e-9;
    ReportingPolicy policy      = ReportingPolicy::BRIEF;
    std::ostream &  testResults;
  };










  inline std::ostream & operator<<( std::ostream & stream, const CheckResults & results )
  {
    return stream << "Summary: "
                  << results.testsPassed << " of " << results.testCount
                  << " (" << results.testsPassed * 100.0 / results.testCount << "%) tests passed";
  }









  inline bool CheckResults::is_true( const std::string & nameOfTest, bool actual )
  {
    return is_equal( nameOfTest, true, actual );
  }









  template<typename T, typename U>
  bool CheckResults::is_equal( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( ! equal(expected, actual) )
    {
      testResults << " *[FAILED] " << nameOfTest << ": the expected value is not equal to the actual value, but should be\n    EXP: {" << expected << "}\n    ACT: {" << actual << "}\n";
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << "  [PASSED] " << nameOfTest;
      if( policy > ReportingPolicy::BRIEF ) testResults << ": as expected, the expected and actual values are equal:  {" << actual << '}';
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  bool CheckResults::is_not_equal( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual) )
    {
      testResults << " *[FAILED] " << nameOfTest << ": the expected value is equal to the actual value, but shouldn't be:  {" << actual << "}\n";
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << "  [PASSED] " << nameOfTest;
      if( policy > ReportingPolicy::BRIEF ) testResults << ": as expected, the expected and actual values are not equal\n    EXP: {" << expected << "}\n    ACT: {" << actual << '}';
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  bool CheckResults::is_less_than( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual)  ||  !(expected < actual) )       // account for "close enough" floating point numbers before check for inequality
    {
      testResults << " *[FAILED] " << nameOfTest << ": the expected value is not less than the actual value, but should be\n    EXP: {" << expected << "}\n    ACT: {" << actual << "}\n";
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << "  [PASSED] " << nameOfTest;
      if( policy > ReportingPolicy::BRIEF ) testResults << ": as expected, the expected value is less than the actual value\n    EXP: {" << expected << "}\n    ACT: {" << actual << '}';
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  bool CheckResults::is_greater_than( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual)  ||  !(actual < expected) )       // account for "close enough" floating point numbers before check for inequality
    {
      testResults << " *[FAILED] " << nameOfTest << ": the expected value is not greater than the actual value, but should be\n    EXP: {" << expected << "}\n    ACT: {" << actual << "}\n";
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << "  [PASSED] " << nameOfTest;
      if( policy > ReportingPolicy::BRIEF ) testResults << ": as expected, the expected value is greater than the actual value\n    EXP: {" << expected << "}\n    ACT: {" << actual << '}';
      testResults << '\n';
    }

    return true;
  }
}    // namespace Regression
