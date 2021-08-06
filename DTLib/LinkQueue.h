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

#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include "Queue.h"
#include "LinuxList.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class LinkQueue : public Queue<T>
{
protected:
    struct Node : public Object
    {
        list_head head;
        T value;
    };

    list_head m_header;
    int m_length;
public:
    LinkQueue()   // O(1)
    {
        m_length = 0;

        INIT_LIST_HEAD(&m_header);
    }

    void add(const T& e)     // O(1)
    {
        Node* node = new Node();

        if( node != NULL )
        {
            node->value = e;

            list_add_tail(&node->head, &m_header);   // O(1)

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No memory to add new element ...");
        }
    }

    void remove()      // O(1)
    {
        if( m_length > 0 )
        {
            list_head* toDel = m_header.next;

            list_del(toDel);

            m_length--;

            delete list_entry(toDel, Node, head);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    T front() const    // O(1)
    {
        if( m_length > 0 )
        {
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    void clear()    // O(n)
    {
        while( m_length > 0 )
        {
            remove();
        }
    }

    int length() const    // O(1)
    {
        return m_length;
    }


    ~LinkQueue()   // O(n)
    {
        clear();
    }
};

}

#endif // LINKQUEUE_H
