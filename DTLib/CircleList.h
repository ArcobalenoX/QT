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

#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace DTLib
{

template < typename T >
class CircleList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;

    int mod(int i) const   // O(1)
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
    }

    Node* last() const    // O(n)
    {
        return this->position(this->m_length-1)->next;
    }

    void last_to_first() const   // O(n)
    {
        last()->next = this->m_header.next;
    }

public:
    bool insert(const T& e)      // O(n)
    {
        return insert(this->m_length, e);
    }

    bool insert(int i, const T& e)        // O(n)
    {
        bool ret = true;

        i = i % (this->m_length + 1);     // O(1)

        ret = LinkList<T>::insert(i, e);  // O(n)

        if( ret && (i == 0) )
        {
            last_to_first();              // O(n)
        }

        return ret;
    }

    bool remove(int i)   // O(n)
    {
        bool ret = true;

        i = mod(i);

        if( i == 0 )
        {
            Node* toDel = this->m_header.next;

            if( toDel != NULL )
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if( this->m_length > 0 )
                {
                    last_to_first();

                    if( this->m_current == toDel )
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }

                this->destroy(toDel);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);   // O(n)
        }

        return ret;
    }

    bool set(int i, const T& e)  // O(n)
    {
        return LinkList<T>::set(mod(i), e);
    }

    T get(int i) const           // O(n)
    {
        return LinkList<T>::get(mod(i));
    }

    bool get(int i, T& e) const  // O(n)
    {
        return LinkList<T>::get(mod(i), e);
    }

    int find(const T& e) const    // O(n)
    {
        int ret = -1;
        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i++)
        {
            if( slider->value == e )
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        return ret;
    }

    void clear()    // O(n)
    {
        while( this->m_length > 1 )   // O(n)
        {
            remove(1);   // O(1)
        }

        if( this->m_length == 1 )     // O(1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step)   // O(n)
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end()   // O(1)
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }

    ~CircleList()   // O(n)
    {
        clear();
    }
};

}


#endif // CIRCLELIST_H
