/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2005 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(MACRO_HELPERS_HPP_931BBC99_EBFA_4692_8FBE_B555998C2C39_INCLUDED)
#define MACRO_HELPERS_HPP_931BBC99_EBFA_4692_8FBE_B555998C2C39_INCLUDED

#include <boost/wave/wave_config.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/validate_universal_char.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace util {

namespace impl {

    // escape a string literal (insert '\\' before every '\"', '?' and '\\')
    template <typename StringT>
    inline StringT
    escape_lit(StringT const &value)
    {
        StringT result;
        typename StringT::size_type pos = 0;
        typename StringT::size_type pos1 = value.find_first_of ("\"\\?", 0);
        if (StringT::npos != pos1) {
            do {
                result += value.substr(pos, pos1-pos) 
                            + StringT("\\") 
                            + StringT(1, value[pos1]);
                pos1 = value.find_first_of ("\"\\?", pos = pos1+1);
            } while (StringT::npos != pos1);
            result += value.substr(pos);
        }
        else {
            result = value;
        }
        return result;
    }

    // un-escape a string literal (remove '\\' just before '\\', '\"' or '?')
    template <typename StringT>
    inline StringT
    unescape_lit(StringT const &value)
    {
        StringT result;
        typename StringT::size_type pos = 0;
        typename StringT::size_type pos1 = value.find_first_of ("\\", 0);
        if (StringT::npos != pos1) {
            do {
                if ('\\' == value[pos1+1] || '\"' == value[pos1+1] || 
                    '?' == value[pos1+1])
                {
                    result += value.substr(pos, pos1-pos);
                }
                pos1 = value.find_first_of ("\\", pos = pos1+1);
            } while (pos1 != StringT::npos);
            result += value.substr(pos);
        }
        else {
        // the string doesn't contain any escaped character sequences
            result = value;
        }
        return result;
    }
    
    // return the string representation of a token sequence
    template <typename ContainerT, typename PositionT>
    inline typename ContainerT::value_type::string_type
    as_stringlit (ContainerT const &token_sequence, PositionT const &pos)
    {
        using namespace boost::wave;
        typedef typename ContainerT::value_type::string_type string_type;
        
        string_type result("\"");
        bool was_whitespace = false;
        typename ContainerT::const_iterator end = token_sequence.end();
        for (typename ContainerT::const_iterator it = token_sequence.begin(); 
             it != end; ++it) 
        {
            token_id id = token_id(*it);
            
            if (IS_CATEGORY(*it, WhiteSpaceTokenType) || T_NEWLINE == id) {
                if (!was_whitespace) {
                // C++ standard 16.3.2.2 [cpp.stringize]
                // Each occurrence of white space between the argumentís 
                // preprocessing tokens becomes a single space character in the 
                // character string literal.
                    result += " ";
                    was_whitespace = true;
                }
            }
            else if (T_STRINGLIT == id || T_CHARLIT == id) {
            // string literals and character literals have to be escaped
                result += impl::escape_lit((*it).get_value());
                was_whitespace = false;
            }
            else 
#if BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
                if (T_PLACEMARKER != id) 
#endif 
            {
            // now append this token to the string
                result += (*it).get_value();
                was_whitespace = false;
            }
        }
        result += "\"";

    // validate the resulting literal to contain no invalid universal character
    // value (throws if invalid chars found)
        boost::wave::cpplexer::impl::validate_literal(result, pos.get_line(), 
            pos.get_column(), pos.get_file()); 
        return result;
    }

#if BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
    // return the string representation of a token sequence
    template <typename ContainerT, typename PositionT>
    inline typename ContainerT::value_type::string_type
    as_stringlit (std::vector<ContainerT> const &arguments, 
        typename std::vector<ContainerT>::size_type i, PositionT const &pos)
    {
        using namespace boost::wave;
        typedef typename ContainerT::value_type::string_type string_type;
        
        BOOST_SPIRIT_ASSERT(0 <= i && i < arguments.size());
        
        string_type result("\"");
        bool was_whitespace = false;
        
        for (/**/; i < arguments.size(); ++i) {
        // stringize all remaining arguments
            typename ContainerT::const_iterator end = arguments[i].end();
            for (typename ContainerT::const_iterator it = arguments[i].begin(); 
                 it != end; ++it) 
            {
                token_id id = token_id(*it);
                
                if (IS_CATEGORY(*it, WhiteSpaceTokenType) || T_NEWLINE == id) {
                    if (!was_whitespace) {
                    // C++ standard 16.3.2.2 [cpp.stringize]
                    // Each occurrence of white space between the argumentís 
                    // preprocessing tokens becomes a single space character in the 
                    // character string literal.
                        result += " ";
                        was_whitespace = true;
                    }
                }
                else if (T_STRINGLIT == id || T_CHARLIT == id) {
                // string literals and character literals have to be escaped
                    result += impl::escape_lit((*it).get_value());
                    was_whitespace = false;
                }
                else if (T_PLACEMARKER != id) {
                // now append this token to the string
                    result += (*it).get_value();
                    was_whitespace = false;
                }
            }
            
        // append comma, if not last argument
            if (i < arguments.size()-1) {
                result += ",";
                was_whitespace = false;
            }
        }
        result += "\"";

    // validate the resulting literal to contain no invalid universal character
    // value (throws if invalid chars found)
        boost::wave::cpplexer::impl::validate_literal(result, pos.get_line(), 
            pos.get_column(), pos.get_file()); 
        return result;
    }
#endif // BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0

    // return the string representation of a token sequence
    template <typename StringT, typename IteratorT>
    inline StringT
    as_string(IteratorT it, IteratorT end)
    {
        StringT result;
        for (/**/; it != end; ++it) 
        {
            result += (*it).get_value();
        }
        return result;
    }
    
    // return the string representation of a token sequence
    template <typename ContainerT>
    inline typename ContainerT::value_type::string_type
    as_string (ContainerT const &token_sequence)
    {
        typedef typename ContainerT::value_type::string_type string_type;
        return as_string<string_type>(token_sequence.begin(), token_sequence.end());
    }
    
#if BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
    ///////////////////////////////////////////////////////////////////////////
    //
    //  Copies all arguments beginning with the given index to the output 
    //  sequence. The arguments are separated by commas.
    //
    template <typename ContainerT, typename PositionT>
    void replace_ellipsis (std::vector<ContainerT> const &arguments,
        typename ContainerT::size_type index, 
        ContainerT &expanded, PositionT const &pos)
    {
        using namespace cpplexer;
        typedef typename ContainerT::value_type token_type;
        
        token_type comma(T_COMMA, ",", pos);
        for (/**/; index < arguments.size(); ++index) {
        ContainerT const &arg = arguments[index];
        
            std::copy(arg.begin(), arg.end(), 
                std::inserter(expanded, expanded.end()));
                
            if (index < arguments.size()-1) 
                expanded.push_back(comma);
        }
    }
#endif

}   // namespace impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace util
}   // namespace wave
}   // namespace boost

#endif // !defined(MACRO_HELPERS_HPP_931BBC99_EBFA_4692_8FBE_B555998C2C39_INCLUDED)
