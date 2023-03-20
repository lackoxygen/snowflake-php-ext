# snowflake-php-ext

Simple snowflake algorithm

#### install

```
git clone https://github.com/lackoxygen/snowflake-php-ext.git

cd snowflake-php-ext

phpize

./configure 

make

make install

echo "\nextension=snowflake" >> php.ini
```

#### Example

```
var_dump(\Lackoxygen\Snowflake::id());
```