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

#ifndef ARRAY_H
#define ARRAY_H

#include "Object.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class Array : public Object
{
protected:
    T* m_array;
public:
    virtual bool set(int i, const T& e) // O(1)
    {
        bool ret = ((0 <= i) && (i < length()));

        if( ret )
        {
            m_array[i] = e;
        }

        return ret;
    }

    virtual bool get(int i, T& e) const   // O(1)
    {
        bool ret = ((0 <= i) && (i < length()));

        if( ret )
        {
            e = m_array[i];
        }

        return ret;
    }

    T& operator[] (int i)   // O(1)
    {
        if((0 <= i) && (i < length()))
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }
    }

    T operator[] (int i) const  // O(1)
    {
        return (const_cast<Array<T>&>(*this))[i];
    }

    T* array() const
    {
        return m_array;
    }

    virtual int length() const = 0;
};

}

#endif // ARRAY_H
