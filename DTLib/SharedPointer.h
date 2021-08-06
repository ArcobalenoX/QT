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

#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if( this->m_ref )
        {
            (*this->m_ref)++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = static_cast<int*>(std::malloc(sizeof(int)));

            if( this->m_ref )
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create SharedPointer object ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
    }

    SharedPointer<T>& operator= (const SharedPointer<T>& obj)
    {
        if( this != &obj )
        {
            clear();
            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if( ref )
        {
            (*ref)--;

            if( *ref == 0 )
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

template < typename T >
bool operator == (const T* l, const SharedPointer<T>& r)
{
    return (l == r.get());
}

template < typename T >
bool operator == (const SharedPointer<T>& l, const T* r)
{
    return (l.get() == r.get());
}

template < typename T >
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());
}

template < typename T >
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
}
}

#endif // SHAREDPOINTER_H
