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

#ifndef GTREE_H
#define GTREE_H

#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"

namespace DTLib
{

template < typename T >
class GTree : public Tree<T>
{
protected:
    LinkQueue<GTreeNode<T>*> m_queue;

    GTreeNode<T>* find(GTreeNode<T>* node, const T& value) const
    {
        GTreeNode<T>* ret = NULL;

        if( node != NULL )
        {
            if( node->value == value )
            {
                return node;
            }
            else
            {
                for(node->child.move(0); !node->child.end() && (ret == NULL); node->child.next())
                {
                    ret = find(node->child.current(), value);
                }
            }
        }

        return ret;
    }

    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = NULL;

        if( node == obj )
        {
            return node;
        }
        else
        {
            if( node != NULL )
            {
                for(node->child.move(0); !node->child.end() && (ret == NULL); node->child.next() )
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }

    void free(GTreeNode<T>* node)
    {
        if( node != NULL )
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                free(node->child.current());
            }

            if( node->flag() )
            {
                delete node;
            }
        }
    }

    void remove(GTreeNode<T>* node, GTree<T>*& ret)
    {
        ret = new GTree<T>();

        if( ret == NULL )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree ...");
        }
        else
        {
            if( root() == node )
            {
                this->m_root = NULL;
            }
            else
            {
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(node->parent)->child;

                child.remove(child.find(node));

                node->parent = NULL;
            }

            ret->m_root = node;
        }
    }

    int count(GTreeNode<T>* node) const
    {
        int ret = 0;

        if( node != NULL )
        {
            ret = 1;

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
            }
        }

        return ret;
    }

    int height(GTreeNode<T>* node) const
    {
        int ret = 0;

        if( node != NULL )
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int h = height(node->child.current());

                if( ret < h )
                {
                    ret = h;
                }
            }

            ret = ret + 1;
        }

        return ret;
    }

    int degree(GTreeNode<T>* node) const
    {
        int ret = 0;

        if( node != NULL )
        {
            ret = node->child.length();

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int d = degree(node->child.current());

                if( ret < d )
                {
                    ret = d;
                }
            }
        }

        return ret;
    }
public:
    GTree()
    {

    }

    bool insert(TreeNode<T>* node)
    {
        bool ret = true;

        if( node != NULL )
        {
            if( this->m_root == NULL )
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                GTreeNode<T>* np = find(node->parent);

                if( np != NULL )
                {
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);

                    if( np->child.find(n) < 0 )
                    {
                        np->child.insert(n);
                    }
                }
                else
                {
                    THROW_EXCEPTION(InvalidOperationException, "Invalid parent tree node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node cannot be NULL ...");
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;
        GTreeNode<T>* node = GTreeNode<T>::NewNode();

        if( node != NULL )
        {
            node->value = value;
            node->parent = parent;

            insert(node);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node ...");
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(const T& value)
    {
        GTree<T>* ret = NULL;
        GTreeNode<T>* node = find(value);

        if( node == NULL )
        {
            THROW_EXCEPTION(InvalidParameterException, "Can not find the node via parameter value ...");
        }
        else
        {
            remove(node, ret);

            m_queue.clear();
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        GTree<T>* ret = NULL;

        node = find(node);

        if( node == NULL )
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid ...");
        }
        else
        {
            remove(dynamic_cast<GTreeNode<T>*>(node), ret);

            m_queue.clear();
        }

        return ret;
    }

    GTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }

    GTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    GTreeNode<T>* root() const
    {
        return dynamic_cast<GTreeNode<T>*>(this->m_root);
    }

    int degree() const
    {
        return degree(root());
    }

    int count() const
    {
        return count(root());
    }

    int height() const
    {
        return height(root());
    }

    void clear()
    {
        free(root());

        this->m_root = NULL;

        m_queue.clear();
    }

    bool begin()
    {
        bool ret = (root() != NULL);

        if( ret )
        {
            m_queue.clear();
            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if( ret )
        {
            GTreeNode<T>* node = m_queue.front();

            m_queue.remove();

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                m_queue.add(node->child.current());
            }

        }

        return ret;
    }

    T current()
    {
        if( !end() )
        {
            return m_queue.front()->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }
    }

    ~GTree()
    {
        clear();
    }
};


}

#endif // GTREE_H
