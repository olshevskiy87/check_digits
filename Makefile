MODULE_big   = check_digits
OBJS         = src/check_digits.o

EXTENSION    = check_digits
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | \
               sed -e "s/default_version[[:space:]]*=[[:space:]]*'\\([^']*\\)'/\\1/")
DATA         = sql/check_digits--0.3.1.sql
MODULES      = check_digits

TESTS        = $(wildcard test/sql/*.sql)
REGRESS      = $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test

PG_CONFIG    = pg_config

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

dist:
	git archive --format zip --prefix=$(EXTENSION)-$(EXTVERSION)/ \
		-o $(EXTENSION)-$(EXTVERSION).zip HEAD
