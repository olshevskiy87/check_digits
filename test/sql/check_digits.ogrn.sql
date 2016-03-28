CREATE EXTENSION IF NOT EXISTS check_digits;

-- should be true
select check_digits_ogrn('1037739010891');
select check_digits_ogrn('304500116000157');

-- should be false
select check_digits_ogrn('302500116000353');

-- should fail with error
-- by length
select check_digits_ogrn('30450011600015712');
select check_digits_ogrn('10373901091');
-- by wrong characters
select check_digits_ogrn('1037739O10891');
