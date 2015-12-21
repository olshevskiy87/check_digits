\echo Use "CREATE EXTENSION check_digits" to load this file. \quit

CREATE FUNCTION chdig_inn(t text)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
