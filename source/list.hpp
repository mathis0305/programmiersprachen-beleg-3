#ifndef BUW_LIST_HPP
#define BUW_LIST_HPP
#include <cassert>
#include <cstddef>  //ptrdiff_t
#include <iterator> //std::bidirectional_iterator_tag
#include <iostream>

#include <initializer_list>

template <typename T>
class List;

template <typename T>
struct ListNode {
  T         value = T{};
  ListNode* prev = nullptr;
  ListNode* next = nullptr;
};


//TODO: Implementierung der Methoden des Iterators 
//      (nach Vorlesung STL-1 am 09. Juni) (Aufgabe 3.12)
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* DESCRIPTION  operator*() */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return node->value;
  }

  /* DESCRIPTION  operator->() */
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return &node->value;
  }


  /* PREINCREMENT, call: ++it, advances one element forward */
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    node = node->next;
    return *this;
    
    //TODO: Implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 3)
    
  }

  /* POSTINCREMENT (signature distinguishes the iterators), 
                    call:  it++, advances one element forward*/
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    auto copy = *this;
    node = node->next;
    return copy;

    //TODO: Implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 4)

  }


  /* ... */
  bool operator==(ListIterator<T> const& x) const {
    //TODO: Implement Equality-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 5)
    // Iterators should be the same if they refer to the same node
      if (node == x.node) {
          return true;
      }
    return false;
  } // call it: == it

  /* ... */
  bool operator!=(ListIterator<T> const& x) const {
    //TODO: Implement Inequality-Operation for Iterator  
    //      (Aufgabe 3.12 - Teil 6)
    // Reuse operator==
      
    return !(*this == x);
  } // call it: != it

  /* Advances Iterator */
  ListIterator<T> next() const {
    if (nullptr != node) {
      return ListIterator{node->next};
    } else {
      return ListIterator{nullptr};
    }
  }


  ListNode <T>* node = nullptr;
};



template <typename T>
class List {
  public:

    //friend declarations for testing the members   
    template <typename TEST_TYPE>
    friend size_t get_size(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_first_pointer(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_last_pointer(List<TEST_TYPE> const& list_to_test);

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using iterator        = ListIterator<T>;

    /* ... */
    // default constructor
    List() :
        size_{ 0 },
        first_{ nullptr },
        last_{ nullptr } {}

    // copy constructor
    List(List<T> const& list) :
        size_{ 0 },
        first_{ nullptr },
        last_{ nullptr }
    {
        ListNode<T>* node = list.first_;
        while (nullptr != node) {
            push_back(node->value);
            node = node->next;
        }
    }

    // test and implement:
    // TODO: Move-Konstruktor (Aufgabe 3.14)
    List(List<T>&& rhs) :
        size_{ 0 },
        first_{ nullptr },
        last_{ nullptr }    
    {
        *this = rhs;
        rhs.first_ = nullptr;
        rhs.last_ = nullptr;
        rhs.size_ = 0;
    }

    //TODO: Initializer-List Konstruktor (3.10 - Teil 1)
    /* ... */
    // test and implement:
    List(std::initializer_list<T> ini_list) {
      //not implemented yet
    }

    /* ... */
    // test and implement:

    void operator=(List<T> rhs) {
        auto node1 = first_;
        first_ = rhs.first_;
        rhs.first_ = node1;

        auto node2 = last_;
        last_ = rhs.last_;
        rhs.last_ = node2;

        auto s = size_;
        size_ = rhs.size_;
        rhs.size_ = s;
    }

    // == operator
    bool operator==(List const& rhs) const {
        if (size_ != rhs.size_) {
            return false;
        }
        auto node1 = first_;
        auto node2 = rhs.first_;
        while (node1 != nullptr) {
            if (node1->value != node2->value) {
                return false;
            }
            node1 = node1->next;
            node2 = node2->next;
        }
        return true;
    }

    // != operator
    bool operator!=(List const& rhs) const
    {
        return !(*this == rhs);
    }

    /* ... */
    ~List() {
        clear();
        delete first_;
        delete last_;
    }

    /* ... */
    ListIterator<T> begin() {
      //TODO: begin-Method returning an Iterator to the 
      //      first element in the List (Aufgabe 3.11)
        iterator begin{ first_ };
      return begin;
    }

    /* ... */
    ListIterator<T> end() {
      //TODO: end-Method returning an Iterator to element after (!) 
      //      the last element in the List (Aufgabe 3.11)
        iterator end{ nullptr };
      return end;
    }

    /* ... */ 
    // test and implement:
    void clear() {
        while (size_ > 0) {
            if (first_ != nullptr) {
                pop_front();
            }
        }
    }


    /* ... */
    //TODO: member function insert (Aufgabe 3.13)
    ListIterator<T> insert(ListNode<T> &node, ListIterator<T> &it) {
        iterator prev{ it.node->prev };
        it.node->prev = node;
        node->next = it.node;
        node->prev = prev.node;
        prev.node->next = node;
        iterator new_it { node };
        return new_it;
    }


    /* ... */
    //TODO: member function insert (Aufgabe 3.14)

    /* ... */

    // member funcion reverse
    void reverse() {
        if (empty()) {
            throw "List is empty";
        }
        else {
            auto node1 = first_;
            first_ = last_;
            last_ = node1;

            auto node2 = last_;
            while (nullptr != node2) {
                auto node3 = node2->next;
                node2->next = node2->prev;
                node2->prev = node3;
                node2 = node3;
            }
        }
    }


    /* ... */
    void push_front(T const& element) {

        ListNode<T>* node = new ListNode<T> { element, nullptr, first_ };
        if (empty()) {
            last_ = node;
        }
        else {
            first_->prev = node;
        }
        first_ = node;
        size_ += 1;
    }

    /* ... */
    void push_back(T const& element) {

        ListNode<T>* node = new ListNode<T>{ element, last_, nullptr };
        if (empty()) {
            first_ = node;
        }
        else {
            last_->next = node;
        }
        last_ = node;
        size_ += 1;
    }

    /* ... */
    void pop_front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          if (size_ == 1) {
              delete first_->next;
              delete first_->prev;
              first_ = nullptr;
              last_ = nullptr;
              size_ -= 1;
          }
          else {
              first_ = first_->next;
              delete first_->prev->next;
              delete first_->prev->prev;
              first_->prev = nullptr;
              size_ -= 1;
          }
      }
    }

    /* ... */
    void pop_back() {
        if (empty()) {
            throw "List is empty";
        }
        else {
            if (size_ == 1) {
                delete first_->next;
                delete first_->prev;
                first_ = nullptr;
                last_ = nullptr;
                size_ -= 1;
            }
            else {
                last_ = last_->prev;
                delete last_->next->prev;
                delete last_->next->next;
                last_->next = nullptr;
                size_ -= 1;
            }
        }
    }
    /* ... */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return first_->value;
      }
    }

    /* ... */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return last_->value;
      }
    }

    /* ... */
    bool empty() const {

      return size_ == 0;
    };


    /* ... */
    std::size_t size() const{ 
      return size_;
  };


  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};


/* ... */
// free function reverse
template <typename T>
List<T> reverse(List<T> const& list) {
    List<T> list2{ list };
    list2.reverse();
    return list2;
}

/* ... */
//TODO: Freie Funktion operator+ (3.10 - Teil 2)


#endif // # define BUW_LIST_HPP