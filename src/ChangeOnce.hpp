#ifndef SREPL_CHANGEONCE_HPP
#define SREPL_CHANGEONCE_HPP

#include <functional>

template<class T>
struct ChangeOnce
{
    explicit ChangeOnce(const T& init)
        : value_ {init}
        , changed_ {false}
    {}

    ChangeOnce& operator=(const T& newValue)
    {
        if (!changed_) {
            if (newValue != value_) {
                changed_ = true;
                value_ = newValue;
            }
        }

        return *this;
    }

    operator T() const
    {
        return value_;
    }

private:
    T value_;
    bool changed_;
};

#endif // SREPL_CHANGEONCE_HPP
