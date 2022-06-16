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
 
#ifndef TREENODE_H
#define TREENODE_H

#include "Object.h"

namespace DTLib
{

template < typename T >
class TreeNode : public Object
{
protected:
    bool m_flag;

    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);

    void* operator new(size_new size) throw()
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T>* parent;

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template < typename T >
TreeNode<T>::~TreeNode()
{

}

}

#endif // TREENODE_H
