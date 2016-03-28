CREATE EXTENSION IF NOT EXISTS check_digits;

-- should be true
select check_digits_inn('500100732259');
select check_digits_inn('7830002293');

-- should be false
select check_digits_inn('500100732258');

-- should fail with error
-- by length
select check_digits_inn('5001007322581240');
select check_digits_inn('40683635');
-- by wrong characters
select check_digits_inn('r00100732259');
