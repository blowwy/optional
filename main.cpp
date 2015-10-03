#include "optional.h"
#include <cassert>

template struct optional<int>;

struct x
{
    x(int, int)
    {}
};

struct scalar
{
    static int instances;

    scalar()
        : value()
    {
        ++instances;
    }

    scalar(int value)
        : value(value)
    {
        ++instances;
    }

    scalar(scalar const& other)
        : value(other.value)
    {
        ++instances;
    }

    ~scalar()
    {
        --instances;
    }

    friend bool operator==(scalar a, scalar b)
    {
        return a.value == b.value;
    }

    friend bool operator!=(scalar a, scalar b)
    {
        return a.value != b.value;
    }

private:
    int value;
};

int scalar::instances;

int main()
{
    optional<x> aa = make_optional<x>(5, 6);

    optional<scalar> bb;
    assert(!bb);
    bb = scalar{5};
    assert(bb);
    assert(*bb == 5);
    bb = scalar{7};
    assert(*bb == 7);
    bb = none;
    assert(!bb);
    bb = bb;
    assert(!bb);
    bb.emplace(10);
    assert(bb);
    assert(*bb == 10);

    return 0;
}

