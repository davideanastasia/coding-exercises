#ifndef SIMPLEHASH_H
#define SIMPLEHASH_H

#include <cstddef>
#include <cassert>

//! \brief Hash container implemented using one array only
class SimpleHash
{
public:
    SimpleHash(size_t size)
        : m_size(size)
        , m_data( new SimpleHashElem[size] )
    {}

    ~SimpleHash()
    {
        delete [] m_data;
    }

    //! \brief Gets the value for the key \c key
    //! \return true if the key is present, false otherwise
    bool get(int key, int& value)
    {
        assert(key < m_size);

        // Scan vector from
        for (size_t idx = key; idx < m_size; ++idx) {
            if ( m_data[idx].busy_ && (m_data[idx].key_ == key) )
            {
                value = m_data[idx].value_;
                return true;
            }
        }

        for (size_t idx = 0; idx < key; ++idx) {
            if ( m_data[idx].busy_ && (m_data[idx].key_ == key) )
            {
                value = m_data[idx].value_;
                return true;
            }
        }

        return false;
    }

    bool insert(int key, int value)
    {
        assert(key < m_size);
        for (size_t idx = key; idx < m_size; ++idx) {
            if ( !m_data[idx].busy_ ||
                 (m_data[idx].busy_ && (m_data[idx].key_ == key)) )
            {
                m_data[idx].key_ = key;
                m_data[idx].value_ = value;
                m_data[idx].busy_ = true;

                return true;
            }
        }

        for (size_t idx = 0; idx < key; ++idx) {
            if ( !m_data[idx].busy_ ||
                 (m_data[idx].busy_ && (m_data[idx].key_ == key)) )
            {
                m_data[idx].key_ = key;
                m_data[idx].value_ = value;
                m_data[idx].busy_ = true;

                return true;
            }
        }

        // I could not find a location available!!!
        return false;
    }

private:
    struct SimpleHashElem
    {
        SimpleHashElem()
            : busy_(false)
            , key_()
            , value_()
        {}

        bool busy_;
        int key_;
        int value_;
    };

    size_t m_size;
    SimpleHashElem* m_data;
};

#endif // #ifndef SIMPLEHASH_H
