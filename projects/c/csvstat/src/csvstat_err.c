#include "csvstat_err.h"

const char *csvstat_err_str(CsvStatErr e) {
    switch (e) {
        case CSVSTAT_OK:        return "ok";
        case CSVSTAT_EARG:      return "invalid arguments";
        case CSVSTAT_EIO:       return "I/O error";
        case CSVSTAT_EFORMAT:   return "invalid CSV format";
        case CSVSTAT_ENOCOL:    return "column not found";
        case CSVSTAT_ENOMEM:    return "out of memory";
        case CSVSTAT_EINTERNAL: return "internal error";
        default:                return "unknown error";
    }
}