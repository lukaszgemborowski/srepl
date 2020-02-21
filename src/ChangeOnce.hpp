#ifndef SREPL_CHANGEONCE_HPP
#define SREPL_CHANGEONCE_HPP

#include <functional>

template<class T>
struct ChangeOnceExecute;

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

    ChangeOnceExecute<T> on(const T &value, std::function<void ()> call)
    {
        return {*this, value, call};
    }

    operator T() const
    {
        return value_;
    }

private:
    T       value_;
    bool    changed_;
};

template<class T>
struct ChangeOnceExecute
{
public:
    ChangeOnceExecute(const ChangeOnce<T> &once, const T& expected, std::function<void ()> call)
        : once_ {once}
        , expected_ {expected}
        , call_ {call}
    {}

    ChangeOnceExecute& operator=(const T& value)
    {
        once_ = value;
        return *this;
    }

    operator T() const { return once_; }

    ~ChangeOnceExecute()
    {
        if (expected_ == once_)
            call_();
    }

private:
    ChangeOnce<T> once_;
    T expected_;
    std::function<void ()> call_;
};

#endif // SREPL_CHANGEONCE_HPP
