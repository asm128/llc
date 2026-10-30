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

        inline  string      ()                              = default;
        inline  string      (vcst_t other)                  : asc_t(other)                                          {}
//        inline  string      (sc_c * unsafe)                 : asc_t(vcst_t{unsafe, (u2_t)-1})                       {}
#ifdef LLC_ARDUINO
        inline  string      (const String & other)          : asc_t(vcst_t{other.begin(), (u2_t)other.length()})    {}
        inline  string      (const IPAddress & other)       : string(other.toString())                              {}

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

    }; // struct

#ifdef LLC_ARDUINO
    ndsi string  str (const IPAddress & s)   { return s.toString(); }
#endif

	err_t		camelCase	(vcst_t input, string & camelCased);

} // namespace

#endif // LLC_STRING_H
