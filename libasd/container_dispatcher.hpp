#ifndef LIBASD_CONTAINER_DISPATCHER_H
#define LIBASD_CONTAINER_DISPATCHER_H
#include <vector>
#include <deque>
#include <array>
#include <libasd/exception_thrower.hpp>

namespace asd
{
namespace container
{

// ---------------------------------- vector ---------------------------------//

struct vec
{
    template<typename T>
    struct rebind
    {
        typedef std::vector<T, std::allocator<T>> other;
    };
};
template<typename T, typename Alloc>
inline T const* get_ptr(const std::vector<T, Alloc>& v) noexcept
{
    return v.data();
}
template<typename T, typename Alloc>
inline std::size_t size(const std::vector<T, Alloc>& v) noexcept
{
    return v.size();
}
template<typename T, typename Alloc>
inline void resize(std::vector<T, Alloc>& v, const std::size_t N)
{
    return v.resize(N);
}
template<typename T, typename Alloc>
inline void clear(std::vector<T, Alloc>& v)
{
    return v.clear();
}

// ---------------------------------- deque ----------------------------------//

struct deq
{
    template<typename T>
    struct rebind
    {
        typedef std::deque<T, std::allocator<T>> other;
    };
};
template<typename T, typename Alloc>
inline std::size_t size(const std::deque<T, Alloc>& v) noexcept
{
    return v.size();
}
template<typename T, typename Alloc>
inline void resize(std::deque<T, Alloc>& v, const std::size_t N)
{
    return v.resize(N);
}
template<typename T, typename Alloc>
inline void clear(std::deque<T, Alloc>& v)
{
    return v.clear();
}



// ---------------------------------- array ----------------------------------//

template<std::size_t N>
struct arr
{
    template<typename T>
    struct rebind
    {
        typedef std::array<T, N> other;
    };
};

template<typename T, std::size_t N>
inline T const* get_ptr(const std::array<T, N>& v) noexcept
{
    return v.data();
}

template<typename T, std::size_t N>
inline void resize(std::array<T, N>& v, const std::size_t M)
{
    if(v.size() < M)
    {
        throw_exception<std::bad_alloc>(
                "array(size = %) has no enough storage(%)", v.size(), M);
    }
    return;
}

template<typename T, std::size_t N>
inline void clear(std::array<T, N>& v)
{
    // do nothing
    return;
}

template<typename T, std::size_t N>
inline constexpr std::size_t size(const std::array<T, N>& v) noexcept
{
    return v.size();
}

} // container
} // asd
#endif//LIBASD_CONTAINER_DISPATCHER_H