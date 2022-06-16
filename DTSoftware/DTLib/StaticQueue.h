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

#ifndef STATICQUEUE_H
#define STATICQUEUE_H

#include "Queue.h"
#include "Exception.h"

namespace DTLib
{

template < typename T, int N >
class StaticQueue : public Queue<T>
{
protected:
    T m_space[N];
    int m_front;
    int m_rear;
    int m_length;
public:
    StaticQueue()
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }

    int capacity() const   // O(1)
    {
        return N;
    }

    void add(const T& e)    // O(1)
    {
        if( m_length < N )
        {
            m_space[m_rear] = e;
            m_rear = (m_rear + 1) % N;
            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in current queue ...");
        }
    }

    void remove()    // O(1)
    {
        if( m_length > 0 )
        {
            m_front = (m_front + 1) % N;
            m_length--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    T front() const   //  O(1)
    {
        if( m_length > 0 )
        {
            return m_space[m_front];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    void clear()   // O(1)
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }

    int length() const   // O(1)
    {
        return m_length;
    }
};

}

#endif // STATICQUEUE_H
