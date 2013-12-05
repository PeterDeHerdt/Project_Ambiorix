ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

OBJECTS = malloc.o

TARGET = libglibcmalloc

CFLAGS  += -O3 -D_GNU_SOURCE -g -Wall -W -fPIC
LDFLAGS +=

all: $(TARGET).so $(TARGET).a

install:
	$(INSTALL) -D -m 0755 libglibcmalloc.so $(AMX_OUTPUT_DIR)/lib/libglibcmalloc.so
	$(INSTALL) -D -m 0755 libglibcmalloc.a $(AMX_OUTPUT_DIR)/lib/libglibcmalloc.a

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
