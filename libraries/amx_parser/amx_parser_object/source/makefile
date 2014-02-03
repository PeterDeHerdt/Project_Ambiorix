ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS = .libs/amx_parser_object_context.o \
          .libs/lex.amx_parser_object_.o \
          .libs/amx_parser_object.tab.o \
		  
TARGET = libamx_parser_object

CFLAGS  += -std=c99 -D_POSIX_SOURCE -g -Wall -W -fPIC -I. -I../include -I$(AMX_INCLUDE_DIR)
LDFLAGS += -L$(AMX_LIB_DIR) $(shell pkg-config --libs amx_containers)

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

ifdef AMX_BUILD_OPTIMIZE
CFLAGS += $(AMX_BUILD_OPTIMIZE)
endif

all: $(TARGET).so

$(TARGET).so: $(OBJECTS)
	$(CC) -Wl,-soname,$(@) -shared -fPIC $(LDFLAGS) -o $(@) $(OBJECTS)

-include $(OBJECTS:.o=.d)

.libs/lex.amx_parser_object_.c: amx_parser_object.l
	flex -o .libs/lex.amx_parser_object_.c amx_parser_object.l

.libs/amx_parser_object.tab.c .libs/amx_parser_object.tab.h: amx_parser_object.y
	mkdir -p .libs
	bison --verbose -o .libs/amx_parser_object.tab.c amx_parser_object.y

.libs/%.o: %.c .libs/amx_parser_object.tab.h .libs/amx_parser_object.tab.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(TARGET).so *.d *.gcno *.gcov *.gcda
	rm -f .libs/*

.PHONY: clean coverage
