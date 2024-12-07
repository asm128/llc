#include "llc_append_json.h"

namespace llc
{
    stxp vcs PREPENDED_QUOT = LLC_CXS(",\"");
    stxp vcs NOPREPDED_QUOT = LLC_CXS("\"");

    err_t   appendOpenKey   (asc_t & output, vcs k       , bool prepSep)  { return append_strings(output, prepSep ? PREPENDED_QUOT : NOPREPDED_QUOT, k, "\":")              ; }
    err_t   appendKeyValue  (asc_t & output, vcs k, vcs v, bool prepSep)  { return append_strings(output, prepSep ? PREPENDED_QUOT : NOPREPDED_QUOT, k, "\":", v)           ; }
    err_t   appendKeyString (asc_t & output, vcs k, vcs v, bool prepSep)  { return append_strings(output, prepSep ? PREPENDED_QUOT : NOPREPDED_QUOT, k, "\":", '"',  v, '"'); }
    err_t   appendKeyList   (asc_t & output, vcs k, vcs v, bool prepSep)  { return append_strings(output, prepSep ? PREPENDED_QUOT : NOPREPDED_QUOT, k, "\":", '[',  v, ']'); }
    err_t   appendKeyObject (asc_t & output, vcs k, vcs v, bool prepSep)  { return append_strings(output, prepSep ? PREPENDED_QUOT : NOPREPDED_QUOT, k, "\":", '{',  v, '}'); }
}
