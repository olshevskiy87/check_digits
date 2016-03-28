check digits
============

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

Copyright (c) 2015 - 2016 Dmitriy Olshevskiy. MIT LICENSE.
See LICENSE for details.
