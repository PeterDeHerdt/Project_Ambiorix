#ifndef AMX_PARSER_OBJECT_CONTEXT
#define AMX_PARSER_OBJECT_CONTEXT

#include <stdio.h>
#include <amx_containers.h>

typedef struct _amx_parser_object_t {
    void* scanner;
    int fd;
} amx_parser_object_t;

enum amx_parser_object_tokens_t
{
    token_object,
    token_signals,
    token_slots,
    token_eof
};

amx_parser_object_t *amx_parser_object_create(int fd);
void amx_parser_object_destroy(amx_parser_object_t *object);
int  amx_parser_object_parse(amx_parser_object_t *object);
int  amx_parser_object_switch_parser(amx_parser_object_t *object, const char *context, const char *content);

void amx_parser_object_create_lex(amx_parser_object_t *object);
void amx_parser_object_destroy_lex(void *scanner);

int amx_parser_object_printf(const char *format, ...);

int amx_parser_object_parse_fd(int fd, amx_var_t *result);
int amx_parser_object_verify_fd(int fd, bool *result);

#endif // AMX_PARSER_OBJECT_CONTEXT
