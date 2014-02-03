#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <amx_parser.h>

#include "../include/amx_parser_object_context.h"

static amx_parser_store_parser_t object_parser;

amx_parser_object_t *amx_parser_object_create(int fd) {
    amx_parser_object_t *object = calloc(1,sizeof(amx_parser_object_t));

    if(!object) {
        printf("Could not create object\n");
        return NULL;
    }
    object->fd = fd;
    amx_parser_object_create_lex(object);

    return object;
}

void amx_parser_object_destroy(amx_parser_object_t *object) {    
    amx_parser_object_destroy_lex(object->scanner);
    free(object);
}

int amx_parser_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}


int  amx_parser_object_switch_parser(amx_parser_object_t *object, const char *context, const char *content) {
    (void)object;
    printf("Switching to context %s with data %s\n", context, content);
    /*
    // todo: change this strcmp to lookup in table for registered parsers
    if(strcmp(context,"MultiLineComment")==0) {
       amx_parser_comment_t *ct = amx_parser_comment_create(content);
       printf("Creating comment context %p\n",ct);
       amx_parser_comment_parse(ct);
       printf("Destroying comment context %p\n",ct);
       amx_parser_comment_destroy(ct);
     }*/
    return 0;
}



int amx_parser_object_parse_fd(int fd, amx_var_t *result) {
    if(!fd || !result)
    {
        return -1;
    }

    amx_parser_object_t *context = amx_parser_object_create(fd);
    if(!context)
    {
        return -1;
    }

    if(!amx_parser_object_parse(context))
    {
        return -1;
    }
    //retrieve result
    amx_parser_object_destroy(context);



    return 0;
}

int amx_parser_object_verify_fd(int fd, bool *result)
{
    if(!fd || !result)
    {
        return -1;
    }

    amx_parser_object_t *context = amx_parser_object_create(fd);
    if(!context)
    {
        return -1;
    }

    if(!amx_parser_object_parse(context))
    {
        *result = false;
        return 0;
    }
    amx_parser_object_destroy(context);
    *result = true;
    return 0;
}


__attribute__((constructor)) static void amx_var_types_init()
{
    // add the object parser
    amx_parser_store_parser_init(&object_parser, "amx object parser");
    amx_parser_store_parser_set_parse_function(&object_parser,amx_parser_object_parse_fd);
    amx_parser_store_parser_set_verify_function(&object_parser,amx_parser_object_verify_fd);

}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
    // remove the object parser
    amx_parser_store_parser_clean(&object_parser);
}
