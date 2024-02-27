#pragma once                                                                                  // include guard

#include <array>
#include <compare>                                                                            // weak_ordering
#include <cstddef>                                                                            // size_t
#include <forward_list>
#include <initializer_list>
#include <iostream>
#include <list>
#include <stdexcept>                                                                          // domain_error, length_error, logic_error
#include <vector>

#include "GroceryItem.hpp"


class GroceryList
{
  // Insertion and Extraction Operators
  friend std::ostream & operator<<( std::ostream & stream, GroceryList const & groceryList );
  friend std::istream & operator>>( std::istream & stream, GroceryList       & groceryList );

  public:
    // Types and Exceptions
    enum class Position {TOP, BOTTOM};

    struct InvalidInternalState_Ex : std::domain_error { using domain_error::domain_error; }; // Thrown if internal data structures become inconsistent with each other
    struct CapacityExceeded_Ex     : std::length_error { using length_error::length_error; }; // Thrown if more grocery items are inserted than will fit
    struct InvalidOffset_Ex        : std::logic_error  { using logic_error ::logic_error;  }; // Thrown if inserting beyond current size


    // Constructors, destructor, and assignments
    //
    // The compiler synthesized copy and move constructors, and copy and move assignment operators work just fine.  But since I also
    // have user defined constructors, I need to explicitly say the compiler synthesized default constructor is also okay.
    GroceryList() = default;                                                                  // constructs an empty grocery list
    GroceryList( std::initializer_list<GroceryItem> const & initList );                       // constructs a grocery list from a braced list of grocery items


    // Queries
    std::size_t size() const;                                                                 // returns the number of grocery items in this grocery list


    // Accessors
    std::size_t find( const GroceryItem & groceryItem ) const;                                // returns the grocery item's (zero-based) offset from top, size() if grocery item not found


    // Modifiers
    void insert   ( GroceryItem const & groceryItem, Position    position = Position::TOP );  // inserts the grocery item at the top (beginning) or bottom (end) of the grocery list
    void insert   ( GroceryItem const & groceryItem, std::size_t offsetFromTop            );  // inserts before the existing grocery item currently at that offset

    void remove   ( GroceryItem const & groceryItem                                       );  // no change occurs if grocery item not found
    void remove   ( std::size_t         offsetFromTop                                     );  // no change occurs if (zero-based) offsetFromTop >= size()

    void moveToTop( GroceryItem const & groceryItem                                       );  // finds then moves grocery item from its current position to the top of the grocery list

    GroceryList & operator+=( std::initializer_list<GroceryItem> const & rhs );               // appends (aka concatenates) a braced list of grocery items to the end of this list
    GroceryList & operator+=( GroceryList                        const & rhs );               // appends (aka concatenates) the rhs list to the bottom of this list


    // Relational Operators
    std::weak_ordering operator<=>( GroceryList const & rhs ) const;
    bool               operator== ( GroceryList const & rhs ) const;


  private:
    // Instance Attributes
    std::array       <GroceryItem, 11>  _gList_array;                                         // underlying containers holding grocery items
    std::vector      <GroceryItem    >  _gList_vector;                                        // operations performed on once container must be
    std::list        <GroceryItem    >  _gList_dll;                                           // replicated across all containers
    std::forward_list<GroceryItem    >  _gList_sll;

    std::size_t                         _gList_array_size = 0;                                // number of valid elements in _gList_array


    // Helper member functions
    bool        containersAreConsistant() const;
    std::size_t gList_sll_size         () const;                                              // std::forward_list doesn't maintain size, so calculate it on demand
};
