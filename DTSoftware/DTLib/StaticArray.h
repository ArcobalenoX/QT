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

#ifndef STATICARRAY_H
#define STATICARRAY_H

#include "Array.h"

namespace DTLib
{

template < typename T, int N >
class StaticArray : public Array<T>
{
protected:
    T m_space[N];
public:
    StaticArray()   // O(1)
    {
        this->m_array = m_space;
    }

    StaticArray(const StaticArray<T, N>& obj)    // O(n)
    {
        this->m_array = m_space;

        for(int i=0; i<N; i++)
        {
            m_space[i] = obj.m_space[i];
        }
    }

    StaticArray<T, N>& operator= (const StaticArray<T, N>& obj)   // O(n)
    {
        if( this != &obj )
        {
            for(int i=0; i<N; i++)
            {
                m_space[i] = obj.m_space[i];
            }
        }

        return *this;
    }

    int length() const     // O(1)
    {
        return N;
    }
};


}

#endif // STATICARRAY_H
