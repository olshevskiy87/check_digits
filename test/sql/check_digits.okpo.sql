CREATE EXTENSION IF NOT EXISTS check_digits;

-- should be true
select check_digits_okpo('47296611');
select check_digits_okpo('0154489581');

-- should be false
select check_digits_okpo('0193273379');

-- should fail with error
-- by length
select check_digits_okpo('1000073254140');
select check_digits_okpo('308363');
-- by wrong characters
select check_digits_okpo('01G4489S81');
