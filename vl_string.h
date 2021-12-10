
#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "vl_vector.h"
#include <cstring>
#include <memory>

template<size_t StaticCapacity = DEF_STATIC_CAP>
class vl_string : public vl_vector<char, StaticCapacity> {

 public:
  /** def ctr */
  vl_string () : vl_vector<char, StaticCapacity> (1, '\0')
  {}
  /** cpy ctr */
  vl_string (const vl_string<StaticCapacity> &other) :
      vl_vector<char, StaticCapacity> (other)
  {}
  /** implicit ctr - the plus 1 for adding the "\0" terminator */
  vl_string (const char *str_to_cpy) :
      vl_vector<char, StaticCapacity> (str_to_cpy,
                                       str_to_cpy + strlen (str_to_cpy) + 1)
  {}

  // iterators typedefs
  typedef char *iterator;
  typedef const char *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // all reverse iterators
  reverse_iterator rbegin () override
  { return std::reverse_iterator<iterator> (this->end () - 1); }
  const_reverse_iterator rbegin () const override
  { return std::reverse_iterator<const_iterator> (this->end () - 1); }
  const_reverse_iterator crbegin () const override
  { return std::reverse_iterator<const_iterator> (this->cend () - 1); }

  size_t size () const override
  { return this->_size - 1; }

  void push_back (const char &elem) override
  {
    this->insert (this->cend () - 1, elem);
  }

  void pop_back () override
  { if (size () > 0) this->erase (this->end () - 2); }

  void clear () override
  { if (this->size () > 0) this->erase (this->begin (), this->end () - 1); }

  virtual bool contains (const char *substr) const;

  /** class operators implementations */
  vl_string<StaticCapacity> &
  operator+= (const vl_string<StaticCapacity> &other);
  vl_string<StaticCapacity> &operator+= (const char *str);
  vl_string<StaticCapacity> &operator+= (char single_char);
  vl_string<StaticCapacity> operator+ (const vl_string<StaticCapacity> &other)
  const;
  vl_string<StaticCapacity> operator+ (const char *str) const;
  vl_string<StaticCapacity> operator+ (char single_char) const;

  operator const char * () const;
};

/**
 * checks if substring in string
 * @tparam StaticCapacity template capacity
 * @param substr the substring to search
 * @return
 */
template<size_t StaticCapacity>
bool vl_string<StaticCapacity>::contains (const char *substr) const
{
  if (strlen (substr) > this->size ())
    return false;
  for (size_t i = 0; i < this->_size - strlen (substr); ++i)
    if (this->at (i) == *substr)
      {
        for (size_t j = 0; j < strlen (substr); ++j)
          if (substr[j] != this->at (i + j))
            return false;
        return true;
      }
  return false;
}

/** friend operators */
template<size_t StaticCapacity>
vl_string<StaticCapacity> &vl_string<StaticCapacity>::operator+=
    (const vl_string<StaticCapacity> &other)
{ // insert new string without its null terminator
  this->insert (this->cend () - 1, other.begin (), other.end () - 1);
  return *this;
}

template<size_t StaticCapacity>
vl_string<StaticCapacity> &vl_string<StaticCapacity>::operator+=
    (const char *str)
{
  this->insert (this->end () - 1, str, str + strlen (str));
  return *this;
}

template<size_t StaticCapacity>
vl_string<StaticCapacity>
&vl_string<StaticCapacity>::operator+= (const char single_char)
{
  this->push_back (single_char);
  return *this;
}

template<size_t StaticCapacity>
vl_string<StaticCapacity>
vl_string<StaticCapacity>::operator+
    (const vl_string<StaticCapacity> &other) const
{
  vl_string<StaticCapacity> ret_val = *this;
  ret_val += other;
  return ret_val;
}

template<size_t StaticCapacity>
vl_string<StaticCapacity>
vl_string<StaticCapacity>::operator+ (const char *str) const
{
  return *this + vl_string<StaticCapacity> (str);
}

template<size_t StaticCapacity>
vl_string<StaticCapacity>
vl_string<StaticCapacity>::operator+ (const char single_char) const
{
  vl_string<StaticCapacity> ret_val = *this;
  ret_val += single_char;
  return ret_val;
}

template<size_t StaticCapacity>
vl_string<StaticCapacity>::operator const char * () const
{
  return this->data ();
}

#endif //_VL_STRING_H_
