ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS =	amx_llist.o \
			amx_llist_it.o \
			amx_array.o \
			amx_array_it.o \
			amx_rbuffer.o \
			amx_hash_func.o \
			amx_htable.o \
			amx_htable_it.o \
			amx_variant.o \
			amx_variant_string.o \
			amx_variant_int8.o \
			amx_variant_int16.o \
			amx_variant_int32.o \
			amx_variant_int64.o \
			amx_variant_uint8.o \
			amx_variant_uint16.o \
			amx_variant_uint32.o \
			amx_variant_uint64.o \
			amx_variant_float.o \
			amx_variant_double.o \
			amx_variant_bool.o \
			amx_variant_list.o \
			amx_variant_htable.o \
			amx_variant_fd.o \
			amx_variant_type.o

TARGET = libamx_containers

CFLAGS  += -std=c99 -g -Wall -W -fPIC -I. -I../include -I$(AMX_INCLUDE_DIR)
LDFLAGS +=

ifdef AMX_TEST_COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage
endif

ifdef AMX_BUILD_OPTIMIZE
CFLAGS += $(AMX_BUILD_OPTIMIZE)
endif

all: $(TARGET).so $(TARGET).a

$(TARGET).so: $(OBJECTS)
	$(CC) -Wl,-soname,$(@) -shared -fPIC $(LDFLAGS) -o $(@) $(OBJECTS)

$(TARGET).a: $(OBJECTS)
	$(AR) rcs $(@) $(OBJECTS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<)

clean:
	rm -f $(OBJECTS) $(TARGET).so $(TARGET).a *.d *.gcno *.gcov *.gcda

.PHONY: clean coverage
