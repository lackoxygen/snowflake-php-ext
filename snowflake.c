/* snowflake extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_snowflake.h"
#include "snowflake_builder.c"
#include <string.h>
/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ snowflake_class_methods[]
 */
static const zend_function_entry snowflake_class_methods[] = {
        PHP_ME(Snowflake, id, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_FE_END
};
/* }}} */

zend_class_entry *Snowflake_ce;

/* {{{ Snowflake::id
 */
PHP_METHOD(Snowflake, id) {
    snowflake_id_t id = generate_id();
    char str_id[50];
    sprintf(str_id, "%lld", id);

    RETVAL_STRING(str_id);
}

/* }}} */


/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(snowflake)
{
#if defined(ZTS) && defined(COMPILE_DL_SNOWFLAKE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(snowflake)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "snowflake support", "enabled");
    php_info_print_table_row(2, "version", PHP_SNOWFLAKE_VERSION);
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(snowflake)
{
    REGISTER_STRING_CONSTANT("SNOWFLAKE_VERSION", PHP_SNOWFLAKE_VERSION, CONST_CS | CONST_PERSISTENT);
    zend_class_entry Snowflake_entry;
    INIT_CLASS_ENTRY(Snowflake_entry, "Snowflake", snowflake_class_methods);
    Snowflake_ce = zend_register_internal_class_ex(&Snowflake_entry, NULL);//注册类
    return SUCCESS;
}
/* }}} */


/* {{{ snowflake_module_entry
 */
zend_module_entry snowflake_module_entry = {
	STANDARD_MODULE_HEADER,
	"snowflake",					/* Extension name */
    snowflake_class_methods,		/* zend_function_entry */
    PHP_MINIT(snowflake),			/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(snowflake),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(snowflake),			/* PHP_MINFO - Module info */
	PHP_SNOWFLAKE_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SNOWFLAKE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(snowflake)
#endif
