#pragma once

#include "basicerr.h"
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum basic_value_type {
    BASIC_VALUE_TYPE_NUMBER = 1,
    BASIC_VALUE_TYPE_STRING = 2,
} basic_value_type_t ;

typedef struct basic_value
{
    uint8_t type_ ;
    union {
        char *svalue_ ;
        double nvalue_ ;
    } value ;
} basic_value_t ;

static inline const char *skipSpaces(const char *line)
{
    while (isspace((uint8_t)*line))
        line++ ;

    return line ;
}

extern bool basic_var_get(const char *name, uint32_t *index, basic_err_t *err) ;
extern bool basic_var_destroy(uint32_t index) ;
extern bool basic_var_set_value(uint32_t index, basic_value_t *value, basic_err_t *err) ;
extern bool basic_var_set_array_value(uint32_t index, basic_value_t *value, int *dims, basic_err_t *err) ;
extern basic_value_t *basic_var_get_value(uint32_t index) ;
extern basic_value_t *basic_var_get_array_value(uint32_t index, int *dims, basic_err_t *err) ;
extern const char *basic_var_get_name(uint32_t index) ;
extern bool basic_var_add_dims(uint32_t index, uint32_t dimcnt, int *dims, basic_err_t *err);
extern int basic_var_get_dim_count(uint32_t index, basic_err_t *err) ;
extern bool basic_var_get_dims(uint32_t index, uint32_t *dimcnt, uint32_t *dims, basic_err_t* err);

extern const char *basic_parse_int(const char *line, int *value, basic_err_t *err) ;
extern const char* basic_parse_dims_const(const char* line, int* dimcnt, int* dims, basic_err_t* err);
extern const char* basic_parse_dims_expr(const char* line, int* dimcnt, uint32_t* dims, basic_err_t* err);

extern const char *basic_parse_expr(const char *line, uint32_t *index, basic_err_t *err) ;
extern basic_value_t *basic_eval_expr(uint32_t index, basic_err_t *err);
extern bool basic_destroy_expr(uint32_t index) ;
extern uint32_t basic_expr_to_string(uint32_t ) ;

#ifdef _DUMP_EXPRS_
extern void basic_expr_dump();
#endif