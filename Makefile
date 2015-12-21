MODULES = check_digits
MODULE_big = check_digits

EXTENSION = check_digits
OBJS = check_digits.o
DATA = check_digits--1.0.sql
PGFILEDESC = "check_digits - functions to check various identification numbers"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
