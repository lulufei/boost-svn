
#ifndef BOOST_MPL_ADVANCE_HPP_INCLUDED
#define BOOST_MPL_ADVANCE_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/negate.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/iterator_tag.hpp>
#include <boost/mpl/aux_/advance_forward.hpp>
#include <boost/mpl/aux_/advance_backward.hpp>
#include <boost/mpl/aux_/iterator_category.hpp>
#include <boost/mpl/aux_/iterator_names.hpp>
#include <boost/mpl/aux_/msvc_never_true.hpp>
#include <boost/mpl/aux_/apply.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/config/nttp.hpp>
#include <boost/config.hpp>

namespace boost {
namespace mpl {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace aux {

// forward/bidirectional iterators
template< typename Category, typename Iterator, typename N >
struct advance_impl
{
    typedef typename less< N,integral_c<long,0> >::type backward_;
    typedef typename if_< backward_, negate<N>, N >::type offset_;

    typedef typename if_<
          backward_
        , aux::advance_backward< BOOST_MPL_AUX_VALUE_WKND(offset_)::value >
        , aux::advance_forward< BOOST_MPL_AUX_VALUE_WKND(offset_)::value >
        >::type algo_;

    typedef typename BOOST_MPL_AUX_APPLY1(algo_,Iterator)::type type;
};

// random-access iterators
template< typename Iterator, typename N >
struct advance_impl<random_access_iterator_tag,Iterator,N>
{
    typedef typename Iterator
        ::template BOOST_MPL_AUX_ITERATOR_ADVANCE<N>::type type;
};

} // namespace aux

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_NA_PARAM(Iterator)
    , typename BOOST_MPL_AUX_NA_PARAM(N)
    >
struct advance
{
    typedef typename aux::advance_impl<
          typename BOOST_MPL_AUX_ITERATOR_CATEGORY(Iterator)
        , Iterator
        , N
        >::type type;
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

template<
      typename Iterator
    , BOOST_MPL_AUX_NTTP_DECL(long, N)
    >
struct advance_c
{
    typedef typename aux::advance_impl<
          typename BOOST_MPL_AUX_ITERATOR_CATEGORY(Iterator)
        , Iterator
        , integral_c<long,N>
        >::type type;
};

#else // no partial specialization

namespace aux {

// forward/bidirectional iterators
template< typename Category >
struct advance_impl
{
    template< typename Iterator, typename N > struct result_
    {
        enum { n = N::value }; // MSVC 7.x workaround
        typedef typename if_c<
              (n < 0)
            , aux::advance_backward<(-n)>
            , aux::advance_forward<n>
            >::type algo_;

        typedef typename BOOST_MPL_AUX_APPLY1(algo_,Iterator)::type type;
    };
};

// random-access iterators

#if defined(BOOST_MSVC) && BOOST_MSVC < 1300

// msvc_advance
#define BOOST_MPL_AUX_MSVC_DTW_NAME msvc_advance
#define BOOST_MPL_AUX_MSVC_DTW_ORIGINAL_NAME BOOST_MPL_AUX_ITERATOR_ADVANCE
#define BOOST_MPL_AUX_MSVC_DTW_ARITY 1
#include <boost/mpl/aux_/msvc_dtw.hpp>

template<>
struct advance_impl<random_access_iterator_tag>
{
    template< typename Iterator, typename N > struct result_
    {
        typedef typename msvc_advance<Iterator>
            ::template result_<N>::type type;
    };
};
#else
template<>
struct advance_impl<random_access_iterator_tag>
{
    template< typename Iterator, typename N > struct result_
    {
        typedef typename Iterator
            ::template BOOST_MPL_AUX_ITERATOR_ADVANCE<N>::type type;
    };
};
#endif

} // namespace aux

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_NA_PARAM(Iterator)
    , typename BOOST_MPL_AUX_NA_PARAM(N)
    >
struct advance
{
    typedef typename BOOST_MPL_AUX_ITERATOR_CATEGORY(Iterator) tag_;
    typedef typename aux::advance_impl<tag_>
        ::template result_<
              Iterator
            , N
            >::type type;
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

template<
      typename Iterator
    , BOOST_MPL_AUX_NTTP_DECL(long, N)
    >
struct advance_c
{
    typedef typename BOOST_MPL_AUX_ITERATOR_CATEGORY(Iterator) tag_;
    typedef typename aux::advance_impl<tag_>
        ::template result_<
              Iterator
            , integral_c<long,N>
            >::type type;
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

BOOST_MPL_AUX_NA_ALGORITHM_SPEC(2, advance)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_ADVANCE_HPP_INCLUDED
