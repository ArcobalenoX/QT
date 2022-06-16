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

#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Pointer.h"

namespace DTLib
{

template <typename T>
class SmartPointer : public Pointer<T>
{
public:
    SmartPointer(T* p = NULL) : Pointer<T>(p)
    {

    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;

        const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;
    }

    SmartPointer<T>& operator= (const SmartPointer<T>& obj)
    {
        if( this != &obj )
        {
            T* p = this->m_pointer;

            this->m_pointer = obj.m_pointer;

            const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;

            delete p;
        }

        return *this;
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
};

}

#endif // SMARTPOINTER_H
