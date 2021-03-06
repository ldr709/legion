/* Copyright 2018 Stanford University, NVIDIA Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// templated intrusive linked lists

#ifndef REALM_LISTS_H
#define REALM_LISTS_H

namespace Realm {

#ifdef DEBUG_REALM
//define DEBUG_REALM_LISTS
#endif

  template <typename T>
  struct IntrusiveListLink {
    IntrusiveListLink(void);
    ~IntrusiveListLink(void);

    T *next;
#ifdef DEBUG_REALM_LISTS
    // actual type is IntrusiveList<T, LINK, LT>, but that would be a
    //  circular definition...
    void *current_list;
#endif
  };

  template <typename T, IntrusiveListLink<T> T::*LINK, typename LT>
  class IntrusiveList {
  public:
    typedef T ITEMTYPE;
    typedef LT LOCKTYPE;

    IntrusiveList(void);
    ~IntrusiveList(void);

    // "copying" a list is only allowed if both lists are empty (this is most
    //  useful when creating lists inside of containers)
    IntrusiveList(const IntrusiveList<T, LINK, LT>& copy_from);
    IntrusiveList<T, LINK, LT>& operator=(const IntrusiveList<T, LINK, LT>& copy_from);

    template <typename LT2>
    void swap(IntrusiveList<T, LINK, LT2>& swap_with);

    // sucks the contents of 'take_from' into the end of the current list
    template <typename LT2>
    void absorb_append(IntrusiveList<T, LINK, LT2>& take_from);

    void push_back(T *new_entry);
    void push_front(T *new_entry);

    bool empty(void) const;

    T *front(void) const;
    T *pop_front(void);

    mutable LT lock;
    IntrusiveListLink<T> head;
    IntrusiveListLink<T> *lastlink;
  };

  template <typename T>
  struct IntrusivePriorityListLink {
    IntrusivePriorityListLink(void);
    ~IntrusivePriorityListLink(void);

    T *next_within_pri;
    T **lastlink_within_pri;  // for O(1) append
    T *next_lower_pri;
#ifdef DEBUG_REALM_LISTS
    // actual type is IntrusivePriorityList<T, ...>, but that would be a
    //  circular definition...
    void *current_list;
#endif
  };

  template <typename T, typename PT, IntrusivePriorityListLink<T> T::*LINK, PT T::*PRI, typename LT>
  class IntrusivePriorityList {
  public:
    typedef T ITEMTYPE;
    typedef PT PRITYPE;
    typedef LT LOCKTYPE;

    IntrusivePriorityList(void);
    ~IntrusivePriorityList(void);

    // "copying" a list is only allowed if both lists are empty (this is most
    //  useful when creating lists inside of containers)
    IntrusivePriorityList(const IntrusivePriorityList<T, PT, LINK, PRI, LT>& copy_from);
    IntrusivePriorityList<T, PT, LINK, PRI, LT>& operator=(const IntrusivePriorityList<T, PT, LINK, PRI, LT>& copy_from);

    template <typename LT2>
    void swap(IntrusivePriorityList<T, PT, LINK, PRI, LT2>& swap_with);

    // sucks the contents of 'take_from' into the end of the current list
    template <typename LT2>
    void absorb_append(IntrusivePriorityList<T, PT, LINK, PRI, LT2>& take_from);

    // places new item at front or back of its priority level
    void push_back(T *new_entry);
    void push_front(T *new_entry);

    bool empty(PT min_priority) const;

    // this call isn't thread safe - the pointer returned may be accessed only
    //  if the caller can guarantee no concurrent pops have occurred
    T *front(void) const;

    T *pop_front(PT min_priority);

    // calls callback for each element in list
    template <typename CALLBACK>
    void foreach(CALLBACK& cb);

    // we don't maintain the size, so this is slow - use only for debugging
    size_t size(void) const;

    mutable LT lock;
    T *head;
    // TODO: consider indexing if many priorities exists simultaneously?
  };

  template <typename T, typename PT, IntrusivePriorityListLink<T> T::*LINK, PT T::*PRI, typename LT>
  std::ostream& operator<<(std::ostream& os, const IntrusivePriorityList<T, PT, LINK, PRI, LT>& to_print);

}; // namespace Realm

#include "realm/lists.inl"

#endif // ifndef REALM_LISTS_H
