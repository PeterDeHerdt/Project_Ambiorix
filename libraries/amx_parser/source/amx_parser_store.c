/****************************************************************************
**
** - DISCLAIMER OF WARRANTY -
**
** THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE SOURCE
** CODE IS WITH YOU. SHOULD THE SOURCE CODE PROVE DEFECTIVE, YOU
** ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
**
** - LIMITATION OF LIABILITY -
**
** IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
** WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES
** AND/OR DISTRIBUTES THE SOURCE CODE, BE LIABLE TO YOU FOR DAMAGES,
** INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
** ARISING OUT OF THE USE OR INABILITY TO USE THE SOURCE CODE
** (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED
** INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE
** OF THE SOURCE CODE TO OPERATE WITH ANY OTHER PROGRAM), EVEN IF SUCH
** HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGES.
**
****************************************************************************/

#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "amx_parser/amx_parser_store.h"

static amx_htable_t amx_parser_types;

static int amx_parser_store_allocate_types()
{
	int retval = -1;
	if (amx_htable_init(&amx_parser_types, AMX_PARSER_STORE_DEFAULT_COUNT) == -1)
	{
		goto exit;
	}
	retval = 0;

exit:
	return retval;
}

static int amx_parser_store_add_parser(amx_parser_store_parser_t *parser)
{
	int result = -1;
	// check if the type is already registered
	amx_htable_it_t *hit = amx_htable_get(&amx_parser_types, parser->name);
	if (hit) // already a type registered with that name
	{
		goto exit;
	}

	if (amx_parser_types.items == 0)
	{
		// nothing allocated yet - pre-allocate all fixed items
		if (amx_parser_store_allocate_types() == -1)
		{
			goto exit;
		}
	}
	// insert in hash table
	amx_htable_it_init(&parser->hit);
	if (amx_htable_insert(&amx_parser_types, parser->name, &parser->hit) == -1)
	{
		goto exit;
	}

	result = 0;

exit:
	return result;
}

static int amx_parser_store_remove_parser(amx_parser_store_parser_t *parser)
{
	int retval = -1;
	amx_htable_it_t *hit = amx_htable_get(&amx_parser_types, parser->name);
	if (!hit)
	{
		// type not found
		goto exit;
	}

	// remove from the table
	amx_htable_it_take(hit);
	amx_htable_it_clean(hit, NULL);

	// if hash table is empty, no more types are registered, clear buffers
	if (amx_parser_types.items == 0)
	{
		// no more registered types - clean buffers
		amx_htable_clean(&amx_parser_types, NULL);
	}

	retval = 0;

exit:
	return retval;
}

int amx_parser_store_parser_init(amx_parser_store_parser_t *parser, const char *name)
{
	int retval = -1;
	if(!parser || !name)
	{
		goto exit;
	}

	parser->name = strdup(name);
	if (!(parser->name))
	{
		goto exit;
	}
	parser->parse = NULL;
	parser->verify = NULL;

	amx_htable_it_init(&parser->hit);
	retval = 0;

exit:
	return retval;
}

void amx_parser_store_parser_clean(amx_parser_store_parser_t *parser)
{
	if (!parser)
	{
		return;
	}
	amx_htable_it_take(&parser->hit);
	amx_htable_it_clean(&parser->hit, NULL);
	free(parser->name);
}

int amx_parser_store_parser_new(amx_parser_store_parser_t **parser, const char *name)
{
	int retval = -1;
	if(!parser || !name)
	{
		goto exit;
	}

	*parser = calloc(1, sizeof(amx_parser_store_parser_t));
	if (!(*parser))
	{
		goto exit;
	}

	retval = amx_parser_store_parser_init(*parser,name);

exit:
	return retval;
}

void amx_parser_store_parser_delete(amx_parser_store_parser_t **parser)
{
	if (!parser || !*parser)
	{
		return;
	}
	amx_parser_store_parser_clean(*parser);
	free(*parser);
	*parser = NULL;
}

int amx_parser_store_parser_set_parse_function(amx_parser_store_parser_t *parser, amx_parser_store_parse_fn_t parse)
{
	int retval = -1;
	if (!parser)
	{
		goto exit;
	}

	parser->parse = parse;
	retval = 0;

exit:
	return retval;
}

int amx_parser_store_parser_set_verify_function(amx_parser_store_parser_t *parser, amx_parser_store_verify_fn_t verify)
{
	int retval = -1;
	if (!parser)
	{
		goto exit;
	}

	parser->verify = verify;
	retval = 0;

exit:
	return retval;
}

int amx_parser_store_register_parser(amx_parser_store_parser_t *parser)
{
	int retval = -1;
	if (!parser)
	{
		goto exit;
	}
	retval = amx_parser_store_add_parser(parser);

exit:
	return retval;
}

int amx_parser_store_unregister_parser(amx_parser_store_parser_t *parser)
{
	int retval = -1;
	if (!parser)
	{
		goto exit;
	}
	retval = amx_parser_store_remove_parser(parser);

exit:
	return retval;
}

int amx_parser_store_parse_fd(int fd, const char *name, amx_var_t *result)
{
	int retval = -1;
	if (!fd || !result || !name) // todo: if no name: figure out the name based on extension or content
	{
		goto exit;
	}

	amx_htable_it_t *hit = amx_htable_get(&amx_parser_types, name);
	if (!hit)
	{
		goto exit;
	}

	amx_parser_store_parser_t *parser = amx_htable_it_get_data(hit, amx_parser_store_parser_t, hit);
	if(parser->parse)
	{
		retval = parser->parse(fd, result);
	}

exit:
	return retval;
}

int amx_parser_store_parse_file(const char *file, const char *name, amx_var_t *result)
{
	int retval = -1;

	if(!file)
	{
		goto exit;
	}

	int fd = open(file, O_RDONLY);
	if(fd == -1)
	{
		goto exit;
	}

	retval = amx_parser_store_parse_fd(fd, name, result);

	close(fd);
exit:
	return retval;
}

int amx_parser_store_verify_fd(int fd, const char *name, bool *result)
{
	int retval = -1;
	if (!fd || !result || !name) // todo: if no name: figure out the name based on extension or content
	{
		goto exit;
	}

	amx_htable_it_t *hit = amx_htable_get(&amx_parser_types, name);
	if (!hit)
	{
		goto exit;
	}

	amx_parser_store_parser_t *parser = amx_htable_it_get_data(hit, amx_parser_store_parser_t, hit);
	if(parser->verify)
	{
		retval = parser->verify(fd, result);
	}

exit:
	return retval;
}

int amx_parser_store_verify_file(const char *file, const char *name, bool *result)
{
	int retval = -1;
	int fd = open(file, O_RDONLY);
	if(fd == -1)
	{
		goto exit;
	}

	retval = amx_parser_store_verify_fd(fd, name, result);

	close(fd);
exit:
	return retval;
}
