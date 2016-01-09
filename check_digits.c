#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include <string.h>
#include <ctype.h>
#include <stdint.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(check_digits_inn);

/*
 * Check Individual Taxpayer Number (rus. INN) digits.
 *
 * Returns true if the argument is a correct INN number.
 * Otherwise, returns false.
 */
Datum
check_digits_inn(PG_FUNCTION_ARGS)
{
    text       *inn;
    uint8_t     inn_len, i;
    char       *c_inn;
    uint8_t     i_inn[12];
    bool        is_correct = false;
    uint16_t    mul_sum, rest;

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
        i_inn[i] = c_inn[i] - '0';
    }
    pfree(c_inn);

    // is physical person
    if (inn_len == 10) {
        mul_sum = i_inn[0]*2 + i_inn[1]*4 + i_inn[2]*10
                + i_inn[3]*3 + i_inn[4]*5 + i_inn[5]*9
                + i_inn[6]*4 + i_inn[7]*6 + i_inn[8]*8;
        elog(DEBUG1, "check_digits_inn: mul_sum [%d]", mul_sum);
        rest = mul_sum % 11;
        if (rest > 9) {
            rest = mul_sum % 10;
        }
        elog(DEBUG1, "check_digits_inn: rest [%d]", rest);
        is_correct = (i_inn[9] == rest);

    // is juridical person
    } else {
        // check digit (1)
        mul_sum = i_inn[0]*7 + i_inn[1]*2 + i_inn[2]*4
                + i_inn[3]*10 + i_inn[4]*3 + i_inn[5]*5
                + i_inn[6]*9 + i_inn[7]*4 + i_inn[8]*6
                + i_inn[9]*8;
        elog(DEBUG1, "check_digits_inn: check digit (1). "
                     "mul_sum [%d]", mul_sum);
        rest = mul_sum % 11;
        if (rest > 9) {
            rest = mul_sum % 10;
        }
        elog(DEBUG1, "check_digits_inn: check digit (1). "
                     "rest [%d]", rest);
        if (i_inn[10] == rest) {
            // check digit (2)
            mul_sum = i_inn[0]*3 + i_inn[1]*7 + i_inn[2]*2
                    + i_inn[3]*4 + i_inn[4]*10 + i_inn[5]*3
                    + i_inn[6]*5 + i_inn[7]*9 + i_inn[8]*4
                    + i_inn[9]*6 + i_inn[10]*8;
            elog(DEBUG1, "check_digits_inn: check digit (2). "
                         "mul_sum [%d]", mul_sum);
            rest = mul_sum % 11;
            if (rest > 9) {
                rest = mul_sum % 10;
            }
            elog(DEBUG1, "check_digits_inn: check digit (2). "
                         "rest [%d]", rest);
            is_correct = (i_inn[11] == rest);
        }
    }
    PG_RETURN_BOOL(is_correct);
}
