#ifndef LIBASD_LINE_PROXY_H
#define LIBASD_LINE_PROXY_H
#include <type_traits>
#include <iterator>

namespace asd
{

namespace detail
{
template<bool IsConst, typename originT>
struct LineProxyIterator;

template<bool IsConst, typename originT>
struct LineProxy
{
    static constexpr bool is_constant = IsConst;
    friend LineProxyIterator<is_constant, originT>;

    typedef originT  origin_type;
    typedef typename origin_type::raw_iterator               origin_iterator;
    typedef typename origin_type::raw_const_iterator         origin_const_iterator;
    typedef typename origin_type::raw_reverse_iterator       origin_reverse_iterator;
    typedef typename origin_type::raw_const_reverse_iterator origin_const_reverse_iterator;

    typedef typename std::conditional<is_constant,
            origin_const_iterator, origin_iterator>::type iterator;
    typedef origin_const_iterator const_iterator;
    typedef typename std::conditional<is_constant,
            origin_const_reverse_iterator, origin_reverse_iterator
            >::type reverse_iterator;
    typedef origin_const_reverse_iterator const_reverse_iterator;

    typedef typename std::iterator_traits<iterator> iterator_tratis_type;

    static_assert(std::is_same<typename iterator_tratis_type::iterator_category,
            std::random_access_iterator_tag>::value,
            "container should be randomly accessible");

    typedef typename iterator_tratis_type::difference_type difference_type;
    typedef typename iterator_tratis_type::value_type      value_type;
    typedef typename iterator_tratis_type::pointer         pointer;
    typedef typename iterator_tratis_type::reference       reference;

    LineProxy(iterator b, iterator e,
              std::size_t l, std::size_t x, std::size_t y) noexcept
        : begin_(b), end_(e), line(l), x_pixel(x), y_pixel(y)
    {}

    reference operator[](const std::size_t i) const noexcept
    {
        return *(this->begin_ + i);
    }

    reference at(const std::size_t i) const
    {
        if(i >= x_pixel)
        {
            throw_exception<std::out_of_range>("libasd: Frame::LineProxy::at: "
                    "line(%): index(%) exceeds x_pixel(%)",
                    this->line, i, this->x_pixel);
        }
        return *(this->begin_ + i);
    }

    std::size_t size()       const noexcept {return x_pixel;}
    std::size_t max_size()   const noexcept {return x_pixel;}
    bool        empty()      const noexcept {return false;}
    std::size_t y_position() const noexcept {return line;}

    iterator begin() const noexcept {return begin_;}
    iterator end()   const noexcept {return end_;}
    const_iterator cbegin() const noexcept {return begin_;}
    const_iterator cend()   const noexcept {return end_;}

    reverse_iterator rbegin() const noexcept {return reverse_iterator(end_);}
    reverse_iterator rend()   const noexcept {return reverse_iterator(begin_);}

    const_reverse_iterator crbegin() const noexcept
    {return const_reverse_iterator(end_);}
    const_reverse_iterator crend()   const noexcept
    {return const_reverse_iterator(begin_);}

    bool operator==(LineProxy const& rhs) const noexcept
    {
        return this->begin_  == rhs.begin_  && this->end_ == rhs.end_ &&
               this->line == rhs.line &&
               this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel;
    }
    bool operator!=(LineProxy const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    bool operator<(LineProxy const& rhs) const noexcept
    {
        return this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel &&
               this->line    <  rhs.line    && this->begin_  <  rhs.begin_;
    }
    bool operator<=(LineProxy const& rhs) const noexcept
    {
        return (*this == rhs) || *this < rhs;
    }

    bool operator>(LineProxy const& rhs) const noexcept
    {
        return this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel &&
               this->line    >  rhs.line    && this->begin_  >  rhs.begin_;
    }
    bool operator>=(LineProxy const& rhs) const noexcept
    {
        return (*this == rhs) || *this > rhs;
    }

  private:
    iterator begin_, end_;
    std::size_t line;
    std::size_t x_pixel;
    std::size_t y_pixel;
};

template<bool IsConst, typename originT>
struct LineProxyIterator
{
    static constexpr bool is_constant = IsConst;
    typedef LineProxy<is_constant, originT> proxy_type;
    typedef proxy_type             value_type;
    typedef value_type const*      pointer;
    typedef value_type const&      reference;
    typedef std::ptrdiff_t         difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    LineProxyIterator(const proxy_type& p) : proxy(p) {}
    LineProxyIterator(proxy_type&& p) : proxy(std::move(p)) {}

    reference operator* () const noexcept {return this->proxy;}
    pointer   operator->() const noexcept {return std::addressof(this->proxy);}

    LineProxyIterator& operator++() noexcept
    {
        this->proxy.line   += 1;
        this->proxy.begin_ += this->proxy.x_pixel;
        this->proxy.end_   += this->proxy.x_pixel;
    }
    LineProxyIterator& operator--() noexcept
    {
        this->proxy.line   -= 1;
        this->proxy.begin_ -= this->proxy.x_pixel;
        this->proxy.end_   -= this->proxy.x_pixel;
    }
    LineProxyIterator  operator++(int) noexcept
    {
        const auto tmp(*this); ++(*this); return tmp;
    }
    LineProxyIterator  operator--(int) noexcept
    {
        const auto tmp(*this); --(*this); return tmp;
    }

    LineProxyIterator& operator+=(const difference_type d) noexcept
    {
        this->proxy.line  += d;
        this->proxy.begin += this->proxy.x_pixel * d;
        this->proxy.end   += this->proxy.x_pixel * d;
    }
    LineProxyIterator& operator-=(const difference_type d) noexcept
    {
        this->proxy.line  -= d;
        this->proxy.begin -= this->proxy.x_pixel * d;
        this->proxy.end   -= this->proxy.x_pixel * d;
    }

    bool operator==(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy == rhs.proxy;
    }
    bool operator!=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy != rhs.proxy;
    }

    bool operator<=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy <= rhs.proxy;
    }
    bool operator<(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy < rhs.proxy;
    }

    bool operator>=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy >= rhs.proxy;
    }
    bool operator>(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy > rhs.proxy;
    }

  private:

    proxy_type proxy;
};

} // detail
} // asd
#endif// LIBASD_LINE_PROXY_H