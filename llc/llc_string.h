#include "llc_array_pod.h"
#ifdef LLC_ARDUINO
#   include <IPAddress.h>
#endif // LLC_ARDUINO

#ifndef LLC_STRING_H
#define LLC_STRING_H

namespace llc
{

    struct string : asc_t {
        using   asc_t  ::Data;
        using   asc_t  ::Count;
        using   asc_t  ::Size;

        bool    operator==  (const view<sc_c> & other) const {
            if(Count != other.size())
                return false;
            if(begin() == other.begin())
                return true;
            return ::llc::equal(other.begin(), begin(), Count);
        }
        bool    operator!=  (const view<sc_c> & other) const { return !operator==(other); }
        bool    operator==  (const string & other) const     { return operator==(view<sc_c>{other.begin(), other.size()}); }
        bool    operator!=  (const string & other) const     { return !operator==(other); }
        tplt<size_t buflen>
        bool    operator==  (sc_c (&other)[buflen]) const    { return operator==(vcst_t{other}); }
        tplt<size_t buflen>
        bool    operator!=  (sc_c (&other)[buflen]) const    { return !operator==(other); }

        tplt<size_t buflen>
        inline  string      (sc_c (&other)[buflen])         : asc_t(vcst_t{other})                                  { initialize(); }
        inline  string      (vs0_c & other)                 : asc_t(other)                                          { initialize(); }
        inline  string      (asc_c & other)                 : asc_t(other)                                          { initialize(); }
        inline  string      (vcst_t other)                  : asc_t(other)                                          { initialize(); }
        inline  string      (sc_c * unsafe)                 : asc_t(vcst_t{unsafe, (u2_t)-1})                       { initialize(); }
        inline  string      ()                              = default;
#ifdef LLC_ARDUINO
        inline  string      (const String & other)          : asc_t(vcst_t{other.begin(), (u2_t)other.length()})    { initialize(); }
        inline  string      (const IPAddress & other)       : string(other.toString())                              { initialize(); }

#   ifdef LLC_ESP8266
        ndin operator   String      ()  const   { return String(Data); }
#   else
        ndin operator   String      ()  const   { return String(Data, Count); }
#   endif
#endif
        ndin operator   vcst_t      ()  const   { return {Data, Count}; }
        ndin operator   const char* ()  const   { return vcst_t{Data, Count}; }

        const char*     begin       ()  const   { return vcst_t{Data, Count}.begin(); }
        const char*     end         ()  const   { return vcst_t{Data, Count}.end  (); }
        char*           begin       ()          { return Data; }
        char*           end         ()          { return Data + Count; }

        err_t           initialize  ()          {
            if_true_block_logf(error_printf, (0 == Data) && (Count || Size), Count = Size = 0, "error|Count:%i, Size:%i.", Count, Size);
            return Count;
        }
    }; // struct

#ifdef LLC_ARDUINO
    ndsi string  str (const IPAddress & s)   { return s.toString(); }
#endif

	err_t		camelCase	(vcst_t input, string & camelCased);
} // namespace

#endif // LLC_STRING_H
