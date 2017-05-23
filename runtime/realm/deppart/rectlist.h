/* Copyright 2015 Stanford University, NVIDIA Corporation
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

// rectangle lists for Realm partitioning

#ifndef REALM_DEPPART_RECTLIST_H
#define REALM_DEPPART_RECTLIST_H

#include "indexspace.h"

namespace Realm {

  // although partitioning operations eventually generate SparsityMap's, we work with
  //  various intermediates that try to keep things from turning into one big bitmask

  // the CoverageCounter just counts the number of points that get added to it
  // it's not even smart enough to eliminate duplicates
  template <int N, typename T>
  class CoverageCounter {
  public:
    CoverageCounter(void);

    void add_point(const ZPoint<N,T>& p);

    void add_rect(const ZRect<N,T>& r);

    size_t get_count(void) const;

  protected:
    size_t count;
  };

  template <int N, typename T>
  class DenseRectangleList {
  public:
    DenseRectangleList(size_t _max_rects = 0);

    void add_point(const ZPoint<N,T>& p);

    void add_rect(const ZRect<N,T>& r);

    void merge_rects(size_t upper_bound);

    std::vector<ZRect<N,T> > rects;
    size_t max_rects;
  };

  template <int N, typename T>
  class HybridRectangleList {
  public:
    static const size_t HIGH_WATER_MARK = 64;
    static const size_t LOW_WATER_MARK = 16;

    HybridRectangleList(void);

    void add_point(const ZPoint<N,T>& p);

    void add_rect(const ZRect<N,T>& r);

    const std::vector<ZRect<N,T> >& convert_to_vector(void);

    std::vector<ZRect<N,T> > as_vector;
    //std::multimap<T, ZRect<N,T> > as_mmap;
  };
    
};

#endif // REALM_DEPPART_RECTLIST_H

#include "rectlist.inl"
