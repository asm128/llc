#include "llc_string_compose.h"
#include "llc_slice.h"
#include "llc_array_ptr.h"
#include "llc_enum.h"


#ifndef LLC_READER_H_261125
#define LLC_READER_H_261125


#define llc_reader_info_printf  info_printf

nmsp llc
{
#pragma pack(push, 1)
    tplt<tpnm TToken>
    stct SToken {
        s2_t	        ParentIndex ;   
        TToken	        Type        : 7;
        TToken		    Container   : 1;
        sliceu2_t       Span        ;   

        inxp            SToken      () = default;
        inxp            SToken      (s2_t parentIndex, TToken tokenType, b8_t isContainer, sliceu2_t span)
            : ParentIndex(parentIndex), Type(tokenType), Container((TToken)isContainer), Span(span) {}

        inln    err_t   str         (asc_t & output) cnst {
            rtrn append_strings(output
                , "ParentIndex  :", ::llc::str      (ParentIndex  )
                , "Type         :", ::llc::str      (Type         )
                , "Container    :", ::llc::b82st    (Container)
                , "Span         :", ::llc::str      (Span.Begin), ',', ::llc::str(Span.End)
                );
        }
    };	
#ifdef LLC_ARDUINO
    tydf  s1_t tid_t;
#else
    tydf  s2_t tid_t;
#endif
    tplTstct SReaderNode {
        tydf SReaderNode<T>     TNode;
        cnst SToken<T>			* Token				= 0;
        TNode   				* Parent			= 0;
        apobj<TNode>	    	Children			= {};
        tid_t					ObjectIndex			= -1;
    };
    tplT    err_t			readerTreeRebuild		(view<cnst SToken<T>> in_object, apobj<SReaderNode<T>> & out_nodes);

    stct SReaderFlag {
        b8_t            Escaping            : 1;
        b8_t            InsideString        : 1;
        b8_t            ExpectingSeparator  : 1;
        b8_t            DoneReading         : 1;

        inln    err_t   str                 (asc_t & output) cnst {
            rtrn append_strings(output
                , "Escaping             :", b82st(Escaping           )
                , "InsideString         :", b82st(InsideString       )
                , "ExpectingSeparator   :", b82st(ExpectingSeparator )
                , "DoneReading          :", b82st(DoneReading        )
                );
        }
    };
    tplt<tpnm TToken>
    stct SReaderState {
        u2_t			IndexCurrentChar	= 0;
        s2_t			IndexCurrentElement	= -1;
        SToken<TToken>	* CurrentElement	= 0;
        s2_t			NestLevel			= 0;
        sc_t			CharCurrent			= 0;
        SReaderFlag     Flags               = {};

        inln    err_t   str         (asc_t & output) cnst {
            if_fail_fe(append_strings(output
                , "IndexCurrentChar   :", ::llc::str(IndexCurrentChar   )
                , "IndexCurrentElement:", ::llc::str(IndexCurrentElement)
                , "* CurrentElement   :", ::llc::str(CurrentElement     )
                , "NestLevel          :", ::llc::str(NestLevel          )
                , "CharCurrent        :", CharCurrent
                ));
            if_fail_fe(Flags.str(output));
            if(CurrentElement) { 
                if_fail_fe(CurrentElement->str(output));
            }
            rtrn 0;
        }                    
    };
#pragma pack(pop)

    tplt<tpnm TToken>
    sinx err_t      readerStateParseStep	    (SReaderState<TToken> & readerState, vcst_t documentCharacters)	{
        if_true_fef(readerState.IndexCurrentChar >= documentCharacters.size(), "state.IndexCurrentChar:%u text.size():%u.", readerState.IndexCurrentChar, documentCharacters.size());
        readerState.CharCurrent	= documentCharacters[readerState.IndexCurrentChar];
        rtrn 0;
    }
    tplt<tpnm TToken, tpnm TOutput, tpnm TProcessCharacter>
    sinx err_t      readerStateParseStep	    (SReaderState<TToken> & readerState, TOutput & outputTokens, vcst_t documentCharacters, const TProcessCharacter & funcProcessCharacter)	{
        if_fail_fe(readerStateParseStep(readerState, documentCharacters));
        if_true_block_log(error_printf
            , failed(funcProcessCharacter(readerState, outputTokens, documentCharacters)
            ), {
                string              errorMessage;
                if_fail_fe(readerState.str(errorMessage));
                error_printf("%.*s", LLC_VIEW_ARGS(errorMessage));
                rtrn -1;
            }
        );
        return 0;
    }
    tplTstct SReader {
        tydf SToken<T>          TToken;
        tydf SReaderNode<T>     TNode;
        tydf pobj<TNode>        PNode;

        SReaderState<T>          State;
        apod    <TToken>         Token;
        aobj    <vcst_t>        View;
        apobj   <TNode>	        Tree;
        apas2_t                 TreeMap;
 
        inxp    cnst pobj<TNode>& oper[]	(u2_t index)	cnst	{ rtrn Tree[index]; }
        inxp    pobj<TNode>*    begin	()				csnx	{ rtrn Tree.begin(); }
        inxp    pobj<TNode>*    end		()				csnx	{ rtrn Tree.end(); }
        inln    err_t		    Reset	()						{
            State                   = {};
            rtrn clear(Token); //, Data, View); Don't clear other than Token!!
        }
    };

    tplt<tpnm TToken>
    stin err_t      readerParseStep		    (SReader<TToken> & reader, vcst_t documentCharacters, const FError<SReader<TToken>&, vcst_c&> & funcProcessCharacter)	{
        if_fail_fe(readerStateParseStep(reader.State, documentCharacters));
        if_true_block_log(error_printf
            , failed(funcProcessCharacter(reader, documentCharacters)
            ), {
                string              errorMessage;
                if_fail_fe(reader.State.str(errorMessage));
                error_printf("%.*s", LLC_VIEW_ARGS(errorMessage));
                rtrn -1;
            }
        );
        rtrn 0;
    }
    tplt<tpnm TToken>
    sttc	err_t	readerCloseType				(SReaderState<TToken> & stateReader, apod<SToken<TToken>> & tokens, TToken type, s2_t indexCurrentChar) {
        if_true_fef(type != stateReader.CurrentElement->Type, "Invalid type to close: %s. Current type: %s.", get_value_label(type).begin(), get_value_label(stateReader.CurrentElement->Type).begin());
        stateReader.CurrentElement->Span.End						= indexCurrentChar;
        stateReader.IndexCurrentElement								= stateReader.CurrentElement->ParentIndex;
        stateReader.CurrentElement									= (-1 != stateReader.IndexCurrentElement) ? &tokens[stateReader.IndexCurrentElement] : nullptr;
        llc_reader_info_printf("Closing expression type: %s. Parent type: %s. Nest level: %i.", get_value_label(type).begin(), stateReader.CurrentElement ? get_value_label(stateReader.CurrentElement->Type).begin() : "UNKNOWN", stateReader.NestLevel);
        --stateReader.NestLevel;
        return 0;
    }

    tplt<tpnm TToken>
    sttc	err_t	readerCloseIfType				(SReaderState<TToken> & stateReader, apod<SToken<TToken>> & tokens, TToken type)	{
        if_null_vif(0, stateReader.CurrentElement, "Cannot close %s. Nothing to close.", get_value_label(type).begin());
        if(type == stateReader.CurrentElement->Type) {
            if_fail_fef(readerCloseType(stateReader, tokens, type, stateReader.IndexCurrentChar), "Failed to close type: %s. Why would this happen?", get_value_label(type).begin());
            return 1;
        }
        return 0;
    }

    tplt<tpnm TToken>
    sttc	err_t	readerOpenLevel		(SReaderState<TToken> & stateReader, apod<SToken<TToken>> & tokens, b8_t isContainer, TToken type, u2_t iChar)	{
        if_fail_fe(stateReader.IndexCurrentElement = tokens.push_back({stateReader.IndexCurrentElement, type, isContainer, {iChar, iChar}}));
        stateReader.CurrentElement		        = &tokens[stateReader.IndexCurrentElement];
        ++stateReader.NestLevel;
        llc_reader_info_printf("Entering expression type: %s. Parent type: %s. Nest level: %u.", get_value_label(stateReader.CurrentElement->Type).begin(), stateReader.CurrentElement ? get_value_label(stateReader.CurrentElement->Type).begin() : "UNKNOWN", stateReader.NestLevel);
        return 0;
    }

    tplT    err_t	readerTreeRebuild		(view<cnst SToken<T>> in_object, apobj<SReaderNode<T>> & out_nodes)								{
        tydf SReaderNode<T> TNode;
        apobj<TNode>    & tree					= out_nodes;
        if_fail_fe(tree.resize(in_object.size()));
        for(u2_t iObject = 0; iObject < tree.size(); ++iObject) { // -- Build all nodes linearly, without assigning the children
            pobj<TNode>	& nodeCurrent		= tree[iObject];
            nodeCurrent->Token			= &in_object[iObject];
            nodeCurrent->Parent			= ((u2_t)nodeCurrent->Token->ParentIndex < tree.size()) ? (TNode*)tree[nodeCurrent->Token->ParentIndex] : nullptr;
            nodeCurrent->ObjectIndex	= iObject;
            nodeCurrent->Children.clear();
            if(nodeCurrent->Parent) {
                if_fail_fe(nodeCurrent->Parent->Children.push_back(nodeCurrent));
            }
        }
        for(u2_t iObject = 0, countNodes = tree.size(); iObject < countNodes; ++iObject) { // -- Remove the key/value wrappers from objects.
            pobj<TNode>				& nodeCurrent										= tree[iObject];
            if_null_fe(nodeCurrent);
            if(false == nodeCurrent->Token->Container)
                continue;
            apobj<TNode>	newChildren;
            if_fail_fe(newChildren.resize(nodeCurrent->Children.size()));
            for(u2_t iChild = 0, countChild = newChildren.size(); iChild < countChild; ++iChild)
                newChildren[iChild]						= nodeCurrent->Children[iChild]->Children[0];
            nodeCurrent->Children					= newChildren;
        }
        rtrn 0;
    }


} // nmsp

#endif // LLC_READER_H_261125
