/*
 * Copyright (C) 2013-2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <assert.h>
#include <utility>

#define RETURNS_NONNULL __attribute__((returns_nonnull))
#define WARN_UNUSED_RETURN __attribute__((__warn_unused_result__))

#define ASSERT assert

#if 0
#include <wtf/Assertions.h>
#include <wtf/DumbPtrTraits.h>
#include <wtf/Forward.h>
#include <wtf/GetPtr.h>
#include <wtf/StdLibExtras.h>
#include <wtf/TypeCasts.h>
#endif

namespace WTF {

inline void adopted(const void*) { }

template<typename T> class Ref;
template<typename T> Ref<T> adoptRef(T&);

template<typename T>
class Ref {
public:
    ~Ref()
    {
        if (m_ptr)
            m_ptr->deref();
    }

    Ref(T& object)
        : m_ptr(&object)
    {
        object.ref();
    }

    // Use copyRef() instead.
    Ref(const Ref& other) = delete;
    template<typename X> Ref(const Ref<X>& other) = delete;

    Ref(Ref&& other)
        : m_ptr(&other.leakRef())
    {
        ASSERT(m_ptr);
    }

    template<typename X>
    Ref(Ref<X>&& other)
        : m_ptr(&other.leakRef())
    {
        ASSERT(m_ptr);
    }

    Ref& operator=(T&);
    Ref& operator=(Ref&&);
    template<typename X> Ref& operator=(Ref<X>&&);

    // Use copyRef() and the move assignment operators instead.
    Ref& operator=(const Ref&) = delete;
    template<typename X> Ref& operator=(const Ref<X>&) = delete;

    template<typename X> void swap(Ref<X>&);

    T* operator->() const { ASSERT(m_ptr); return m_ptr; }
    T* ptr() const RETURNS_NONNULL { ASSERT(m_ptr); return m_ptr; }
    T& get() const { ASSERT(m_ptr); return *m_ptr; }
    operator T&() const { ASSERT(m_ptr); return *m_ptr; }
    bool operator!() const { ASSERT(m_ptr); return !*m_ptr; }

    template<typename X> Ref<T> replace(Ref<X>&&) WARN_UNUSED_RETURN;

    Ref copyRef() && = delete;
    Ref copyRef() const & WARN_UNUSED_RETURN { return Ref(*m_ptr); }

    T& leakRef() WARN_UNUSED_RETURN
    {
        ASSERT(m_ptr);

        T& result = *std::exchange(m_ptr, nullptr);
        return result;
    }

private:
    friend Ref adoptRef<T>(T&);
    template<typename X> friend class Ref;

    enum AdoptTag { Adopt };
    Ref(T& object, AdoptTag)
        : m_ptr(&object)
    {
    }

    T* m_ptr;
};

template<typename T> Ref<T> adoptRef(T&);
template<typename T> Ref<T> makeRef(T&);

template<typename T>
inline Ref<T>& Ref<T>::operator=(T& reference)
{
    Ref copiedReference = reference;
    swap(copiedReference);
    return *this;
}

template<typename T>
inline Ref<T>& Ref<T>::operator=(Ref&& reference)
{
    Ref movedReference = WTFMove(reference);
    swap(movedReference);
    return *this;
}

template<typename T>
template<typename X>
inline Ref<T>& Ref<T>::operator=(Ref<X>&& reference)
{
    Ref movedReference = WTFMove(reference);
    swap(movedReference);
    return *this;
}

template<typename T>
template<typename X>
inline void Ref<T>::swap(Ref<X>& other)
{
    std::swap(m_ptr, other.m_ptr);
}

template<typename T>
template<typename X>
inline Ref<T> Ref<T>::replace(Ref<X>&& reference)
{
    auto oldReference = adoptRef(*m_ptr);
    m_ptr = &reference.leakRef();
    return oldReference;
}

template<typename T, typename X>
inline Ref<T> static_reference_cast(Ref<X>& reference)
{
    return Ref<T>(static_cast<T&>(reference.get()));
}

template<typename T, typename X>
inline Ref<T> static_reference_cast(Ref<X>&& reference)
{
    return adoptRef(static_cast<T&>(reference.leakRef()));
}

template<typename T, typename X>
inline Ref<T> static_reference_cast(const Ref<X>& reference)
{
    return Ref<T>(static_cast<T&>(reference.copyRef().get()));
}

template<typename T>
inline Ref<T> adoptRef(T& reference)
{
    adopted(&reference);
    return Ref<T>(reference, Ref<T>::Adopt);
}

template<typename T>
inline Ref<T> makeRef(T& reference)
{
    return Ref<T>(reference);
}

template<typename ExpectedType, typename ArgType>
inline bool is(Ref<ArgType>& source)
{
    return is<ExpectedType>(source.get());
}

template<typename ExpectedType, typename ArgType>
inline bool is(const Ref<ArgType>& source)
{
    return is<ExpectedType>(source.get());
}

} // namespace WTF

using WTF::Ref;
using WTF::adoptRef;
using WTF::makeRef;
using WTF::static_reference_cast;
