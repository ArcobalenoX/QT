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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Object.h"

namespace DTLib
{

#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__))

class Exception : public Object
{
protected:
    char* m_message;
    char* m_location;

    void init(const char* message, const char* file, int line);
public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int line);

    Exception(const Exception& e);
    Exception& operator= (const Exception& e);

    virtual const char* message() const;
    virtual const char* location() const;

    virtual ~Exception() = 0;
};

class ArithmeticException : public Exception
{
public:
    ArithmeticException() : Exception(0) { }
    ArithmeticException(const char* message) : Exception(message) { }
    ArithmeticException(const char* file, int line) : Exception(file, line) { }
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    ArithmeticException(const ArithmeticException& e) : Exception(e) { }

    ArithmeticException& operator= (const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NullPointerException : public Exception
{
public:
    NullPointerException() : Exception(0) { }
    NullPointerException(const char* message) : Exception(message) { }
    NullPointerException(const char* file, int line) : Exception(file, line) { }
    NullPointerException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    NullPointerException(const NullPointerException& e) : Exception(e) { }

    NullPointerException& operator= (const NullPointerException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException() : Exception(0) { }
    IndexOutOfBoundsException(const char* message) : Exception(message) { }
    IndexOutOfBoundsException(const char* file, int line) : Exception(file, line) { }
    IndexOutOfBoundsException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { }

    IndexOutOfBoundsException& operator= (const IndexOutOfBoundsException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NoEnoughMemoryException : public Exception
{
public:
    NoEnoughMemoryException() : Exception(0) { }
    NoEnoughMemoryException(const char* message) : Exception(message) { }
    NoEnoughMemoryException(const char* file, int line) : Exception(file, line) { }
    NoEnoughMemoryException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e) { }

    NoEnoughMemoryException& operator= (const NoEnoughMemoryException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class InvalidParameterException : public Exception
{
public:
    InvalidParameterException() : Exception(0) { }
    InvalidParameterException(const char* message) : Exception(message) { }
    InvalidParameterException(const char* file, int line) : Exception(file, line) { }
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    InvalidParameterException(const InvalidParameterException& e) : Exception(e) { }

    InvalidParameterException& operator= (const InvalidParameterException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class InvalidOperationException : public Exception
{
public:
    InvalidOperationException() : Exception(0) { }
    InvalidOperationException(const char* message) : Exception(message) { }
    InvalidOperationException(const char* file, int line) : Exception(file, line) { }
    InvalidOperationException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    InvalidOperationException(const InvalidOperationException& e) : Exception(e) { }

    InvalidOperationException& operator= (const InvalidOperationException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

}


#endif // EXCEPTION_H
