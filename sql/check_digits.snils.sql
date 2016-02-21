CREATE EXTENSION IF NOT EXISTS check_digits;

-- should be true
select check_digits_snils('11223344595');
select check_digits_snils('086-754-303 00');

-- should be false
select check_digits_snils('11323344595');
select check_digits_snils('921-182-904 18');

-- should fail with error
-- by length
select check_digits_snils('1122334459505');
select check_digits_snils('21-182-904 18');
