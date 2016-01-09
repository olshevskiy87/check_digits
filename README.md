# check_digits
Functions to check various identification numbers.

### Functions:
* check_digits_inn(inn text)

Checks russian Individual Taxpayer Number ([INN](https://www.nalog.ru/eng/exchinf/inn/))

## INSTALLATION
### Setup
Build with
```
make
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

## LICENSE
Copyright (c) 2015 - 2016 Dmitriy Olshevskiy. MIT LICENSE.
See LICENSE.md for details.
