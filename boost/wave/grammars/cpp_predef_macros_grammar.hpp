/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2005 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(CPP_PREDEF_MACROS_GRAMMAR_HPP_53858C9A_C202_4D60_AD92_DC9CAE4DBB43_INCLUDED)
#define CPP_PREDEF_MACROS_GRAMMAR_HPP_53858C9A_C202_4D60_AD92_DC9CAE4DBB43_INCLUDED

#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/lists.hpp>

#include <boost/wave/wave_config.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_predef_macros_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace grammars {

///////////////////////////////////////////////////////////////////////////////
//  define, whether the rule's should generate some debug output
#define TRACE_PREDEF_MACROS_GRAMMAR \
    bool(BOOST_SPIRIT_DEBUG_FLAGS_CPP & BOOST_SPIRIT_DEBUG_FLAGS_PREDEF_MACROS_GRAMMAR) \
    /**/

///////////////////////////////////////////////////////////////////////////////
// Encapsulation of the grammar for command line driven predefined macros.
struct predefined_macros_grammar : 
    public boost::spirit::grammar<predefined_macros_grammar>
{
    template <typename ScannerT>
    struct definition
    {
    // 'normal' (parse_tree generating) rule type
        typedef boost::spirit::rule<ScannerT> rule_t;

        rule_t plain_define, macro_definition, macro_parameters;

        definition(predefined_macros_grammar const &self) 
        {
        // import the spirit and cpplexer namespaces here
            using namespace boost::spirit;
            using namespace boost::wave;
            using namespace boost::wave::util;
            
        // save the rule id's for later use
            self.rule_ids.plain_define_id = plain_define.id().to_long();
            self.rule_ids.macro_parameters_id = macro_parameters.id().to_long();
            self.rule_ids.macro_definition_id = macro_definition.id().to_long();

        // recognizes command line defined macro syntax, i.e.
        //  -DMACRO
        //  -DMACRO=
        //  -DMACRO=value
        //  -DMACRO(x)
        //  -DMACRO(x)=
        //  -DMACRO(x)=value

        // This grammar resembles the overall structure of the cpp_grammar to
        // make it possible to reuse the parse tree traversal code
            plain_define
                =   (   ch_p(T_IDENTIFIER)
                    |   pattern_p(KeywordTokenType, TokenTypeMask)
                    |   pattern_p(OperatorTokenType|AltExtTokenType, 
                            ExtTokenTypeMask)   // and, bit_and etc.
                    )
                    >>  !macro_parameters
                    >>  !macro_definition
                ;

        // parameter list
            macro_parameters
                =   confix_p(
                        no_node_d[ch_p(T_LEFTPAREN) >> *ch_p(T_SPACE)],
                       !list_p(
                            (   ch_p(T_IDENTIFIER)
                            |   pattern_p(KeywordTokenType, TokenTypeMask)
                            |   pattern_p(OperatorTokenType|AltExtTokenType, 
                                    ExtTokenTypeMask)   // and, bit_and etc.
#if BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
                            |   ch_p(T_ELLIPSIS)
#endif
                            ), 
                            no_node_d
                            [
                                *ch_p(T_SPACE) >> ch_p(T_COMMA) >> *ch_p(T_SPACE)
                            ]
                        ),
                        no_node_d[*ch_p(T_SPACE) >> ch_p(T_RIGHTPAREN)]
                    )
                ;

        // macro body (anything left until eol)
            macro_definition
                =   no_node_d[ch_p(T_ASSIGN)]
                    >> *anychar_p
                ;

            BOOST_SPIRIT_DEBUG_TRACE_RULE(plain_define, TRACE_PREDEF_MACROS_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(macro_definition, TRACE_PREDEF_MACROS_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(macro_parameters, TRACE_PREDEF_MACROS_GRAMMAR);
        }

    // start rule of this grammar
        rule_t const& start() const
        { return plain_define; }
    };

    predefined_macros_grammar_rule_ids &rule_ids;
    
    predefined_macros_grammar(predefined_macros_grammar_rule_ids &rule_ids_) 
    :   rule_ids(rule_ids_)
    { 
        BOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME(*this, 
            "predefined_macros_grammar", TRACE_PREDEF_MACROS_GRAMMAR); 
    }

};

///////////////////////////////////////////////////////////////////////////////
#undef TRACE_PREDEF_MACROS_GRAMMAR

///////////////////////////////////////////////////////////////////////////////
//  
//  The following parse function is defined here, to allow the separation of 
//  the compilation of the cpp_predefined_macros_grammar from the function 
//  using it.
//  
///////////////////////////////////////////////////////////////////////////////

#if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0
#define BOOST_WAVE_PREDEF_MACROS_GRAMMAR_GEN_INLINE
#else
#define BOOST_WAVE_PREDEF_MACROS_GRAMMAR_GEN_INLINE inline
#endif 

template <typename LexIteratorT>
BOOST_WAVE_PREDEF_MACROS_GRAMMAR_GEN_INLINE 
boost::spirit::tree_parse_info<LexIteratorT>
predefined_macros_grammar_gen<LexIteratorT>::parse_predefined_macro (
    LexIteratorT const &first, LexIteratorT const &last)
{
    static predefined_macros_grammar g(rule_ids);
    return boost::spirit::pt_parse (first, last, g);
}

#undef BOOST_WAVE_PREDEF_MACROS_GRAMMAR_GEN_INLINE

///////////////////////////////////////////////////////////////////////////////
}   // namespace grammars
}   // namespace wave
}   // namespace boost

#endif // !defined(CPP_PREDEF_MACROS_GRAMMAR_HPP_53858C9A_C202_4D60_AD92_DC9CAE4DBB43_INCLUDED)
