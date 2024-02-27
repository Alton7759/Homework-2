#include <algorithm>                                                                // find(), shift_left(), shift_right(), equal(), swap(), lexicographical_compare()
#include <cmath>                                                                    // min()
#include <cstddef>                                                                  // size_t
#include <initializer_list>
#include <iomanip>                                                                  // setw()
#include <iterator>                                                                 // distance(), next()
#include <stdexcept>                                                                // logic_error
#include <string>
#include <utility>                                                                  // move()

#include "GroceryItem.hpp"
#include "GroceryList.hpp"



// As a rule, I strongly recommend avoiding macros, unless there is a compelling reason - this is such a case. This really does need
// to be a macro and not a function due to the way the preprocessor expands the source code location information.  It's important to
// have these expanded where they are used, and not here. But I just can't bring myself to writing this, and getting it correct,
// everywhere it is used.  Note:  C++20 will change this technique with the introduction of the std::source_location class. Also note the
// usage of having the preprocessor concatenate two string literals separated only by whitespace.  :(  In the meantime ...
//
// Update:  As of August 2023 and clang 16, still no support for source_location. Rummer has it that they may never implement it.
// Yay Microsoft and GCC!
#define exception_location "\n detected in function \"" + std::string(__func__) +  "\""    \
                           "\n at line " + std::to_string( __LINE__ ) +                    \
                           "\n in file \"" __FILE__ "\""












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializer List Constructor
GroceryList::GroceryList( const std::initializer_list<GroceryItem> & initList )
{
  for( auto && groceryItem : initList )   insert( groceryItem, Position::BOTTOM );

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size() const
std::size_t GroceryList::size() const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (1) //////////////////////////////
    /// All the containers are the same size, so pick one and return the size of that.  Since the forward_list has to calculate the
    /// size on demand, stay away from using that one.
  return _gList_vector.size();
  /////////////////////// END-TO-DO (1) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (2) //////////////////////////////
    /// Locate the grocery item in this grocery list and return the zero-based position of that grocery item.  If the grocery item
    /// does not exist, return the size of this grocery list as an indicator the grocery item does not exist.  The grocery item will
    /// be in the same position in all the containers (array, vector, list, and forward_list) so pick just one of those to search.
    /// The STL provides the find() function that is a perfect fit here, but you may also write your own loop.
  auto what_we_need_to_find = std::find(_gList_vector.begin(), _gList_vector.end(), groceryItem);
  const std::size_t index_of_found = std::distance(_gList_vector.begin(), what_we_need_to_find);
  return index_of_found;
  /////////////////////// END-TO-DO (2) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifiers
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// insert( position )
void GroceryList::insert( const GroceryItem & groceryItem, Position position )
{
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work
  if     ( position == Position::TOP    )  insert( groceryItem, 0      );
  else if( position == Position::BOTTOM )  insert( groceryItem, size() );
  else                                     throw std::logic_error( "Unexpected insertion position" exception_location );  // Programmer error.  Should never hit this!
}



// insert( offset )
void GroceryList::insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )        // insert provided grocery item at offsetFromTop, which places it before the current grocery item at offsetFromTop
{
  // Validate offset parameter before attempting the insertion.  std::size_t is an unsigned type, so no need to check for negative
  // offsets, and an offset equal to the size of the list says to insert at the end (bottom) of the list.  Anything greater than the
  // current size is an error.
  if( offsetFromTop > size() )   throw InvalidOffset_Ex( "Insertion position beyond end of current list size" exception_location );


  /**********  Prevent duplicate entries  ***********************/
  ///////////////////////// TO-DO (3) //////////////////////////////
    /// Silently discard duplicate items from getting added to the grocery list.  If the to-be-inserted grocery item is already in
    /// the list, simply return.
    ///
    /// Remember, you already have a function that tells you if the to-be-inserted grocery item is already in the list, so use it.
    /// Don't implement it again.
  if (find(groceryItem) != _gList_vector.size()) return;
  /////////////////////// END-TO-DO (3) ////////////////////////////


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    ///////////////////////// TO-DO (4) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      /// https://github.com/TBettens/Data_Structure_Implementation_Examples/tree/latest/Sequence%20Container%20Implementation%20Examples/Vector
      ///
      /// Unlike the other containers, an std::array has fixed capacity and fixed size (and size is always == capacity) hence has no
      /// insert() function, so you have to write it yourself. Insert into the array by shifting all the items at and after the
      /// insertion point (offsetFromTop) to the right opening a gap in the array that can be populated with the given grocery item.
      /// Remember that arrays have fixed capacity and cannot grow, so make sure there is room in the array for another grocery item
      /// before you start by verifying _gList_array_size (the number of valid elements) is less than _gList_array.size() (the
      /// capacity).  If not, throw CapacityExceeded_ex. Also remember that you must keep track of the number of valid grocery items
      /// in your array, so don't forget to adjust _gList_array_size.
      ///
      /// Open a hole to insert new grocery item by shifting to the right everything at and after the insertion point.
      /// For example:  a[8] = a[7];  a[7] = a[6];  a[6] = a[5];  and so on.
      /// std::shift_* will be helpful, or write your own loop./*
      _gList_array_size += 1;
      if(_gList_array_size > _gList_array.max_size()) throw CapacityExceeded_Ex( "you no more room to add ");
      std::shift_right(std::next(_gList_array.begin(),offsetFromTop), std::next(_gList_array.begin(), _gList_array_size), 1);
      _gList_array.at(offsetFromTop) = groceryItem;

     
    
    /////////////////////// END-TO-DO (4) ////////////////////////////
  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    ///////////////////////// TO-DO (5) //////////////////////////////
      /// The vector STL container std::vector has an insert function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::insert() shifts to the right everything at and after the insertion point, just like you
      /// did for the array above.
    _gList_vector.insert(std::next(_gList_vector.begin(), offsetFromTop), groceryItem);
    if (_gList_vector.size() > _gList_array.max_size()) _gList_vector.pop_back();
    /////////////////////// END-TO-DO (5) ////////////////////////////
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    ///////////////////////// TO-DO (6) //////////////////////////////
      /// The doubly linked list STL container std::list has an insert function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert
      /// the zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The
      /// STL has a function called std::next() that does that, or you can write your own loop.
    _gList_dll.insert(std::next(_gList_dll.begin(), offsetFromTop), groceryItem);
    if(_gList_dll.size() > _gList_array.max_size()) _gList_dll.pop_back();
    /////////////////////// END-TO-DO (6) ////////////////////////////
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
    ///////////////////////// TO-DO (7) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an insert function, which can be directly used here.  But that
      /// function inserts AFTER the grocery item pointed to, not before like the other containers.  A singly linked list cannot
      /// look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.
    _gList_sll.insert_after(std::next(_gList_sll.before_begin(), offsetFromTop), groceryItem);
    if(gList_sll_size() > _gList_array.max_size()) _gList_sll.erase_after(std::next(_gList_sll.before_begin(),_gList_array.max_size()));
    /////////////////////// END-TO-DO (7) ////////////////////////////
  } // Part 4 - Insert into singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )



// remove( groceryItem )
void GroceryList::remove( const GroceryItem & groceryItem )
{
  // Delegate to the version of remove() that takes an index as a parameter
  remove( find( groceryItem ) );
}



// remove( offset )
void GroceryList::remove( std::size_t offsetFromTop )
{
  // Removing from the grocery list means you remove the grocery item from each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets removed is a
  // little different for each.  You are to remove the grocery item from each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.

  if( offsetFromTop >= size() )   return;                                           // no change occurs if (zero-based) offsetFromTop >= size()


  { /**********  Part 1 - Remove from array  ***********************/
    ///////////////////////// TO-DO (8) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      ///
      ///
      /// Close the hole created by shifting to the left everything at and after the remove point.
      /// For example:  a[5] = a[6];  a[6] = a[7];  a[7] = a[8];  and so on
      ///
      /// std::move() will be helpful, or write your own loop.  Also remember that you must keep track of the number of valid grocery items
      /// in your array, so don't forget to adjust _gList_array_size.
      
      std::shift_left(std::next(_gList_array.begin(), offsetFromTop), std::next(_gList_array.begin(), _gList_array_size) , 1);
      --_gList_array_size;
      _gList_array.at(_gList_array_size) = GroceryItem();
    /////////////////////// END-TO-DO (8) ////////////////////////////
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    ///////////////////////// TO-DO (9) //////////////////////////////
      /// The vector STL container std::vector has an erase function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to be removed.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::erase() shifts to the left everything after the insertion point, just like you did for the
      /// array above.
      _gList_vector.erase(std::next(_gList_vector.begin(), offsetFromTop));
    /////////////////////// END-TO-DO (9) ////////////////////////////
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    ///////////////////////// TO-DO (10) //////////////////////////////
      /// The doubly linked list STL container std::list has an erase function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to remove.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can write your own loop.
      _gList_dll.erase(std::next(_gList_dll.begin(), offsetFromTop));
    /////////////////////// END-TO-DO (10) ////////////////////////////
  } // Part 3 - Remove from doubly linked list




  {/**********  Part 4 - Remove from singly linked list  **********/
    ///////////////////////// TO-DO (11) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an erase function, which can be directly used here.  But that
      /// function erases AFTER the grocery item pointed to, not the one pointed to like the other containers.  A singly linked list
      /// cannot look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.
      _gList_sll.erase_after(std::next(_gList_sll.before_begin(), offsetFromTop));
    /////////////////////// END-TO-DO (11) ////////////////////////////
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  ///////////////////////// TO-DO (12) //////////////////////////////
    /// If the grocery item exists, then remove and reinsert it.  Otherwise, do nothing.
    /// Remember, you already have functions to do all this.
    auto found = find(groceryItem);
   if (found != _gList_vector.size()){
    remove(found);
    insert(groceryItem, Position::TOP);
   }
  /////////////////////// END-TO-DO (12) ////////////////////////////
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  ///////////////////////// TO-DO (13) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. The input type is just a container of grocery items accessible with iterators just like all the other
    /// containers.  The constructor above gives an example.  Remember to add that grocery item at the bottom of each container
    /// (array, vector, list, and forward_list) of this grocery list, and that you already have a function that does that.
  for (const auto & items : rhs)
  {
    insert(items,Position::BOTTOM);
  }
  
  /////////////////////// END-TO-DO (13) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  ///////////////////////// TO-DO (14) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. All the rhs containers (array, vector, list, and forward_list) contain the same information, so pick just one
    /// to traverse. Walk the container you picked inserting its grocery items to the bottom of this grocery list. Remember to add
    /// that grocery item at the bottom of each container (array, vector, list, and forward_list) of this grocery list, and that you
    /// already have a function that does that.
  for (const auto & items : rhs._gList_vector)
  {
    insert(items, Position::BOTTOM);
  }
  /////////////////////// END-TO-DO (14) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Relational Operators
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<=>
std::weak_ordering GroceryList::operator<=>( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (15) //////////////////////////////
    /// Find the common extent.  That is, if one list has 20 grocery items, and the other has only 13, then the common extent is 13
    /// grocery items. We can compare only the first 13 grocery items of each list.  Find the first grocery item in the common
    /// extent that is different using three-way comparison (e.g., gItem1 <=> gItem2  !=  0) and return the results.  If no
    /// differences were found in the common extent, then the list with the smaller size is less than the other.  In that case,
    /// return the results of three-way comparing the sizes.
    ///
    ///
    /// The content of all the grocery lists's containers is the same - so pick an easy one to walk.
  unsigned its = 0;
  for (const auto & array : _gList_array)
  {
    auto results = array <=> rhs._gList_vector[its];
    if (results != 0){
      return results;
    }
    ++its;
  }
  return size() <=> rhs.size() ;
  
  
  
  /////////////////////// END-TO-DO (15) ////////////////////////////
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (16) //////////////////////////////
    /// Two lists are different if their sizes are different, or one of their grocery items is different.  Otherwise the lists are
    /// equal.
    ///
    /// Comparing the sizes is quick (constant time), so check that first.
    ///
    /// Walk the list looking for grocery items that don't match.  The content of all the grocery lists's containers is the same -
    /// so pick an easy one to walk.
    if (size() != rhs.size()) return false;
   
      unsigned its = 0;
      for (const auto & array : _gList_array) {
        if(!(array == rhs._gList_array[its])) return false;
        ++its;
      }
   
    return true;
   
  /////////////////////// END-TO-DO (16) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// containersAreConsistant() const
bool GroceryList::containersAreConsistant() const
{
  // Sizes of all containers must be equal to each other
  if(    _gList_array_size != _gList_vector.size()
      || _gList_array_size != _gList_dll.size()
      || _gList_array_size !=  gList_sll_size() ) return false;

  // Element content and order must be equal to each other
  auto current_array_position   = _gList_array .cbegin();
  auto current_vector_position  = _gList_vector.cbegin();
  auto current_dll_position     = _gList_dll   .cbegin();
  auto current_sll_position     = _gList_sll   .cbegin();

  auto end = _gList_vector.cend();
  while( current_vector_position != end )
  {
    if(    *current_array_position != *current_vector_position
       || *current_array_position != *current_dll_position
       || *current_array_position != *current_sll_position ) return false;

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dll_position;
    ++current_sll_position;
  }

  return true;
}



// gList_sll_size() const
std::size_t GroceryList::gList_sll_size() const
{
  ///////////////////////// TO-DO (17) //////////////////////////////
    /// Some implementations of a singly linked list maintain the size (number of elements in the list).  std::forward_list does
    /// not. The size of singly linked list must be calculated on demand by walking the list from beginning to end counting the
    /// number of elements visited.  The STL's std::distance() function does that, or you can write your own loop.
  return std::distance(_gList_sll.begin(), _gList_sll.end());
  
  /////////////////////// END-TO-DO (17) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  // For each grocery item in the provided grocery list, insert the grocery item into the provided stream.  Each grocery item is
  // inserted on a new line and preceded with its index (aka offset from top)
  unsigned count = 0;
  for( auto && groceryItem : groceryList._gList_sll )   stream << '\n' << std::setw(5) << count++ << ":  " << groceryItem;

  return stream;
}



// operator>>
std::istream & operator>>( std::istream & stream, GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (18) //////////////////////////////
    /// Extract until end of file grocery items from the provided stream and insert them at the bottom of the provided grocery list.
    /// Be sure to extract grocery items and not individual fields such as product name or UPC.
    GroceryItem holder;
    while (stream >> holder){
    groceryList.insert(holder, GroceryList::Position::BOTTOM);
    }
  /////////////////////// END-TO-DO (18) ////////////////////////////

  return stream;
}
