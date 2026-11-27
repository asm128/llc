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

    tplt <tpnm _tEvntParent, tpnm _tEvntChild>
    static    err_t    eventWrapChild        (SEvent<_tEvntParent> & parentEvent, _tEvntChild childEventType, vcu0_t eventData) {
        SEView<_tEvntChild>    childEvent            = {childEventType, eventData};
        rtrn childEvent.Save(parentEvent.Data);
    }
    tplt <tpnm _tEvntParent, tpnm _tEvntChild>
    static    err_t    eventEnqueueChild    (TEventQueue<_tEvntParent> & eventQueue, _tEvntParent parentEventType, _tEvntChild childEventType, vcu0_t eventData) {
        PEvent<_tEvntParent>    parentEvent            = {};
        parentEvent->Type = parentEventType;
        if_fail_fe(eventWrapChild(*parentEvent, childEventType, eventData));
        rtrn eventQueue.push_back(parentEvent);
    }
    tplt <tpnm _tEvntParent, tpnm _tEvntChild, tpnm _tPOD>
    static    err_t    eventEnqueueChild    (TEventQueue<_tEvntParent> & eventQueue, _tEvntParent parentEventType, _tEvntChild childEventType, view<cnst _tPOD> eventData) {
        rtrn eventEnqueueChild(eventQueue, parentEventType, childEventType, vcu0_t{(cnst u0_t*)eventData.begin(), eventData.byte_count()});
    }
    tplt <tpnm _tEvntParent, tpnm _tEvntChild, tpnm _tPOD>
    static    err_t    eventEnqueueChild    (TEventQueue<_tEvntParent> & eventQueue, _tEvntParent parentEventType, _tEvntChild childEventType, cnst _tPOD & childEventDataType) {
        rtrn eventEnqueueChild(eventQueue, parentEventType, childEventType, vcu0_t{(cnst u0_t*)&childEventDataType, szof(_tPOD)});
    }
    tplt<tpnm _tChildEvent, tpnm _tParentEvent>
    static    err_t    eventExtractAndHandle    (cnst SEvent<_tParentEvent> & parentEvent, cnst function<err_t (cnst SEventView<_tChildEvent> &)> & funcHandleChild) {
        SEView<_tChildEvent>    childEvent; 
        if_fail_fe(parentEvent.ExtractChild(childEvent)); 
        rtrn funcHandleChild(childEvent);
    }

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