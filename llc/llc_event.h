#include "llc_array_ptr.h"
#include "llc_enum.h"

#include "llc_apod_serialize.h"

#ifndef LLC_EVENT_H_23627
#define LLC_EVENT_H_23627

#define llc_event_printf verbose_printf

namespace llc
{
    tpl_t struct SEventView {
        tdfT(_t);
        tydf    SEventView<T>    TEView;
        T        Type            = {};
        vcu0_t    Data            = {};

        err_t    Save        (au0_t & output)    cnst    {
            if_fail_fe(savePOD (output, Type));
            if_fail_fe(saveView(output, Data));
            rtrn 0;
        }

        err_t    Load        (vcu0_t & input)            {
            if_fail_fe(loadPOD (input, Type));
            if_fail_fe(loadView(input, Data));
            rtrn 0;
        }
    };
    tplTusng SEView                     = SEventView<T>;
    tplTusng TEViewQueue                 = aobj<SEView<T>>;

    tplTusng FEViewHandler            = function<err_t(      SEView<T>&)>;
    tplTusng FEViewHandlerConst        = function<err_t(cnst SEView<T>&)>;
    tplTusng FEventViewHandler        = function<err_t(      SEView<T>&)>;
    tplTusng FEventViewHandlerConst    = function<err_t(cnst SEView<T>&)>;

    tplt<tpnm _tEventType>
    stct SEvent {
        tydf _tEventType            T;
        tydf SEView<T>    TEView;
        tydf SEvent<T>    TEvent;

        T                Type        = {};
        au0_t            Data        = {};

                        SEvent        (cnst TEvent &)                    = dflt;
        cxpr            SEvent        (T type = {})                        : Type(type) {}
                        SEvent        (T type, vcu0_c & data)                : Type(type), Data(data.as<u0_c>()) {}
                        SEvent        (cnst TEView & eventView)            : Type(eventView.Type), Data(eventView.Data) {}

        TEvent&            oper=         (cnst TEvent &)                = dflt;
        TEvent&            oper=         (cnst TEView & eventView)        { Type = eventView.Type; Data = eventView.Data; rtrn *this; }

        oper            TEView        ()                        cnst    { rtrn {Type, Data.as<u0_c>()}; }

        err_t            Save        (au0_t & output)    cnst    {
            if_fail_fe(savePOD(output, Type));
            if_fail_fe(saveView(output, Data));
            rtrn 0;
        }

        err_t    Load        (vcu0_t & input)            {
            if_fail_fe(loadPOD(input, Type));
            if_fail_fe(loadView(input, Data));
            rtrn 0;
        }

        tplt<tpnm _tETypeOther>
        err_t    ExtractChild(SEvent<_tETypeOther> & outputEvent)        cnst    {
            vcu0_t            input                = Data;
            if_fail_fe(outputEvent.Load(input));
            llc_event_printf("%s", get_value_namep(outputEvent.Type)); 
            rtrn 0; 
        }

        tplt<tpnm _tETypeOther>
        err_t    ExtractChild(SEView<_tETypeOther> & outputEvent)    cnst    {
            vcu0_t            input                = Data;
            if_fail_fe(outputEvent.Load(input));
            llc_event_printf("%s", get_value_namep(outputEvent.Type)); 
            rtrn 0; 
        }
    };
    tplTusng    FEventHandler      = function<err_t(     SEvent<T>&)>;
    tplTusng    FEventHandlerConst = function<err_t(cnst SEvent<T>&)>;
    tplTusng    PEvent             = pobj <SEvent<T>>;
    tplTusng    TEventQueue        = apobj<SEvent<T>>;

    tplt<tpnm TParent, tpnm TChild, tpnm _tPOD>
    sttc    err_t    eventWrapChild        (SEvent<TParent> & parentEvent, TChild childType, cnst view<cnst _tPOD> & eventData) {
        SEView<TChild>    childEvent            = {childType, eventData.tplt as<u0_c>()};
        rtrn childEvent.Save(parentEvent.Data);
    }
    tplt<tpnm TParent, tpnm TChild, tpnm _tPOD>
    sttc    err_t    eventEnqueueChild    (TEventQueue<TParent> & eventQueue, TParent parentType, TChild childType, cnst view<cnst _tPOD> & eventData) {
        PEvent<TParent>    parentEvent            = {};
        parentEvent->Type = parentType;
        if_fail_fe(eventWrapChild(*parentEvent, childType, eventData));
        rtrn eventQueue.push_back(parentEvent);
    }
    tplt<tpnm TParent, tpnm TChild, tpnm _tPOD>
    sttc    err_t    eventEnqueueChild    (TEventQueue<TParent> & eventQueue, TParent parentType, TChild childType, cnst _tPOD & childEventDataType) {
        rtrn eventEnqueueChild(eventQueue, parentType, childType, view<cnst _tPOD>{&childEventDataType, 1});
    }
    tplt<tpnm TChild, tpnm TParent>
    sttc    err_t    eventExtractAndHandle    (cnst SEvent<TParent> & parentEvent, cnst function<err_t (cnst SEventView<TChild> &)> & funcHandleChild) {
        SEView<TChild>    childEvent; 
        if_fail_fe(parentEvent.ExtractChild(childEvent)); 
        rtrn funcHandleChild(childEvent);
    }
    tplt<tpnm TParent, tpnm TChild>          stin err_t eventWrapChild    (SEvent<TParent> & parentEvent, TChild childType, vcst_c & eventData)                            { rtrn eventWrapChild(parentEvent, childType, eventData.tplt as<u0_c>()); }
    tplt<tpnm TParent, tpnm TChild>          stin err_t eventWrapChild    (SEvent<TParent> & parentEvent, TChild childType, vstr_c & eventData)                            { rtrn eventWrapChild(parentEvent, childType, eventData.tplt as<u0_c>()); }
    tplt<tpnm TParent, tpnm TChild, u2_t N>  stin err_t eventWrapChild    (SEvent<TParent> & parentEvent, TChild childType, sc_c (&eventData)[N])                          { rtrn eventWrapChild(parentEvent, childType, vcst_t{eventData}); }
    tplt<tpnm TParent, tpnm TChild>          stin err_t eventEnqueueChild (TEventQueue<TParent> & eventQueue, TParent parentType, TChild childType, vcst_c & eventData)    { rtrn eventEnqueueChild(eventQueue, parentType, childType, eventData.tplt as<u0_c>()); }
    tplt<tpnm TParent, tpnm TChild>          stin err_t eventEnqueueChild (TEventQueue<TParent> & eventQueue, TParent parentType, TChild childType, vstr_c & eventData)    { rtrn eventEnqueueChild(eventQueue, parentType, childType, eventData.tplt as<u0_c>()); }
    tplt<tpnm TParent, tpnm TChild, u2_t N>  stin err_t eventEnqueueChild (TEventQueue<TParent> & eventQueue, TParent parentType, TChild childType, sc_c (&eventData)[N])  { rtrn eventEnqueueChild(eventQueue, parentType, childType, vcst_t{eventData}); }

    tydf SEvent            <RESULT>     SEventResult;
    tydf SEventView        <RESULT>     SEViewResult;
    tydf FEventHandler     <RESULT>     FEventResult;
    tydf FEventHandlerConst<RESULT>     FEventResultConst;
    tydf SEvent            <COMMAND>    SEventCommand;
    tydf SEventView        <COMMAND>    SEViewCommand;
    tydf FEventHandler     <COMMAND>    FEventCommand;
    tydf FEventHandlerConst<COMMAND>    FEventCommandConst;
}

#define llc_warning_unhandled_event(eventUnhandled)    warning_printf("Unhandled '%s' event: '%s' (0x%llX)(%lli)(%c)"    , get_enum_namep((eventUnhandled).Type), get_value_namep((eventUnhandled).Type), (u3_t)(eventUnhandled).Type, (s3_t)(eventUnhandled).Type, char((eventUnhandled).Type ? (eventUnhandled).Type : ' '))
#define llc_warning_not_implemented(eventUnhandled) warning_printf("Implement for '%s'! '%s' (0x%llX)(%lli)(%c)"    , get_enum_namep((eventUnhandled).Type), get_value_namep((eventUnhandled).Type), (u3_t)(eventUnhandled).Type, (s3_t)(eventUnhandled).Type, char((eventUnhandled).Type ? (eventUnhandled).Type : ' '))

#endif // LLC_EVENT_H_23627
