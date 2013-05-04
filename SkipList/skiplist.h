#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <cassert>

template <typename TKey, typename TVal,
          typename Comp = std::less< TKey >,
          size_t MaxDepth = 10>
class skiplist
{
private:
    struct node
    {
        //! \brief key,value pair
        std::pair<TKey, TVal> pair_;

        /// \brief depth of the current element
        size_t depth_;
        /// \brief vector to node(s)
        node** forward_;

        node(TKey key, TVal val, size_t depth)
            : pair_( std::make_pair(key, val) )
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

    /// \brief comparator
    Comp comp_;

    /// \brief number of nodes in the list
    size_t num_nodes_;
    // size_t default_depth_;

    /// \brief head of the list
    node * head_;

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
        , head_(new node(TKey(), TVal(), MaxDepth))
    {}

    ~skiplist() {
        node* current_node = head_;
        while ( current_node != NULL )
        {
            node* current_temp = current_node;
            current_node = current_node->forward_[0];

            // every node will get rid of his internal data
            delete current_temp;
        }
    }

    //! \return current number of nodes in the data structure
    size_t size() const { return num_nodes_; }

    TVal* find(const TKey& key)
    {
        node* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->pair_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
        }
        current_node = current_node->forward_[0];
        if ( current_node != NULL && current_node->pair_.first == key)
        {
            return &current_node->pair_.second;
        }
        return NULL;
    }

    bool insert(const TKey& key, const TVal& val)
    {
        node* node_update[MaxDepth];
        node* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->pair_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
            node_update[current_level] = current_node;
        }
        current_node = current_node->forward_[0];
        if ( current_node != NULL && current_node->pair_.first == key ) {
            return false;
        }

        node* new_node = new node(key, val, get_level());
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
        node* node_update[MaxDepth];    // book keeping!
        node* current_node = head_;
        for ( int current_level = MaxDepth - 1; current_level >= 0; --current_level )
        {
            while ( current_node->forward_[current_level] != NULL &&
                    comp_(current_node->forward_[current_level]->pair_.first, key) )
            {
                current_node = current_node->forward_[current_level];
            }
            node_update[current_level] = current_node;
        }
        current_node = current_node->forward_[0];   // ... move to current node
        if ( current_node == NULL || current_node->pair_.first != key ) {
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

        for ( node* current_node = head_; current_node != NULL ; current_node = current_node->forward_[0] )
        {
            for ( int current_level = MaxDepth-1; current_level >= 0; --current_level )
            {
                if ( current_level < current_node->depth_ ) {
                    if ( current_node->forward_[current_level] != NULL ) {
                        out << std::setw(5) << current_node->forward_[current_level]->pair_.first;
                    } else {
                        out << std::setw(5) << "o";
                    }
                } else {
                    out << std::setw(5) << ".";
                }
            }
            out << " [" << std::setw(5) << current_node->pair_.first << "]";
            out << std::endl;
        }
    }
};

#endif // SKIPLIST_H
