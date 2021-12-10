
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#define DEF_STATIC_CAP 16
#include <iostream>
#include <iterator>

template<typename T, size_t StaticCapacity = DEF_STATIC_CAP>
class vl_vector {

 public:

  /** def ctr */
  vl_vector () : _size (0), _dynamic_cap (0), _dynamic_arr_p (nullptr),
                 _static_capacity (StaticCapacity)
  {}
  /** cpy ctr */
  vl_vector (const vl_vector<T, StaticCapacity> &other_vec)
      : _size (other_vec._size), _dynamic_cap (other_vec._dynamic_cap),
        _static_capacity (other_vec._static_capacity)
  {
    if (other_vec._dynamic_arr_p != nullptr)
      {
        _dynamic_arr_p = new T[_dynamic_cap];
        for (size_t i = 0; i < other_vec._size; ++i)
          _dynamic_arr_p[i] = other_vec._dynamic_arr_p[i];
      }
    else
      {
        for (size_t i = 0; i < other_vec._size; ++i)
          _static_arr[i] = other_vec._static_arr[i];
        _dynamic_arr_p = nullptr;
      }
  }
  /** sequence based ctr - construct the vector and adds all the sequence of elems */
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last)
      : _size (0), _dynamic_cap (0), _dynamic_arr_p (nullptr),
        _static_capacity (StaticCapacity)
  { insert (begin (), first, last); }
  /** Single-value init ctr */
  vl_vector (size_t count, const T &elem) : _size (0),
                                            _static_capacity (StaticCapacity)
  {
    if (count > _static_capacity)
      {
        size_t new_size = cap_func (count);
        _dynamic_cap = new_size;
        _dynamic_arr_p = new T[new_size];
        for (size_t i = 0; i < count; ++i)
          _dynamic_arr_p[i] = elem;
      }
    else
      {
        for (size_t i = 0; i < count; ++i)
          _static_arr[i] = elem;
        _dynamic_arr_p = nullptr;
        _dynamic_cap = 0;
      }
    _size = count;
  }
  /** destructor - virtual for the bonus VLString */
  virtual ~vl_vector ()
  { if (_dynamic_arr_p != nullptr) delete[] _dynamic_arr_p; }

  // iterators typedefs
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // definition of all iterators
  iterator begin ()
  { return _dynamic_arr_p != nullptr ? _dynamic_arr_p : _static_arr; }
  const_iterator begin () const
  { return _dynamic_arr_p != nullptr ? _dynamic_arr_p : _static_arr; }
  const_iterator cbegin () const
  { return _dynamic_arr_p != nullptr ? _dynamic_arr_p : _static_arr; }
  iterator end ()
  {
    return _dynamic_arr_p != nullptr ? _dynamic_arr_p + _size
                                     : _static_arr + _size;
  }
  const_iterator end () const
  {
    return _dynamic_arr_p != nullptr ? _dynamic_arr_p + _size
                                     : _static_arr + _size;
  }
  const_iterator cend () const
  {
    return _dynamic_arr_p != nullptr ? _dynamic_arr_p + _size
                                     : _static_arr + _size;
  }
  // all reverse iterators
  virtual reverse_iterator rbegin ()
  { return std::reverse_iterator<iterator> (end ()); }
  virtual const_reverse_iterator rbegin () const
  { return std::reverse_iterator<const_iterator> (end ()); }
  virtual const_reverse_iterator crbegin () const
  { return std::reverse_iterator<const_iterator> (cend ()); }
  virtual reverse_iterator rend ()
  { return std::reverse_iterator<iterator> (begin ()); }
  virtual const_reverse_iterator rend () const
  { return std::reverse_iterator<const_iterator> (begin ()); }
  virtual const_reverse_iterator crend () const
  { return std::reverse_iterator<const_iterator> (cbegin ()); }

  virtual size_t size () const
  { return _size; }
  size_t capacity () const
  { return _dynamic_arr_p != nullptr ? _dynamic_cap : _static_capacity; }
  bool empty () const
  { return size () == 0; }

  /** All the declarations, implementations outside the class*/
  T &at (size_t index);
  T at (size_t index) const;
  virtual void push_back (const T &elem);
  iterator insert (const_iterator position, const T &new_elem);

  template<class ForwardIterator>
  iterator insert (const_iterator position,
                   ForwardIterator first, ForwardIterator last);
  virtual void pop_back ();
  iterator erase (const_iterator elem_to_remove);
  iterator erase (const_iterator first, const_iterator last);
  virtual void clear ();
  T *data ();
  const T *data () const;
  virtual bool contains (const T &elem_to_check) const;

  /** cpy assignment ctr */
  vl_vector<T, StaticCapacity> &
  operator= (const vl_vector<T, StaticCapacity> &other);
  /** subscript operator */
  T &operator[] (size_t index)
  {
    return _dynamic_arr_p != nullptr ? _dynamic_arr_p[index]
                                     : _static_arr[index];
  }
  /** const subscript operator */
  const T &operator[] (size_t index) const
  {
    return _dynamic_arr_p != nullptr ? _dynamic_arr_p[index]
                                     : _static_arr[index];
  }
  /** comparison operator */
  bool operator== (const vl_vector<T, StaticCapacity> &other_vec) const;
  /** comparison operator */
  bool operator!= (const vl_vector<T, StaticCapacity> &other_vec) const
  { return !(*this == other_vec); }

 protected:

  /** the capacity function. calc it by the formula in ex6 def file */
  size_t cap_func (size_t k) const
  {
    return _size + k <= _static_capacity ? _static_capacity : 3 * (_size + k)
                                                              / 2;
  }

  size_t _size; // size of the vector
  size_t _dynamic_cap; // dynamic allocated capacity
  T *_dynamic_arr_p; // the dynamic allocated data
  size_t _static_capacity; // static capacity
  T _static_arr[StaticCapacity]; // the static array
};
/**
 * returns what in the vector's index place. throws out of range if fails
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param index where to look
 * @return what in the vector's index place
 */
template<typename T, size_t StaticCapacity>
T &vl_vector<T, StaticCapacity>::at (size_t index)
{
  if (index < 0 || index >= _size)
    {
      throw std::out_of_range ("Index Out of Range. ");
    }

  if (_dynamic_arr_p == nullptr)
    return _static_arr[index];

  return _dynamic_arr_p[index];
}
/**
 * returns what in the vector's index place (const). throws out of range if fails
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param index where to look
 * @return what in the vector's index place (const)
 */
template<typename T, size_t StaticCapacity>
T vl_vector<T, StaticCapacity>::at (size_t index) const
{
  if (index < 0 || index >= _size)
    throw std::out_of_range ("Index Out of Range");

  if (_dynamic_arr_p == nullptr)
    return _static_arr[index];

  return _dynamic_arr_p[index];
}
/**
 *
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param elem the elem to push
 */
template<typename T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::push_back (const T &elem)
{
  if (_size < _static_capacity)
    { // size + 1 <= _static_cap
      _static_arr[_size] = elem;
    }
  else if (_size == _static_capacity) // size + 1 > _static_cap
    {
      _dynamic_cap = cap_func (1);
      _dynamic_arr_p = new T[_dynamic_cap];

      for (size_t i = 0; i < _size; ++i)
        _dynamic_arr_p[i] = _static_arr[i];

      _dynamic_arr_p[_size] = elem;
    }
  else // size + 1 > _dynamic_cap
    {
      if (_size + 1 > _dynamic_cap)
        {
          _dynamic_cap = cap_func (1);
          T *new_array = new T[_dynamic_cap];
          for (size_t i = 0; i < _size; ++i)
            new_array[i] = _dynamic_arr_p[i];
          delete[] _dynamic_arr_p;
          _dynamic_arr_p = new_array;
        }
      _dynamic_arr_p[_size] = elem;
    }
  _size++;
}
/**
 * insert a single element
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param position iterator to the location to insert in
 * @param new_elem the elem to insert
 * @return iterator to the new element
 */
template<typename T, size_t StaticCapacity>
typename vl_vector<T, StaticCapacity>::iterator
vl_vector<T, StaticCapacity>::insert
    (const_iterator position, const T &new_elem)
{
  if (_size == 0)
    {
      push_back (new_elem);
      return begin ();
    }
  size_t dist = std::distance (cbegin (), position);
  if (_dynamic_cap == 0 && _static_capacity == _size)
    {
      _dynamic_cap = cap_func (1);
      _dynamic_arr_p = new T[_dynamic_cap];
      for (size_t i = 0; i < _size; ++i)
        _dynamic_arr_p[i] = _static_arr[i];
    }
  else if (_dynamic_arr_p != nullptr && _dynamic_cap == _size)
    {
      _dynamic_cap = cap_func (1);
      T *temp_arr = new T[_dynamic_cap];
      for (size_t i = 0; i < _size; ++i)
        temp_arr[i] = _dynamic_arr_p[i];
      delete[] _dynamic_arr_p;
      _dynamic_arr_p = temp_arr;
    }

  for (size_t i = _size; i > dist; --i)
    (*this)[i] = (*this)[i - 1];
  (*this)[dist] = new_elem;
  _size++;
  return &at (dist);
}

/**
 * insert a range of elements using iterators
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param position iterator to the location to insert in
 * @tparam ForwardIterator ForwardIterator type of iterator
 * @param first iterator of the first elem to add
 * @param last iterator of the last elem to add
 * @return iterator to the first newly added elem
 */
template<typename T, size_t StaticCapacity>
template<class ForwardIterator>
typename vl_vector<T, StaticCapacity>::iterator
vl_vector<T, StaticCapacity>::insert
    (const_iterator position, ForwardIterator first, ForwardIterator last)
{
  size_t dist = std::distance (first, last); // k - count of elements to cpy
  size_t pos = std::distance (cbegin (), position); // index of position
  if (_dynamic_arr_p == nullptr && _size + dist > _static_capacity)
    {
      _dynamic_cap = cap_func (dist);
      _dynamic_arr_p = new T[_dynamic_cap];
      for (size_t i = 0; i < _size; ++i)
        _dynamic_arr_p[i] = _static_arr[i];
    }
  else if (_dynamic_arr_p != nullptr && _dynamic_cap < dist + _size)
    {
      _dynamic_cap = cap_func (dist);
      T *temp_arr = new T[_dynamic_cap];
      for (size_t i = 0; i < _size; ++i)
        temp_arr[i] = _dynamic_arr_p[i];
      delete[] _dynamic_arr_p;
      _dynamic_arr_p = temp_arr;
    }
  // push all elements after position k places
  for (size_t i = _size + dist - 1; i > pos + dist - 1; --i)
    (*this)[i] = (*this)[i - dist];
  for (size_t i = pos; i < pos + dist; ++i)
    {
      (*this)[i] = *first;
      ++first;
    }
  _size += dist;
  return &at (pos);
}
/**
 * pops the last elem
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 */
template<typename T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::pop_back ()
{
  if (_size == 0)
    return;

  if (_dynamic_arr_p != nullptr && _size == _static_capacity + 1)
    {
      for (size_t i = 0; i < _size - 1; ++i)
        _static_arr[i] = _dynamic_arr_p[i];
      delete[] _dynamic_arr_p;
      _dynamic_arr_p = nullptr;
      _dynamic_cap = 0;
    }
  --_size;
}
/**
 * erases an element. gets an iterator to it and removes it
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param elem_to_remove iterator to the elem to remove
 * @return iterator to elem after the one who got removed
 */
template<typename T, size_t StaticCapacity>
typename vl_vector<T, StaticCapacity>::iterator
vl_vector<T, StaticCapacity>::erase (const_iterator elem_to_remove)
{
  size_t dist = std::distance (cbegin (), elem_to_remove);
  for (size_t i = dist; i < _size - 1; ++i)
    at (i) = at (i + 1);
  _size--;
  if (_dynamic_arr_p != nullptr && _size == _static_capacity)
    {
      for (size_t i = 0; i < _size; ++i)
        _static_arr[i] = _dynamic_arr_p[i];
      delete[] _dynamic_arr_p;
      _dynamic_arr_p = nullptr;
      _dynamic_cap = 0;
      if (dist == _static_capacity)
        return end ();
    }
  if (size () == 0)
    return begin ();
  if (dist == size ())
    return end ();
  return &at (dist);
}
/**
 * gets a range (using iterators) of elements, and erase them all
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param first iterator to the first elem to remove
 * @param last iterator to the last elem to remove
 * @return iterator to elem after the last one who got removed
 */
template<typename T, size_t StaticCapacity>
typename vl_vector<T, StaticCapacity>::iterator
vl_vector<T, StaticCapacity>::erase (const_iterator first, const_iterator last)
{
  size_t len = std::distance (first, last);
  size_t pos = std::distance (cbegin (), first);
  for (size_t i = pos; i < _size - len; ++i)
    at (i) = at (i + len);
  _size -= len;
  if (_dynamic_arr_p != nullptr && _size <= _static_capacity)
    {
      for (size_t i = 0; i < _size; ++i)
        _static_arr[i] = _dynamic_arr_p[i];
      delete[] _dynamic_arr_p;
      _dynamic_arr_p = nullptr;
      _dynamic_cap = 0;
      if (pos >= _static_capacity)
        return end ();
      else if (size () == 0) // clear was made
        return begin ();
    }
  if (pos == size ())
    return end ();
  return &at (pos);
}
/**
 * clears the vector
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 */
template<typename T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::clear ()
{
  if (size () > 0)
    erase (begin (), end ());
}
/**
 * functions that grants access to the vector's data
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @return the data of the vector
 */
template<typename T, size_t StaticCapacity>
T *vl_vector<T, StaticCapacity>::data ()
{
  if (_dynamic_arr_p != nullptr)
    return _dynamic_arr_p;
  return _static_arr;
}
// same but const data
template<typename T, size_t StaticCapacity>
const T *vl_vector<T, StaticCapacity>::data () const
{
  if (_dynamic_arr_p != nullptr)
    return _dynamic_arr_p;
  return _static_arr;
}
/**
 *
 * @tparam T the template arg
 * @tparam StaticCapacity the static capacity of the vector
 * @param elem_to_check to see if in the vector
 * @return true if found, false otherwise
 */
template<typename T, size_t StaticCapacity>
bool vl_vector<T, StaticCapacity>::contains (const T &elem_to_check) const
{
  for (size_t i = 0; i < _size; ++i)
    {
      if (at (i) == elem_to_check)
        return true;
    }
  return false;
}
/** copy assignment operator */
template<typename T, size_t StaticCapacity>
vl_vector<T, StaticCapacity> &vl_vector<T, StaticCapacity>::operator=
    (const vl_vector<T, StaticCapacity> &other)
{
  if (this == &other)
    return *this;

  _size = other._size;
  _static_capacity = other._static_capacity;
  _dynamic_cap = other._dynamic_cap;

  if (other._dynamic_arr_p != nullptr)
    {
      _dynamic_arr_p = new T[_dynamic_cap];
      for (size_t i = 0; i < other._size; ++i)
        _dynamic_arr_p[i] = other._dynamic_arr_p[i];
    }
  else
    {
      _dynamic_arr_p = nullptr;
      for (size_t i = 0; i < other._size; ++i)
        _static_arr[i] = other._static_arr[i];
    }
  return *this;
}
/** comp operator */
template<typename T, size_t StaticCapacity>
bool vl_vector<T, StaticCapacity>::operator==
    (const vl_vector<T, StaticCapacity> &other_vec) const
{
  if (size () != other_vec.size ())
    return false;

  if (_dynamic_arr_p != nullptr)
    {
      for (size_t i = 0; i < _size; ++i)
        if (_dynamic_arr_p[i] != other_vec._dynamic_arr_p[i])
          return false;
    }
  else
    {
      for (size_t i = 0; i < _size; ++i)
        if (_static_arr[i] != other_vec._static_arr[i])
          return false;
    }
  return true;
}

#endif //_VL_VECTOR_H_
