namespace boost { namespace mpl {

template<
      typename Tag1
    , typename Tag2
    , int tag1_ = Tag1::value
    , int tag2_ = Tag2::value
    >
struct less_equal_impl
    : if_c<
          ( tag1_ > tag2_ )
        , aux::cast2nd_impl< less_equal_impl<Tag1,Tag2>,Tag1,Tag2 >
        , aux::cast1st_impl< less_equal_impl<Tag1,Tag2>,Tag1,Tag2 >
        >
{
};

template< typename T > struct less_equal_tag
{
    typedef typename T::tag type;
};

template<
      typename N1
    , typename N2
    >
struct less_equal
    : aux::msvc_eti_base< typename apply_wrap2<
          less_equal_impl<
              typename less_equal_tag<N1>::type
            , typename less_equal_tag<N2>::type
            >
        , N1
        , N2
        >::type >::type
{
};

}}
namespace boost { namespace mpl {
template<>
struct less_equal_impl< integral_c_tag,integral_c_tag >
{
    template< typename N1, typename N2 > struct apply
    {
        enum { value =
             ( BOOST_MPL_AUX_VALUE_WKND(N1)::value <=
             BOOST_MPL_AUX_VALUE_WKND(N2)::value )
            };
        typedef bool_<value> type;
    };
};

}}
