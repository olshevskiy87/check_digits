#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "check_digits.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(check_digits_inn);
PG_FUNCTION_INFO_V1(check_digits_okpo);

char *
check_array_of_digits(char* arr, uint8_t len)
{
    uint8_t     i;
    char       *ret = (char *) calloc(25, sizeof(char));

    if (len == 0) {
        return "zero length";
    }

    for (i = 0; i < len; i++) {
        if (!isdigit(arr[i])) {
            sprintf(ret, "invalid symbol \"%c\"", arr[i]);
            return ret;
        }
    }
    return "";
}

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
    char       *err_ret;
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

    err_ret = check_array_of_digits(c_inn, inn_len);
    if (strlen(err_ret)) {
        ereport(ERROR,
                (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg("%s", err_ret)));
    }
    for (i = 0; i < inn_len; i++) {
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

/*
 * Check All-Russian Classifier of Enterprises and Organizations(rus. OKPO).
 *
 * Returns true if the argument is a correct OKPO number.
 * Otherwise, returns false.
 */
Datum
check_digits_okpo(PG_FUNCTION_ARGS)
{
    text       *okpo;
    char       *c_okpo;
    char       *err_ret;
    uint8_t     okpo_len, i, penult_index;
    uint8_t     i_okpo[10];
    uint16_t    mul_sum = 0, rest;

    if (PG_ARGISNULL(0)) {
        PG_RETURN_NULL();
    }

    okpo = PG_GETARG_TEXT_P(0);
    okpo_len = VARSIZE(okpo) - VARHDRSZ;
    if (okpo_len != 8 && okpo_len != 10) {
        ereport(ERROR,
                (errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
                 errmsg("OKPO must contain 8 or 10 digits")));
    }
    c_okpo = text_to_cstring(okpo);

    err_ret = check_array_of_digits(c_okpo, okpo_len);
    if (strlen(err_ret)) {
        ereport(ERROR,
                (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg("%s", err_ret)));
    }
    for (i = 0; i < okpo_len; i++) {
        i_okpo[i] = c_okpo[i] - '0';
    }
    pfree(c_okpo);

    penult_index = okpo_len - 1;
    for (i = 0; i < penult_index; i++) {
        mul_sum += i_okpo[i] * (i + 1);
    }
    rest = mul_sum % 11;
    if (rest > 9) {
        mul_sum = 0;
        for (i = 0; i < penult_index; i++) {
            mul_sum += i_okpo[i] * (i + 3);
        }
        rest = mul_sum % 11;
        if (rest > 9) {
            rest = 0;
        }
    }

    PG_RETURN_BOOL((i_okpo[penult_index] == rest));
}
