/*
 * This file is part of the DTLib template project, http://www.dt4sw.com
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 唐佐林 (Delphi Tang)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class SeqList : public List<T>
{
protected:
    T* m_array;
    int m_length;
public:
    bool insert(int i, const T& e)                 // n + 5 => O(n+5) => O(n)
    {
        bool ret = ((0 <= i) && (i <= m_length));  // 1

        ret = ret && (m_length < capacity());  // 1

        if( ret )
        {
            for(int p=m_length-1; p>=i; p--)   // n, 0
            {
                m_array[p + 1] = m_array[p];
            }

            m_array[i] = e;                    // 1

            m_length++;                        // 1
        }

        return ret;                            // 1
    }

    bool insert(const T& e)         // O(n)
    {
        return insert(m_length, e);
    }

    bool remove(int i)   // O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            for(int p=i; p<m_length-1; p++)   // n - 1
            {
                m_array[p] = m_array[p+1];
            }

            m_length--;
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            m_array[i] = e;
        }

        return ret;
    }

    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            e = m_array[i];
        }

        return ret;
    }

    int find(const T& e) const    // O(n)
    {
        int ret = -1;

        for(int i=0; i<m_length; i++)
        {
            if( m_array[i] == e )
            {
                ret = i;
                break;
            }
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        m_length = 0;
    }

    // 顺序存储线性表的数组访问方式
    T& operator[] (int i)
    {
        if( (0 <= i) && (i < m_length) )
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }
    }

    T operator[] (int i) const
    {
        return (const_cast<SeqList<T>&>(*this))[i];
    }

    // 顺序存储空间的容量
    virtual int capacity() const = 0;
};


}

#endif // SEQLIST_H
