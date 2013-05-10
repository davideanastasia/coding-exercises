#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <cassert>

/// \brief generic skiplist node: can contain a pair (for a map) or a single
/// value (for a set)
template <typename Tp>
struct node
{
    typedef Tp value_type;

    /// \brief value of the node
    value_type value_;

    /// \brief depth of the current element
    size_t depth_;
    /// \brief vector to node(s)
    node** forward_;

    node(Tp value, size_t depth)
        : value_(value)
        , depth_(depth)
        , forward_(new node*[depth])
    {
        // set all pointers to zero...
        for (size_t idx = 0; idx < depth_; ++idx) {
            forward_[idx] = NULL;
        }
    }

    ~node()
    {
        delete [] forward_;
    }
};

/// \brief iterator for skiplist nodes
template <typename Tp>
class skiplist_iterator
{
public:
    typedef skiplist_iterator<Tp>                   self;

    typedef typename std::forward_iterator_tag      iterator_category;
    typedef ptrdiff_t                               difference_type;

    typedef Tp                                      value_type;
    typedef Tp&                                     reference;
    typedef Tp*                                     pointer;

    typedef node<Tp>*                               node_ptr;

    /// \brief ctor
    explicit skiplist_iterator(node_ptr ptr)
        : current_node_(ptr)
    {}

    /// \brief empty ctor
    skiplist_iterator()
        : current_node_(NULL)
    {}

    /// \brief copy ctor
    skiplist_iterator(const skiplist_iterator& rhs)
        : current_node_(rhs.current_node_)
    {}

    self& operator++() {
        current_node_ = current_node_->forward_[0];
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        current_node_ = current_node_->forward_[0];
        return tmp;
    }

    pointer operator->() const {
        return &current_node_->value_;
    }

    reference operator*() const {
        return current_node_->value_;
    }

    bool operator==(const self& other) const
    { return current_node_ == other.current_node_; }

    bool operator!=(const self& other) const
    { return current_node_ != other.current_node_; }

    node_ptr current_node_;
};

/// \brief const iterator for skiplist nodes
template <typename Tp>
class skiplist_const_iterator
{
public:
    typedef skiplist_const_iterator<Tp>             self;

    typedef typename std::forward_iterator_tag      iterator_category;
    typedef ptrdiff_t                               difference_type;

    typedef Tp                                      value_type;
    typedef const Tp&                               reference;
    typedef const Tp*                               pointer;

    typedef const node<Tp>*                         node_ptr;

    typedef skiplist_iterator<Tp>                   iterator;

    /// \brief ctor
    explicit skiplist_const_iterator(node_ptr ptr)
        : current_node_(ptr)
    {}

    /// \brief empty ctor
    skiplist_const_iterator()
        : current_node_(NULL)
    {}

    /// \brief copy ctor
    skiplist_const_iterator(const skiplist_const_iterator& rhs)
        : current_node_(rhs.current_node_)
    {}

    /// \brief copy ctor from non-const iterator
    skiplist_const_iterator(const iterator& rhs)
        : current_node_(rhs.current_node_)
    {}

    self& operator++() {
        current_node_ = current_node_->forward_[0];
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        current_node_ = current_node_->forward_[0];
        return tmp;
    }

    pointer operator->() const {
        return &current_node_->value_;
    }

    reference operator*() const {
        return current_node_->value_;
    }

    bool operator==(const self& other) const
    { return current_node_ == other.current_node_; }

    bool operator!=(const self& other) const
    { return current_node_ != other.current_node_; }

    node_ptr current_node_;
};

template <typename Val>
inline bool
operator==(const skiplist_iterator<Val>& x,
           const skiplist_const_iterator<Val>& y)
{ return x.current_node_ == y.current_node_; }

template <typename Val>
inline bool
operator!=(const skiplist_iterator<Val>& x,
           const skiplist_const_iterator<Val>& y)
{ return x.current_node_ != y.current_node_; }


template <typename TKey, typename TVal,
          typename Comp = std::less< TKey >,
          size_t MaxDepth = 10>
class skiplist
{
public:
    typedef TKey                                key_type;
    typedef TVal                                mapped_type;
    typedef std::pair<const TKey, TVal>         value_type;

    typedef node<value_type>                    node_type;

    typedef Comp                                key_compare;
    // typedef Alloc                            allocator_type;

private:
    /// \brief comparator
    Comp comp_;

    /// \brief number of nodes in the list
    size_t num_nodes_;
    // size_t default_depth_;

    /// \brief head of the list
    node_type * head_;

    size_t get_level() {
        size_t l = 1;
        while ( rand() < RAND_MAX/2 && l < MaxDepth ) {
            l++;
        }

        assert( l >= 1 );
        assert( l <= MaxDepth );

        return l;
    }

public:
    skiplist()
        : num_nodes_(0)
        , head_(new node_type(std::make_pair(TKey(), TVal()), MaxDepth))
    {}

    ~skiplist() {
        node_type* current_node = head_;
        while ( current_node != NULL )
        {
            node_type* current_temp = current_node;
            current_node = current_node->forward_[0];

            // every node will get rid of his internal data
            delete current_temp;
        }
    }

    /// \return current number of nodes in the data structure
    size_t size() const { return num_nodes_; }

    TVal* find(const TKey& key)
    {
        node_type* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->value_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
        }
        current_node = current_node->forward_[0];
        if ( current_node != NULL && current_node->value_.first == key)
        {
            return &current_node->value_.second;
        }
        return NULL;
    }

    bool insert(const TKey& key, const TVal& val)
    {
        node_type* node_update[MaxDepth];
        node_type* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->value_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
            node_update[current_level] = current_node;
        }
        current_node = current_node->forward_[0];
        if ( current_node != NULL && current_node->value_.first == key ) {
            return false;
        }

        node_type* new_node = new node_type(std::make_pair(key, val), get_level());
        for (size_t idx = 0; idx < new_node->depth_; ++idx)
        {
            new_node->forward_[idx] = node_update[idx]->forward_[idx];
            node_update[idx]->forward_[idx] = new_node;
        }
        ++num_nodes_;
        return true;
    }

    bool remove(const TKey& key)
    {
        node_type* node_update[MaxDepth];    // book keeping!
        node_type* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->value_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
            node_update[current_level] = current_node;
        }
        current_node = current_node->forward_[0];   // ... move to current node
        if ( current_node == NULL || current_node->value_.first != key ) {
            return false;
        }

        for (size_t idx = 0; idx < current_node->depth_; ++idx)
        {
            node_update[idx]->forward_[idx] = current_node->forward_[idx];
        }
        delete current_node;
        --num_nodes_;

        return true;
    }

    void print(std::ostream& out)
    {
        for ( int current_level = MaxDepth-1; current_level >= 0; --current_level )
        {
            out << std::setw(5) << "----";
        }
        out << " [" << std::setw(5) << num_nodes_ << "]";
        out << std::endl;

        for ( node_type* current_node = head_; current_node != NULL ; current_node = current_node->forward_[0] )
        {
            for ( int current_level = MaxDepth-1; current_level >= 0; --current_level )
            {
                if ( current_level < current_node->depth_ ) {
                    if ( current_node->forward_[current_level] != NULL ) {
                        out << std::setw(5) << current_node->forward_[current_level]->value_.first;
                    } else {
                        out << std::setw(5) << "o";
                    }
                } else {
                    out << std::setw(5) << ".";
                }
            }
            out << " [" << std::setw(5) << current_node->value_.first << "]";
            out << std::endl;
        }
    }

public:
    typedef skiplist_iterator<value_type>             iterator;
    typedef skiplist_const_iterator<value_type>       const_iterator;

    iterator begin()             { return iterator(head_->forward_[0]); }
    iterator end()               { return iterator(NULL); }

    const_iterator begin() const { return const_iterator(head_->forward_[0]); }
    const_iterator end()   const { return const_iterator(NULL); }
};



#endif // SKIPLIST_H
