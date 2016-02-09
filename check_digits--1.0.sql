\echo Use "CREATE EXTENSION check_digits" to load this file. \quit

CREATE FUNCTION check_digits_inn(inn text)
RETURNS boolean
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION check_digits_okpo(okpo text)
RETURNS boolean
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
