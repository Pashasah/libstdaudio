// libstdaudio
// Copyright (c) 2018 - Timur Doumler

#pragma once
#include <vector>
#include <iterator>
#include <__audio_config>

_LIBSTDAUDIO_NAMESPACE_BEGIN

template <typename T>
class _strided_dynamic_extent_span {
public:
  using element_type = T;
  using pointer = T*;
  using reference = T&;
  using size_type = size_t;
  using index_type = ptrdiff_t;
  using difference_type = ptrdiff_t;

  _strided_dynamic_extent_span() = default;

  _strided_dynamic_extent_span(pointer data, size_type size, difference_type stride)
    : _data(data), _size(size), _stride(stride) {
    assert(stride > 0);
  }

  pointer data() const noexcept {
    return _data;
  }

  size_type size() const noexcept {
    return _size / _stride;
  }

  bool empty() const noexcept {
    return _size == 0;
  }

  difference_type stride() const noexcept {
    return _stride;
  }

  class iterator : public std::iterator<random_access_iterator_tag, T, difference_type, pointer, reference> {
  private:
    using _super = std::iterator<random_access_iterator_tag, T>;

  public:
    // TODO: why is this needed?
    using typename _super::iterator_category;
    using typename _super::value_type;
    using typename _super::difference_type;
    using typename _super::pointer;
    using typename _super::reference;

    iterator(pointer ptr, difference_type stride)
      : _ptr(ptr), _stride(stride) {
    }

    reference operator*() const {
      return *_ptr;
    }

    pointer operator->() const noexcept {
      return _ptr;
    }

    reference operator[](index_type i) {
      return _ptr[_stride * i];
    }

    iterator& operator++() noexcept {
      _ptr += _stride;
      return *this;
    }

    // TODO: why does clang-tidy give a warning here?
    iterator operator++(int) noexcept {
      auto retval = *this;
      ++(*this);
      return retval;
    }

    iterator& operator--() noexcept {
      _ptr -= _stride;
      return *this;
    }

    // TODO: why does clang-tidy give a warning here?
    iterator operator--(int) noexcept {
      auto retval = *this;
      --(*this);
      return retval;
    }

    iterator operator+(difference_type dist) {
      iterator retval = *this;
      retval += dist;
      return retval;
    }

    iterator operator-(difference_type dist) {
      iterator retval = *this;
      retval -= dist;
      return retval;
    }

    iterator& operator+=(difference_type dist) {
      _ptr += (_stride * dist);
      return *this;
    }

    iterator& operator-=(difference_type dist) {
      _ptr -= (_stride * dist);
      return *this;
    }

    bool operator==(const iterator& other) const noexcept {
      return _ptr == other._ptr;
    }

    bool operator!=(const iterator& other) const noexcept {
      return !(*this == other);
    }

    bool operator<(const iterator& other) const noexcept {
      return _ptr < other._ptr;
    }

    bool operator<=(const iterator& other) const noexcept {
      return (*this == other) || (*this < other);
    }

    bool operator>(const iterator& other) const noexcept {
      return (*this != other) && !(*this < other);
    }

    bool operator>=(const iterator& other) const noexcept {
      return !(*this < other);
    }

  private:
    pointer _ptr;
    index_type _stride;
  };

  iterator begin() const noexcept {
    return {_data, _stride};
  }

  iterator end() const noexcept {
    return {_data + _size, _stride};
  }

  reference operator[](index_type i) const {
    return _data[_stride * i];
  }

private:
  pointer _data = nullptr;
  size_type _size = 0;
  difference_type _stride = 1;
};

_LIBSTDAUDIO_NAMESPACE_END