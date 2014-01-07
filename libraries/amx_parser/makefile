ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

export PKG_CONFIG_PATH=$(AMX_OUTPUT_DIR)/pkg-config
export AMX_PARSER_VERSION=0.0.1
export DOC_OUTPUT_DIR=$(AMX_OUTPUT_DIR)/documentation/amx_parser/

all: amx_parser.pc
	make -C ./source

amx_parser.pc: amx_parser.pc.m4 $(AMX_BUILD_DIR)/amx.config
	cat $(AMX_BUILD_DIR)/amx.config.m4 $< | m4 > $@

install:
	$(INSTALL) -D -p -m 0644 include/amx_parser/amx_parser_store.h $(AMX_OUTPUT_DIR)/include/amx_parser/amx_parser_store.h
	$(INSTALL) -D -p -m 0644 include/amx_parser.h $(AMX_OUTPUT_DIR)/include/amx_parser.h
	$(INSTALL) -D -m 0755 source/libamx_parser_store.so $(AMX_OUTPUT_DIR)/lib/libamx_parser_store.so
	$(INSTALL) -D -m 0755 source/libamx_parser_store.a $(AMX_OUTPUT_DIR)/lib/libamx_parser_store.a
	$(INSTALL) -D -m 0644 amx_parser.pc $(AMX_OUTPUT_DIR)/pkg-config/amx_parser.pc

doc:
	$(INSTALL) -d $(DOC_OUTPUT_DIR)
	$(DOXYGEN) amx_parser.doxy

test:
	make -C ./test

clean:
	make -C ./source clean
	make -C ./test clean
	rm -f amx_parser.pc

.PHONY: all install documentation test clean
