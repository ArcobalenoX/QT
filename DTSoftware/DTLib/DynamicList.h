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

#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;    // 顺序存储空间的大小
public:
    DynamicList(int capacity) // 申请空间
    {
        this->m_array = new T[capacity];

        if( this->m_array != NULL )
        {
            this->m_length = 0;
            this->m_capacity = capacity;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicList object ...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    /* 重新设置顺序存储空间的大小 */
    void resize(int capacity)
    {
        if( capacity != m_capacity )
        {
            T* array = new T[capacity];

            if( array != NULL )
            {
                int length = (this->m_length < capacity ? this->m_length : capacity);

                for(int i=0; i<length; i++)
                {
                    array[i] = this->m_array[i];
                }

                T* temp = this->m_array;

                this->m_array = array;
                this->m_length = length;
                this->m_capacity = capacity;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize DynamicList object ...");
            }
        }
    }

    ~DynamicList()
    {
        delete[] this->m_array;
    }
};


}

#endif // DYNAMICLIST_H
