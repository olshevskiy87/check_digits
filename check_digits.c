#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include <string.h>
#include <ctype.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(check_digits_inn);

Datum
check_digits_inn(PG_FUNCTION_ARGS) {
    text    *inn;
    int      inn_len, i;
    char    *c_inn;

    if (PG_ARGISNULL(0)) {
        PG_RETURN_NULL();
    }

    inn = PG_GETARG_TEXT_P(0);
    inn_len = VARSIZE(inn) - VARHDRSZ;
    if (inn_len != 10 && inn_len != 12) {
        ereport(ERROR,
                (errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
                 errmsg("INN must contain 10 or 12 digits")));
    }
    c_inn = text_to_cstring(inn);
    for (i = 0; i < inn_len; i++) {
        if (!isdigit(c_inn[i])) {
            ereport(ERROR,
                    (errcode(ERRCODE_DATA_EXCEPTION),
                     errmsg("invalid symbol \"%c\"", c_inn[i])));
        }
    }
    pfree(c_inn);

    PG_RETURN_INT32(inn_len);
}
