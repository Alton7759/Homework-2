#include <cmath>                                                                            // abs(), ceil(), log10()
#include <exception>
#include <iomanip>                                                                          // setprecision()
#include <iostream>                                                                         // boolalpha(), showpoint(), fixed(), clog, ios, streamsize
#include <sstream>                                                                          // istringstream, stringstream
#include <utility>                                                                          // move()

#include "RegressionTests/CheckResults.hpp"
#include "GroceryItem.hpp"




namespace  // anonymous
{
  constexpr auto EPSILON = 1E-4;

  class GroceryItemRegressionTest
  {
    public:
      GroceryItemRegressionTest();

    private:
      void construction();
      void io();
      void comparison();
      void copyVsMoveSemantics();

      Regression::CheckResults affirm;
  } run_grocery_item_tests;




  void GroceryItemRegressionTest::construction()
  {
    GroceryItem gItem1,
                gItem2( "grocery item's product name"                                                                  ),
                gItem3( "grocery item's product name",  "grocery item's brand name"                                    ),
                gItem4( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code"         ),
                gItem5( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code", 123.79 );

    affirm.is_equal( "Default price                                     ", gItem1.price(), 0.0 );

    gItem1.productName( "1-3-5" );
    affirm.is_equal( "Symmetrical Product Name mutator/query            ", "1-3-5", gItem1.productName() );

    gItem1.brandName( "21-23-25" );
    affirm.is_equal( "Symmetrical Brand Name mutator/query              ", "21-23-25", gItem1.brandName() );

    gItem1.upcCode( "11-13-15" );
    affirm.is_equal( "Symmetrical UPC code mutator/query                ", "11-13-15", gItem1.upcCode() );

    gItem1.price( 31.11 );
    affirm.is_equal( "Symmetrical Price mutator/query                   ", 31.11, gItem1.price() );

    affirm.is_true
    ( "Grocery item construction with arguments          ",
           gItem2.productName() == "grocery item's product name"
        && gItem3.productName() == "grocery item's product name" && gItem3.brandName() == "grocery item's brand name"
        && gItem4.productName() == "grocery item's product name" && gItem4.brandName() == "grocery item's brand name" && gItem4.upcCode() == "grocery item's UPC code"
        && gItem5.productName() == "grocery item's product name" && gItem5.brandName() == "grocery item's brand name" && gItem5.upcCode() == "grocery item's UPC code" && std::abs(gItem5.price() - 123.79) < EPSILON
     );

    GroceryItem gItem6( gItem5 );
    affirm.is_true
    ("Copy construction                                 ",
          gItem6.productName() ==  gItem5.productName()
       && gItem6.brandName()   ==  gItem5.brandName()
       && gItem6.upcCode()     ==  gItem5.upcCode()
       && std::abs(gItem6.price() - gItem5.price()) < EPSILON
    );

    GroceryItem gItem7( std::move(gItem6) );
    affirm.is_equal( "Move construction                                 ", gItem5, gItem7 );  // uses GroceryItem's operator == and operator <<, so if those doesn't work ...

    gItem6.productName( "" ).brandName( "" ).upcCode( "" ).price( 0 );                        // uses chaining, so if that doesn't work ...
    gItem6 = gItem5;
    affirm.is_true
    ("Copy assignment                                  ",
          gItem6.productName() ==  gItem5.productName()
       && gItem6.brandName()   ==  gItem5.brandName()
       && gItem6.upcCode()     ==  gItem5.upcCode()
       && std::abs(gItem6.price() - gItem5.price()) < EPSILON
    );


    gItem7 = std::move( gItem6 );
    affirm.is_equal( "Move assignment                                  ", gItem5, gItem7 );    // uses GroceryItem's operator == and operator <<, so if those doesn't work ...

  }



  void GroceryItemRegressionTest::io()
  {
    {  // Input parsing
      std::istringstream stream( R"~~( "00072250018548","Nature's Own","Nature's Own Butter Buns Hotdog - 8 Ct",56.69

                                        "00028000517205", "Nestle"             ,
                                        "Nestle \"Media Crema\" Table Cream"       ,
                                        118.07

                                        "00034000020706"    ,
                                        "York",
                                        "York Peppermint Patties Dark Chocolate Covered Snack Size",
                                        31.57 "00038000570742",
                                        "Kellogg's", "Kellogg's Cereal Krave Chocolate",
                                          65.65

                                        "00014100072331" , "Pepperidge  \"Home Town\"  Farm", "Pepperidge Farm Classic Cookie Favorites", 26.45

                                        "00000000000000", "incomplete / invalid grocery item"
                                 )~~" );

      GroceryItem t1, t2, t3, t4, t5, t6;
      stream >> t1 >> t2 >> t3 >> t4 >> t5 >> t6;

      affirm.is_equal( "GroceryItem input parsing 1                       ", GroceryItem { "Nature's Own Butter Buns Hotdog - 8 Ct",                     "Nature's Own",                     "00072250018548", 56.69  }, t1 );
      affirm.is_equal( "GroceryItem input parsing 2                       ", GroceryItem { "Nestle \"Media Crema\" Table Cream",                         "Nestle",                           "00028000517205", 118.07 }, t2 );
      affirm.is_equal( "GroceryItem input parsing 3                       ", GroceryItem { "York Peppermint Patties Dark Chocolate Covered Snack Size",  "York",                             "00034000020706", 31.57  }, t3 );
      affirm.is_equal( "GroceryItem input parsing 4                       ", GroceryItem { "Kellogg's Cereal Krave Chocolate",                           "Kellogg's",                        "00038000570742", 65.65  }, t4 );
      affirm.is_equal( "GroceryItem input parsing 5                       ", GroceryItem { "Pepperidge Farm Classic Cookie Favorites",                   "Pepperidge  \"Home Town\"  Farm",  "00014100072331", 26.45  }, t5 );

      affirm.is_equal( "GroceryItem input parsing 6                       ", GroceryItem(), t6 );
    }
    {  // read what you write
      GroceryItem gItem1,
                  gItem2( "grocery item's product name" ),
                  gItem3( "grocery item's product name", "grocery item's brand name" ),
                  gItem4( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code" ),
                  gItem5( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code", 123.79 );

      std::stringstream stream;

      stream << gItem1 << '\n'
             << gItem2 << '\n'
             << gItem3 << '\n'
             << gItem4 << '\n'
             << gItem5;

      GroceryItem t1, t2, t3, t4, t5;
      stream >> t1 >> t2 >> t3 >> t4 >> t5;

      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 1", gItem1, t1 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 2", gItem2, t2 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 3", gItem3, t3 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 4", gItem4, t4 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 5", gItem5, t5 );
    }
  }



  void GroceryItemRegressionTest::comparison()
  {
    GroceryItem less( "a1", "a1", "a1", 10.0 ), more(less);

    // Be careful - using affirm.xxx() may hide the class-under-test overloaded operators.  But affirm.is_true() doesn't provide as
    // much information when the test fails.
    affirm.is_equal    ( "Equality test - is equal                          ", less, more );
    affirm.is_equal    ( "Equality test - floating point lower limit        ", less, GroceryItem {"a1", "a1", "a1", less.price() - EPSILON} );
    affirm.is_equal    ( "Equality test - floating point upper limit        ", less, GroceryItem {"a1", "a1", "a1", less.price() + EPSILON} );

    affirm.is_not_equal( "Inequality Product Name test                      ", less, GroceryItem {"b1", "a1", "a1", 10.0} );
    affirm.is_not_equal( "Inequality Brand Name test                        ", less, GroceryItem {"a1", "b1", "a1", 10.0} );
    affirm.is_not_equal( "Inequality UPC test                               ", less, GroceryItem {"a1", "a1", "b1", 10.0} );
    affirm.is_not_equal( "Inequality Price test - lower limit               ", less, GroceryItem {"a1", "a1", "a1", less.price() - ( 2 * EPSILON )} );
    affirm.is_not_equal( "Inequality Price test - upper limit               ", less, GroceryItem {"a1", "a1", "a1", less.price() + ( 2 * EPSILON )} );


    auto check = [&]()
    {
      // Exercise all relational operators for the class-under-test.
      return less <  more
          && less <= more
          && more >  less
          && more >= less;
    };

    more = {"a1", "a1", "a1", 11.0};
    affirm.is_true( "Relational Price test                             ", check() );

    more = {"a1", "a2", "a1", 9.0};
    affirm.is_true( "Relational Brand Name test                        ", check() );

    more = {"a2", "a0", "a1", 9.0};
    affirm.is_true( "Relational Product Name test                      ", check() );

    more = {"a0", "a0", "a2", 9.0};
    affirm.is_true( "Relational UPC code test                          ", check() );
  }



  void GroceryItemRegressionTest::copyVsMoveSemantics()
  {
    GroceryItem const gItem5( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code", 123.79 );


    { // UPC Code
      GroceryItem gItem( gItem5 );
      gItem.upcCode();                            // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - UPC Code Copy    ", gItem5, gItem );

      std::move( gItem ).upcCode();               // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - UPC Code Move    ", gItem5, gItem );
    }


    { // Brand Name
      GroceryItem gItem( gItem5 );
      gItem.brandName();                          // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - Brand Name Copy  ", gItem5, gItem );

      std::move( gItem ).brandName();             // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - Brand Name Move  ", gItem5, gItem );
    }


    { // Product Name
      GroceryItem gItem( gItem5 );
      gItem.productName();                        // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - Product Name Copy", gItem5, gItem );

      std::move( gItem ).productName();           // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - Product Name Move", gItem5, gItem );
    }
  }



  GroceryItemRegressionTest::GroceryItemRegressionTest()
  {
    // affirm.policy = Regression::CheckResults::ReportingPolicy::ALL;
    struct StreamStateRAII
    {
      StreamStateRAII( std::ios & stream ) : _stream( stream )
      {}

      ~StreamStateRAII()
      {
        std::clog.precision( _precision );
        std::clog.flags    ( _flags     );
      }

      std::ios &         _stream;
      std::ios::fmtflags _flags     = std::clog.flags();
      std::streamsize    _precision = std::clog.precision();
    } stream_state_raii( std::clog );

    std::clog << std::boolalpha << std::showpoint << std::fixed << std::setprecision( 2 );


    try
    {
      std::clog << "\n\n\nGroceryItem Regression Test:  Construction\n";
      construction();

      std::clog << "\nGroceryItem Regression Test:  Relational comparisons\n";
      auto previousPrecision = std::clog.precision( static_cast<std::streamsize>( std::ceil( -std::log10( EPSILON ) ) ) );
      comparison();
      std::clog.precision( previousPrecision );

      std::clog << "\nGroceryItem Regression Test:  Input/Output\n";
      io ();


      std::clog << "\nGroceryItem Regression Test:  Move Semantics\n";
      copyVsMoveSemantics();


      std::clog << "\n\nGroceryItem Regression Test " << affirm << "\n\n";
    }
    catch( const std::exception & ex )
    {
      std::clog << "FAILURE:  Regression test for \"class GroceryItem\" failed with an unhandled exception. \n\n\n"
                << ex.what() << std::endl;
    }
  }
} // namespace
