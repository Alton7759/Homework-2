#include <algorithm>                                                      // move( range ), move_backward( range )
#include <array>
#include <cstddef>                                                        // size_t
#include <exception>
#include <forward_list>
#include <iomanip>                                                        // setprecision()
#include <iostream>                                                       // boolalpha(), showpoint(), fixed(), setw()
#include <list>
#include <sstream>                                                        // ostringstream
#include <string>                                                         // string, to_string()
#include <utility>                                                        // move( object )
#include <vector>

#include "CheckResults.hpp"
#include "GroceryItem.hpp"
#include "GroceryList.hpp"



namespace    // anonymous
{
  class GroceryListRegressionTest
  {
    public:
      GroceryListRegressionTest();

    private:
      void test();
      void deepArrayInterrogationTest();

      Regression::CheckResults affirm;
  } run_grocery_list_tests;




  void GroceryListRegressionTest::test()
  {
    const GroceryItem gItem_1( "gItem_1" ),
                      gItem_2( "gItem_2" ),
                      gItem_3( "gItem_3" ),
                      gItem_4( "gItem_4" ),
                      gItem_5( "gItem_5" ),
                      gItem_6( "gItem_6" );

    {
      GroceryList list;
      affirm.is_equal( "Default construction:  Size", 0U, list.size() );
    }

    {
      GroceryList list1 = { gItem_2, gItem_3, gItem_1, gItem_4 };
      GroceryList list2( list1 );
      affirm.is_equal( "Copy construction and equality", list1, list2 );

      GroceryList list3 = { gItem_3, gItem_1, gItem_4, gItem_5 };
      affirm.is_less_than   ("Relational 1", list1, list3);
      affirm.is_greater_than("Relational 2", list3, list1);
    }

    {
      GroceryList list;
      list.insert( gItem_3                             );
      list.insert( gItem_4, GroceryList::Position::BOTTOM );
      list.insert( gItem_1, 1                          );
      list.insert( gItem_2                             );

      GroceryList expected = {gItem_2, gItem_3, gItem_1, gItem_4};

      affirm.is_equal( "Initializer list constructor:  Size",    4U,       list.size() );
      affirm.is_equal( "Initializer list constructor:  content", expected, list        );
    }

    {
      GroceryList list1 = {gItem_2, gItem_3, gItem_1, gItem_4};
      list1 += {gItem_3, gItem_1, gItem_2, gItem_5};

      GroceryList list2 = {gItem_2, gItem_6, gItem_1, gItem_4};
      list1 += list2;

      GroceryList expected = {gItem_2, gItem_3, gItem_1, gItem_4, gItem_5, gItem_6};

      affirm.is_equal( "Initializer list concatenation:  Size",    6U,       list1.size() );
      affirm.is_equal( "Initializer list concatenation:  content", expected, list1        );

      affirm.is_equal( "Search - gItem_2",    0U, list1.find( gItem_2        ) );
      affirm.is_equal( "Search - gItem_3",    1U, list1.find( gItem_3        ) );
      affirm.is_equal( "Search - gItem_1",    2U, list1.find( gItem_1        ) );
      affirm.is_equal( "Search - gItem_4",    3U, list1.find( gItem_4        ) );
      affirm.is_equal( "Search - gItem_5",    4U, list1.find( gItem_5        ) );
      affirm.is_equal( "Search - gItem_6",    5U, list1.find( gItem_6        ) );
      affirm.is_equal( "Search - not there", 6U, list1.find( {"not there"} ) );
    }

    {
      GroceryList list = { gItem_3, gItem_3, gItem_3 };
      affirm.is_equal( "Silently ignore duplicates - size", list.size(), 1U );
      affirm.is_equal( "Silently ignore duplicates - content", list, GroceryList {gItem_3} );
    }

    {
      GroceryList list = {gItem_2, gItem_1, gItem_4, gItem_5, gItem_6};

      list.remove( gItem_1 );
      affirm.is_equal( "Remove by grocery item - middle", GroceryList {gItem_2, gItem_4, gItem_5, gItem_6}, list );

      list.remove( gItem_6 );
      affirm.is_equal( "Remove by grocery item - bottom", GroceryList {gItem_2, gItem_4, gItem_5}, list );

      list.remove( gItem_2 );
      affirm.is_equal( "Remove by grocery item - top", GroceryList {gItem_4, gItem_5}, list );

      list.remove( {"not there"} );
      affirm.is_equal( "Remove by grocery item - not there", GroceryList {gItem_4, gItem_5}, list );
    }

    {
      GroceryList list = {gItem_2, gItem_1, gItem_4, gItem_5, gItem_6};

      list.remove( 1 );
      affirm.is_equal( "Remove by position - middle", GroceryList {gItem_2, gItem_4, gItem_5, gItem_6}, list );

      list.remove( 3 );
      affirm.is_equal( "Remove by position - bottom", GroceryList {gItem_2, gItem_4, gItem_5}, list );

      list.remove( 0 );
      affirm.is_equal( "Remove by position - top", GroceryList {gItem_4, gItem_5}, list );

      list.remove( 10 );
      affirm.is_equal( "Remove by position - bad index", GroceryList {gItem_4, gItem_5}, list );
    }

    {
      GroceryList list = {gItem_2, gItem_1, gItem_4, gItem_5, gItem_6};

      list.moveToTop( gItem_5        );
      list.moveToTop( gItem_6        );
      list.moveToTop( gItem_6        );
      list.moveToTop( gItem_5        );
      list.moveToTop( gItem_4        );
      list.moveToTop( {"not there"} );

      GroceryList expected = {gItem_4, gItem_5, gItem_6, gItem_2, gItem_1};
      affirm.is_equal( "Move to top", expected, list );
    }

    {
      GroceryList list;

      try
      {
        for( unsigned i = 0; i < 100; ++i ) list.insert( GroceryItem{ "GroceryItem-" + std::to_string( i ) } );
        affirm.is_true( "Fixed size array capacity check", false );
      }
      catch ( const GroceryList::CapacityExceeded_Ex & )  // expected
      {
        affirm.is_true( "Fixed size array capacity check", true );
      }
    }
  }  // GroceryListRegressionTest::test()




  void GroceryListRegressionTest::deepArrayInterrogationTest()
  {
    // Inserting and removing from the array attribute is particularly error prone.  Let's dig deeper into this looking for
    // hard-to-spot logic errors.  The attributes of the GroceryList are private, so I can't get to them in the usual way.  If
    // you're reading this, don't do what I'm about to do - this is strictly for testing, and then only in a very controlled
    // environment. If the order of attributes in GroceryList, or if other attributes are added - I'm screwed.  Not to mention I'm
    // depending on GroceryList.hpp including what I need here.  By creating a struct that mirrors the attribute layout of the
    // GroceryList I ensure proper attribute alignment and offset while gaining visibility.
    struct Attributes                                                                         // must exactly match the type and order of GroceryList's instance attributes
    {
      std::array       <GroceryItem, 11>    _gList_array;
      std::vector      <GroceryItem    >    _gList_vector;
      std::list        <GroceryItem    >    _gList_dll;
      std::forward_list<GroceryItem    >    _gList_sll;
      std::size_t                           _gList_array_size = 0;
    };



    // Convert an array to string.  The STL does not define an insertion operator for std::array, which is needed if an std::array
    // is passed to Regression::CheckResults::is_equal.  So instead of passing standard arrays, let's convert the array to a string
    // and pass the string to Regression::CheckResults::is_equal.  Perhaps a better approach would be to create insertion operator
    // templates in Regression::CheckResults, but this gets the job done with less ripple effect, for now ...
    auto to_string = []( const auto & container ) -> std::string
    {
      std::ostringstream s;
      s << '\n';

      for( std::size_t index = 0; index < container.size(); ++index ) s << std::setw( 3 ) << index << ":  " << container[index] << '\n';
      return s.str();
    };



    // Create a grocery list and access its private instance attributes, something we should never do outside of a controlled
    // white-box test like this
    GroceryList   list;
    auto &        list_attributes = reinterpret_cast<Attributes &>( list );    // direct access to list's private parts
    auto &        actual_array    = list_attributes._gList_array;
    auto &        actual_size     = list_attributes._gList_array_size;

    decltype( Attributes::_gList_array      ) expected_array;
    decltype( Attributes::_gList_array_size ) expected_size = 0;



    auto verify_arrays = [&]( char const * const message, std::size_t id )
    {
      // Only tally the mismatches and don't flood results with the matches
      if( expected_array != actual_array )
      {
        // native containers don't have an insertion operator, so convert to string and then compare the strings
        affirm.is_equal( "Array management deep interrogation (array) #" + std::to_string( id ) + " :  " + message, to_string( expected_array ), to_string( actual_array ) );
      }

      if( actual_size != expected_size )
      {
        affirm.is_equal( "Array management deep interrogation (size) #" + std::to_string( id ) + " :  " + message, expected_size, actual_size );
      }
    };



    // The tests below assume all unused spots of the expected array are filled with default constructed items.  Tests fail when
    // unused spots have something other than a default constructed item. Spots are filled with items that leave a residue after
    // moving, making then easy to detect if not cleaned up.


    // fill it up from the top ...
    while( expected_size < expected_array.size() )                                            // yes, all the way to capacity, not just _gList_array_size
    {
      GroceryItem gItem{ "item #" + std::to_string( expected_size ), "", "", 9876543.21 };    // something that will be recognizable after a move.  Strings will be cleared but not the price

      list.insert( gItem, GroceryList::Position::TOP );

      std::move_backward( expected_array.begin(), expected_array.end() - 1, expected_array.end() );
      expected_array[0] = std::move( gItem );
      ++expected_size;

      verify_arrays( "Fill to capacity", __LINE__ );
    }


    { // Remove from the top
      list.remove( 0 );
      std::move( expected_array.begin() + 1, expected_array.end(), expected_array.begin() );
      *( expected_array.end() - 1 ) = {};
      --expected_size;

      verify_arrays( "Remove from top", __LINE__ );
    }


    { // Remove the one after the top
      list.remove( 1 );
      std::move( expected_array.begin() + 2, expected_array.end(), expected_array.begin() + 1 );
      *( expected_array.end() - 1 ) = {};
      --expected_size;

      verify_arrays( "Remove 1st after top", __LINE__ );
    }


    { // Insert at the bottom with 2 empty slots available
      GroceryItem gItem{ "item #" + std::to_string( __LINE__ ), "", "", 9876543.21 };

      list.insert( gItem, GroceryList::Position::BOTTOM );
      *( expected_array.end() - 2 ) = std::move( gItem );
      ++expected_size;

      verify_arrays( "Insert at bottom with extra capacity", __LINE__ );
    }


    { // Insert at the bottom with 1 empty slot available
      GroceryItem gItem{ "item #" + std::to_string( __LINE__ ), "", "", 9876543.21 };

      list.insert( gItem, actual_size - 1 );
      std::move_backward( expected_array.end() - 2, expected_array.end() - 1, expected_array.end() );
      *( expected_array.end() - 2 ) = std::move( gItem );
      ++expected_size;

      verify_arrays( "Insert at bottom with no extra capacity", __LINE__ );
    }


    { // Remove first-from-bottom of a full list
      list.remove( actual_size - 2 );

      std::move( expected_array.end() - 1, expected_array.end(), expected_array.end() - 2 );
      *( expected_array.end() - 1 ) = {};
      --expected_size;

      verify_arrays( "Remove first-from-bottom of a full list", __LINE__ );
    }


    { // Remove bottom of a full list
      GroceryItem gItem{ "item #" + std::to_string( __LINE__ ), "", "", 9876543.21 };

      list.insert( gItem, GroceryList::Position::BOTTOM );                                    // fill the list back up to capacity (replace the item removed above)
      affirm.is_equal( "Array management deep interrogation (capacity)", actual_array.size(), actual_size );

      list.remove( actual_array.size() - 1);

      verify_arrays( "Remove bottom of a full list", __LINE__ );
    }

    { //  Moving too much when erasing?
      // start with a half full list
      while( list.size() > actual_array.size() / 2 ) list.remove( 0 );
      expected_array = actual_array;
      expected_size  = actual_size;

      // Fill the upper half with recognizable junk
      for( auto i = actual_size; i < actual_array.size(); ++i )
      {
        actual_array[i] = expected_array[i] = { "item #" + std::to_string( __LINE__ ) + '.' +  std::to_string(i), "", "", 9876543.21 };
      }

      // now remove something from the lower half
      list.remove( 2 );

      std::move( expected_array.begin() + 3, expected_array.begin() + expected_size, expected_array.begin() + 2 );
      --expected_size;
      actual_array[expected_size] = expected_array[expected_size] = {};                       // ignore the "hole" differences, this was checked above and gives false positives when whole not cleaned

      verify_arrays( "Moving too much when erasing", __LINE__ );
    }

    { //  Moving too much when inserting?
      GroceryItem gItem{ "item #" + std::to_string( __LINE__ ), "", "", 9876543.21 };
      list.insert( gItem, 1 );

      std::move_backward( expected_array.begin() + 1, expected_array.begin() + expected_size, expected_array.begin() + expected_size + 1 );
      expected_array[1] = gItem;
      ++expected_size;

      verify_arrays( "Moving too much when inserting", __LINE__ );
    }
  }    // GroceryListRegressionTest::deepArrayInterrogationTest()




  GroceryListRegressionTest::GroceryListRegressionTest()
  {
    // affirm.policy = Regression::CheckResults::ReportingPolicy::ALL;
    std::clog << std::boolalpha << std::showpoint << std::fixed << std::setprecision( 2 );


    try
    {
      std::clog << "\nGroceryList Regression Tests:\n";
      test();

      std::clog << "\nGroceryList Deep Array Interrogation Tests:\n";
      deepArrayInterrogationTest();

      std::clog << "\n\nGroceryList Regression Test " << affirm << "\n\n";
    }
    catch( const std::exception & ex )
    {
      std::clog << "FAILURE:  Regression test for \"class GroceryList\" failed with an unhandled exception. \n\n\n"
                << ex.what() << std::endl;
    }
  }
}    // namespace
