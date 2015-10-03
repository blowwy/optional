#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <cassert>
#include <memory>
#include <type_traits>

static const struct none_t
{} none = none_t();

template <typename T>
struct optional
{
    optional()
        : initialized(false)
    {}

    optional(optional const& other)
        : initialized(other.initialized)
    {
        if (initialized)
            new (&storage) T (*other);
    }

    optional(T const& value)
        : initialized(true)
    {
        new (&storage) T (value);
    }

    optional(none_t)
        : initialized(false)
    {}

    ~optional()
    {
        if (initialized)
            (**this).T::~T();
    }

    optional& operator=(optional rhs)
    {
        swap(rhs);
        return *this;
    }

    template <typename ... Args>
    void emplace(Args&& ...args)
    {
        if (initialized)
        {
            data_ptr()->T::~T();
            initialized = false;
        }
        new (&storage) T(std::forward<Args>(args)...);
        initialized = true;
    }

    void swap(optional& other)
    {
        if (initialized)
        {
            if (other.initialized)
            {
                std::swap(**this, *other);
            }
            else
            {
                new (&other.storage) T(**this);
                data_ptr()->T::~T();
                other.initialized = true;
                initialized = false;
            }
        }
        else
        {
            if (other.initialized)
            {
                new (&storage) T(*other);
                other.data_ptr()->T::~T();
                other.initialized = false;
                initialized = true;
            }
        }
    }

    explicit operator bool() const
    {
        return initialized;
    }

    T& operator*()
    {
        assert(initialized);
        return *data_ptr();
    }

    T const& operator*() const
    {
        assert(initialized);
        return *data_ptr();
    }

    T* operator->()
    {
        assert(initialized);
        return data_ptr();
    }

    T const* operator->() const
    {
        assert(initialized);
        return data_ptr();
    }

private:
    T* data_ptr()
    {
        return reinterpret_cast<T*>(&storage);
    }

    T const* data_ptr() const
    {
        return reinterpret_cast<T const*>(&storage);
    }

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
    bool initialized;

    template <typename U, typename ... Args>
    friend optional<U> make_optional(Args&& ...args);

    friend bool operator<(optional const& a, optional const& b)
    {
        if (a)
        {
            if (b)
                return *a < *b;
            else
                return false;
        }
        else
        {
            if (b)
                return true;
            else
                return false;
        }
    }

    friend bool operator>(optional const& a, optional const& b)
    {
        return b < a;
    }

    friend bool operator==(optional const& a, optional const& b)
    {
        if (a.initialized != b.initialized)
            return false;

        if (!a.initialized)
            return true;

        return *a == *b;
    }

    friend bool operator!=(optional const& a, optional const& b)
    {
        return !(a == b);
    }

    friend bool operator<=(optional const& a, optional const& b)
    {
        return a < b || a == b;
    }

    friend bool operator>=(optional const& a, optional const& b)
    {
        return a > b || a == b;
    }
};

template <typename T, typename ... Args>
optional<T> make_optional(Args&& ...args)
{
    optional<T> result;
    result.emplace(std::forward<Args>(args)...);
    return result;
}

#endif // OPTIONAL_H
