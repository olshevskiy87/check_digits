#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include <string.h>
#include <ctype.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(chdig_inn);

Datum chdig_inn(PG_FUNCTION_ARGS) {
    text    *t;
    int      t_len, i;
    char    *c_t;

    if (PG_ARGISNULL(0)) {
        PG_RETURN_NULL();
    }

    t = PG_GETARG_TEXT_P(0);
    t_len = VARSIZE(t) - VARHDRSZ;
    if (t_len != 10 && t_len != 12) {
        ereport(ERROR,
                (errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
                 errmsg("INN must contain 10 or 12 digits")));
    }
    c_t = text_to_cstring(t);
    for (i = 0; i < t_len; i++) {
        if (!isdigit(c_t[i])) {
            ereport(ERROR,
                    (errcode(ERRCODE_DATA_EXCEPTION),
                     errmsg("invalid symbol \"%c\"", c_t[i])));
        }
    }
    pfree(c_t);

    PG_RETURN_INT32(t_len);
}
