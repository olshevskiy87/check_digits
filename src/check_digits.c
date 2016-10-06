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
PG_FUNCTION_INFO_V1(check_digits_ogrn);
PG_FUNCTION_INFO_V1(check_digits_snils);
PG_FUNCTION_INFO_V1(check_digits_isbn);

char *
check_array_of_digits(char *arr, uint8_t len)
{
	uint8_t		i;
	char	   *ret = (char *) calloc(25, sizeof(char));

	if (len == 0)
	{
		return "zero length";
	}

	for (i = 0; i < len; i++)
	{
		if (!isdigit(arr[i]))
		{
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
	text	   *inn;
	uint8_t		inn_len,
				i,
				i_inn[12];
	char	   *c_inn,
			   *err_ret;
	bool		is_correct = false;
	uint16_t	mul_sum,
				rest;

	if (PG_ARGISNULL(0))
	{
		PG_RETURN_NULL();
	}

	inn = PG_GETARG_TEXT_P(0);
	inn_len = VARSIZE(inn) - VARHDRSZ;
	if (inn_len != 10 && inn_len != 12)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("INN must contain 10 or 12 digits")));
	}
	c_inn = text_to_cstring(inn);

	err_ret = check_array_of_digits(c_inn, inn_len);
	if (strlen(err_ret))
	{
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("%s", err_ret)));
	}
	for (i = 0; i < inn_len; i++)
	{
		i_inn[i] = c_inn[i] - '0';
	}
	pfree(c_inn);

	/* is physical person */
	if (inn_len == 10)
	{
		mul_sum = i_inn[0] * 2 + i_inn[1] * 4 + i_inn[2] * 10
			+ i_inn[3] * 3 + i_inn[4] * 5 + i_inn[5] * 9
			+ i_inn[6] * 4 + i_inn[7] * 6 + i_inn[8] * 8;
		elog(DEBUG1, "check_digits_inn: mul_sum [%d]", mul_sum);
		rest = mul_sum % 11;
		if (rest > 9)
		{
			rest = mul_sum % 10;
		}
		elog(DEBUG1, "check_digits_inn: rest [%d]", rest);
		is_correct = (i_inn[9] == rest);

	}
	/* is juridical person */
	else
	{
		/* check digit (1) */
		mul_sum = i_inn[0] * 7 + i_inn[1] * 2 + i_inn[2] * 4
			+ i_inn[3] * 10 + i_inn[4] * 3 + i_inn[5] * 5
			+ i_inn[6] * 9 + i_inn[7] * 4 + i_inn[8] * 6
			+ i_inn[9] * 8;
		elog(DEBUG1, "check_digits_inn: check digit (1). mul_sum [%d]",
			 mul_sum);
		rest = mul_sum % 11;
		if (rest > 9)
		{
			rest = mul_sum % 10;
		}
		elog(DEBUG1, "check_digits_inn: check digit (1). rest [%d]", rest);
		if (i_inn[10] == rest)
		{
			/* check digit (2) */
			mul_sum = i_inn[0] * 3 + i_inn[1] * 7 + i_inn[2] * 2
				+ i_inn[3] * 4 + i_inn[4] * 10 + i_inn[5] * 3
				+ i_inn[6] * 5 + i_inn[7] * 9 + i_inn[8] * 4
				+ i_inn[9] * 6 + i_inn[10] * 8;
			elog(DEBUG1, "check_digits_inn: check digit (2). mul_sum [%d]",
				 mul_sum);
			rest = mul_sum % 11;
			if (rest > 9)
			{
				rest = mul_sum % 10;
			}
			elog(DEBUG1, "check_digits_inn: check digit (2). rest [%d]", rest);
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
	text	   *okpo;
	char	   *c_okpo,
			   *err_ret;
	uint8_t		okpo_len,
				i,
				penult_index;
	uint8_t		i_okpo[10];
	uint16_t	mul_sum = 0,
				rest;

	if (PG_ARGISNULL(0))
	{
		PG_RETURN_NULL();
	}

	okpo = PG_GETARG_TEXT_P(0);
	okpo_len = VARSIZE(okpo) - VARHDRSZ;
	if (okpo_len != 8 && okpo_len != 10)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("OKPO must contain 8 or 10 digits")));
	}
	c_okpo = text_to_cstring(okpo);

	err_ret = check_array_of_digits(c_okpo, okpo_len);
	if (strlen(err_ret))
	{
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("%s", err_ret)));
	}
	for (i = 0; i < okpo_len; i++)
	{
		i_okpo[i] = c_okpo[i] - '0';
	}
	pfree(c_okpo);

	penult_index = okpo_len - 1;
	for (i = 0; i < penult_index; i++)
	{
		mul_sum += i_okpo[i] * (i + 1);
	}
	rest = mul_sum % 11;
	if (rest > 9)
	{
		mul_sum = 0;
		for (i = 0; i < penult_index; i++)
		{
			mul_sum += i_okpo[i] * (i + 3);
		}
		rest = mul_sum % 11;
		if (rest > 9)
		{
			rest = 0;
		}
	}

	PG_RETURN_BOOL((i_okpo[penult_index] == rest));
}

/*
 * Check Primary State Registration Number (rus. OGRN)
 *
 * Returns true if the argument is a correct OGRN number.
 * Otherwise, returns false.
 */
Datum
check_digits_ogrn(PG_FUNCTION_ARGS)
{
	/* ogrn number without last digit */
	unsigned long long ogrn_num;
	text	   *ogrn;
	char	   *c_ogrn,
			   *err_ret;
	uint8_t		len,
				last_digit;
	uint16_t	rest;

	if (PG_ARGISNULL(0))
	{
		PG_RETURN_NULL();
	}

	ogrn = PG_GETARG_TEXT_P(0);
	len = VARSIZE(ogrn) - VARHDRSZ;
	elog(DEBUG1, "check_digits_ogrn: length [%d]", len);

	if (len != 13 && len != 15)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("OGRN must contain 13 or 15 digits")));
	}
	c_ogrn = text_to_cstring(ogrn);

	err_ret = check_array_of_digits(c_ogrn, len);
	if (strlen(err_ret))
	{
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("%s", err_ret)));
	}

	last_digit = c_ogrn[len - 1] - '0';
	elog(DEBUG1, "check_digits_ogrn: last digit [%d]", last_digit);

	/* remove last digit before cast to LL */
	c_ogrn[len - 1] = '\0';

	sscanf(c_ogrn, "%lld", &ogrn_num);
	elog(DEBUG1, "check_digits_ogrn: ogrn num [%lld]", ogrn_num);

	pfree(c_ogrn);

	rest = ogrn_num % (len - 2);
	if (rest > 9)
	{
		rest = rest % 10;
	}
	elog(DEBUG1, "check_digits_ogrn: rest [%d]", rest);

	PG_RETURN_BOOL((last_digit == rest));
}

/*
 * Check Insurance Number of Individual Ledger Account (rus. SNILS)
 */
Datum
check_digits_snils(PG_FUNCTION_ARGS)
{
	unsigned long long snils_num;
	text	   *p_snils;
	char	   *c_snils,
			   *snils;
	uint8_t		len,
				new_len,
				i,
				weight,
				ch_num;
	uint16_t	mul_sum = 0,
				rest;
	int			i_snils[11];

	if (PG_ARGISNULL(0))
	{
		PG_RETURN_NULL();
	}

	p_snils = PG_GETARG_TEXT_P(0);
	len = VARSIZE(p_snils) - VARHDRSZ;

	snils = (char *) calloc(len, sizeof(char));

	c_snils = text_to_cstring(p_snils);
	pfree(p_snils);
	new_len = 0;
	for (i = 0; i < len; i++)
	{
		if (!isdigit(c_snils[i]))
		{
			continue;
		}
		snils[new_len] = c_snils[i];
		new_len++;
	}
	if (new_len != 11)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("SNILS must contain 11 digits")));
	}

	sscanf(snils, "%lld", &snils_num);
	elog(DEBUG1, "check_digits_snils: snils num [%lld]", snils_num);

	if (snils_num <= SNILS_MIN_VALUE)
	{
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("SNILS must be more than %d", SNILS_MIN_VALUE)));
	}

	for (i = 0; i < new_len; i++)
	{
		i_snils[i] = snils[i] - '0';
	}
	free(snils);

	ch_num = i_snils[new_len - 2] * 10 + i_snils[new_len - 1];
	elog(DEBUG1, "check_digits_snils: check num [%d]", ch_num);

	for (weight = 9, i = 0; weight > 0; weight--)
	{
		mul_sum += i_snils[i++] * weight;
	}
	elog(DEBUG1, "check_digits_snils: mul_sum [%d]", mul_sum);

	rest = mul_sum;
	if (rest == 100 || rest == 101)
	{
		rest = 0;
	}
	else if (rest > 101)
	{
		rest = mul_sum % 101;
		if (rest == 100 || rest == 101)
		{
			rest = 0;
		}
	}
	elog(DEBUG1, "check_digits_snils: rest [%d]", rest);

	PG_RETURN_BOOL((rest == ch_num));
}

/*
 * Check International Standard Book Number (ISBN-10 and ISBN-13)
 */
Datum
check_digits_isbn(PG_FUNCTION_ARGS)
{
	text	   *p_isbn;
	char	   *c_isbn,
			   *isbn,
				isbn_symbol;
	uint8_t		len,
				new_len,
				i,
				weight;
	uint16_t	mul_sum = 0,
				rest;
	int			i_isbn[13];
	bool		is_x_char,
				is_isbn_10;

	if (PG_ARGISNULL(0))
	{
		PG_RETURN_NULL();
	}

	p_isbn = PG_GETARG_TEXT_P(0);
	len = VARSIZE(p_isbn) - VARHDRSZ;

	isbn = (char *) calloc(len, sizeof(char));

	c_isbn = text_to_cstring(p_isbn);
	pfree(p_isbn);
	new_len = 0;
	is_x_char = false;
	for (i = 0; i < len; i++)
	{
		isbn_symbol = c_isbn[i];

		if (isdigit(isbn_symbol))
		{
			isbn[new_len] = c_isbn[i];
		}
		else if (isbn_symbol == 'x' || isbn_symbol == 'X')
		{
			isbn[new_len] = 'x';
			is_x_char = true;
		}
		else
		{
			continue;
		}
		new_len++;
	}
	if (new_len != 10 && new_len != 13)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("ISBN must contain 10 or 13 digits")));
	}

	elog(DEBUG1, "check_digits_isbn: parsed isbn number [%s]", isbn);

	for (i = 0; i < new_len; i++)
	{
		i_isbn[i] = (isbn[i] != 'x' ? isbn[i] - '0' : 10);
	}
	free(isbn);

	is_isbn_10 = (new_len == 10);
	if (is_isbn_10)
	{
		elog(DEBUG1, "ISBN-10 detected");
		for (weight = 10, i = 0; weight > 0; weight--, i++)
		{
			mul_sum += i_isbn[i] * weight;
		}
	}
	else
	{
		elog(DEBUG1, "ISBN-13 detected");
		if (is_x_char) {
			ereport(ERROR,
					(errcode(ERRCODE_DATA_EXCEPTION),
					 errmsg("ISBN-13 could not contain symbol \"x\"")));
		}
		for (i = 0; i < 13; i++)
		{
			mul_sum += i_isbn[i] * (i % 2 ? 3 : 1);
		}
	}
	elog(DEBUG1, "check_digits_isbn: mul_sum [%d]", mul_sum);

	rest = mul_sum % (is_isbn_10 ? 11 : 10);
	elog(DEBUG1, "check_digits_isbn: rest [%d]", rest);

	PG_RETURN_BOOL((rest == 0));
}
