#ifndef CSVSTAT_ERR_H
#define CSVSTAT_ERR_H

typedef enum {
    CSVSTAT_OK = 0,

    // CLI / usage
    CSVSTAT_EARG = 2,

    // I/O
    CSVSTAT_EIO = 3,

    // Parsing / format
    CSVSTAT_EFORMAT = 4,
    CSVSTAT_ENOCOL = 5,  // requested column not found

    // Memory
    CSVSTAT_ENOMEM = 6,

    // Internal / unexpected
    CSVSTAT_EINTERNAL = 10,
} CsvStatErr;

const char *csvstat_err_str(CsvStatErr e);

#endif