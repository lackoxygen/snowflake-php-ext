--TEST--
snowflake_test_id() Basic test
--SKIPIF--
<?php
if (!extension_loaded('snowflake')) {
	echo 'skip';
}
?>
--FILE--
<?php
Snowflake::id();
?>
--EXPECT--