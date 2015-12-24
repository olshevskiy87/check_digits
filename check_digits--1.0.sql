\echo Use "CREATE EXTENSION check_digits" to load this file. \quit

CREATE FUNCTION check_digits_inn(inn text)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
