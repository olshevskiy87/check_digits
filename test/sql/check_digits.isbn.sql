CREATE EXTENSION IF NOT EXISTS check_digits;

-- should be true
select check_digits_isbn('1451627289');
select check_digits_isbn('978-1-934356-98-2');

-- should be false
select check_digits_isbn('5-93286-093-8');
select check_digits_isbn('5-19-036976-X');

-- should fail with error
-- by length
select check_digits_isbn('978-5-271-1611-8');
select check_digits_isbn('978-0-752860436-5');
