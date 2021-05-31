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



template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* The star operator dereferences the iterator by giving back a reference of the value of the node it points on */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return node->value;
  }

  /* The arrow operator dereferences the iterator by giving back a pointer to the value of the node it points on  */
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
  }


  /* gives back true when both iterators refer to the same node */
  bool operator==(ListIterator<T> const& x) const {
      if (node == x.node) {
          return true;
      }
    return false;
  }

  /* gives back true when both iterators refer to the different nodes */
  bool operator!=(ListIterator<T> const& x) const {      
    return !(*this == x);
  }

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
        // pushes the value of the old node to the new List to make a new node
        ListNode<T>* node = list.first_;
        while (nullptr != node) {
            push_back(node->value);
            node = node->next;
        }
    }

    // move constructor
    List(List<T>&& rhs) :
        size_{ 0 },
        first_{ nullptr },
        last_{ nullptr }    
    {
        // copy construct new List
        *this = rhs;

        // emptying old List
        rhs.first_ = nullptr;
        rhs.last_ = nullptr;
        rhs.size_ = 0;
    }

    // optional initializer list
    List(std::initializer_list<T> ini_list) {
      //not implemented yet
    }

    // the unifying assignment operator swaps both nodes
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

    /* gives back true when both Lists contain the same values */
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

    // /* gives back true when both Lists contain different values */
    bool operator!=(List const& rhs) const
    {
        return !(*this == rhs);
    }

    // the destructor is called when the scope of a virable is ending. It just clears the List
    ~List() {
        clear();
    }

    /* returns an Iterator to the first element in the List using the first_-pointer*/
    ListIterator<T> begin() {
        iterator begin{ first_ };
      return begin;
    }

    /* returns an Iterator to the element behind the last element of the list, which is a nullptr*/
    ListIterator<T> end() {
        iterator end{ nullptr };
      return end;
    }

    // pops all elements of the List by using the pop_front() method
    void clear() {
        while (size_ > 0) {
            if (first_ != nullptr) {
                pop_front();
            }
        }
    }


    // insert a node before the given iterator and return an iterator to the new node
    ListIterator<T> insert(ListNode<T> &node, ListIterator<T> &it) {
        iterator prev{ it.node->prev };
        it.node->prev = node;
        node->next = it.node;
        node->prev = prev.node;
        prev.node->next = node;
        iterator new_it { node };
        return new_it;
    }


    // reverses the List so that the last element will be the first
    void reverse() {
        if (empty()) {
            throw "List is empty";
        }
        else {
            auto node1 = first_;
            first_ = last_;
            last_ = node1;

            // swaps all pointers from next to prev and from prev to next
            auto node2 = last_;
            while (nullptr != node2) {
                auto node3 = node2->next;
                node2->next = node2->prev;
                node2->prev = node3;
                node2 = node3;
            }
        }
    }


    /* makes a new node and sets it to the first list element */
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

    /* makes a new node and sets it to the last list element */
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

    /* returns the first list element and deletes the element from the List and in the storage */
    void pop_front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          if (size_ == 1) {
              delete first_;
              first_ = nullptr;
              last_ = nullptr;
              size_ -= 1;
          }
          else {
              first_ = first_->next;
              delete first_->prev;
              first_->prev = nullptr;
              size_ -= 1;
          }
      }
    }

    /* returns the last list element and deletes the element from the List and in the storage */
    void pop_back() {
        if (empty()) {
            throw "List is empty";
        }
        else {
            if (size_ == 1) {
                delete first_;
                first_ = nullptr;
                last_ = nullptr;
                size_ -= 1;
            }
            else {
                last_ = last_->prev;
                delete last_->next;
                last_->next = nullptr;
                size_ -= 1;
            }
        }
    }
    /* returns value of the first node without removing it */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return first_->value;
      }
    }

    /* returns value of the last node without removing it */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return last_->value;
      }
    }

    /* returns true if there is no node in the list meaning that first_ and last_ are nullpointers and the size_ == 0 */
    bool empty() const {

      return size_ == 0;
    };


    /* returns the number of nodes in the List */
    std::size_t size() const{ 
      return size_;
  };


  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};


/* same as the member function reverse but as free function. It makes a new List with the copy constructor and uses the member function */
template <typename T>
List<T> reverse(List<T> const& list) {
    List<T> list2{ list };
    list2.reverse();
    return list2;
}


#endif // # define BUW_LIST_HPP