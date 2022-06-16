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

#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "Exception.h"

namespace DTLib
{

template < typename T, int N >
class StaticStack : public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;
public:
    StaticStack()    // O(1)
    {
        m_top = -1;
        m_size = 0;
    }

    int capacity() const  // O(1)
    {
        return N;
    }

    void push(const T& e)  // O(1)
    {
        if( m_size < N )
        {
            m_space[m_top + 1] = e;
            m_top++;
            m_size++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in current stack ...");
        }
    }

    void pop()   // O(1)
    {
        if( m_size > 0 )
        {
            m_top--;
            m_size--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    T top() const   // O(1)
    {
        if( m_size > 0 )
        {
            return m_space[m_top];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    void clear()   // O(1)
    {
        m_top = -1;
        m_size = 0;
    }

    int size() const  // O(1)
    {
        return m_size;
    }
};

}

#endif // STATICSTACK_H
