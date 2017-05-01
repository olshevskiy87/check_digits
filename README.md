check digits
============

[![PGXN version](https://badge.fury.io/pg/check_digits.svg)](http://pgxn.org/dist/check\_digits)

Functions to check various identification numbers.

Functions
---------

* `check_digits_inn(inn text)`

Checks russian Individual Taxpayer Number ([INN](https://goo.gl/dK7BZs))

* `check_digits_okpo(okpo text)`

Checks All-Russian Classifier of Enterprises and Organizations ([OKPO](https://goo.gl/gWldnP))

* `check_digits_ogrn(ogrn text)`

Check Primary State Registration Number ([OGRN](https://goo.gl/Abd9oc))

* `check_digits_snils(snils text)`

Check Insurance Number of Individual Ledger Account ([SNILS](https://goo.gl/NxeFKV))

* `check_digits_isbn(isbn text)`

Check [International Standard Book Number](https://goo.gl/0LINxR) (ISBN-10 and ISBN-13)

Installation
------------

Build with

```
$ make
```

To make the regression tests run

```
$ make installcheck
```

Install with

```
$ sudo make install
```

At last to create the extension type in psql

```
$$ CREATE EXTENSION check_digits;
```

License
-------

Copyright (c) 2015 - 2017 Dmitriy Olshevskiy. MIT LICENSE.

See LICENSE for details.
