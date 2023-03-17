# snowflake-php-ext

Simple snowflake algorithm

#### install

```
phpize
./configure 
make
make install
echo "extension=snowflake" >> php.ini
```

#### Example

```
var_dump(Snowflake::id());
```