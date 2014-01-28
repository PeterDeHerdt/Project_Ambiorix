ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

export PKG_CONFIG_PATH=$(AMX_OUTPUT_DIR)/pkg-config
export AMX_PARSER_OBJECT_VERSION=0.0.1
export DOC_OUTPUT_DIR=$(AMX_OUTPUT_DIR)/documentation/amx_parser_object/

all: 
	make -C ./source

install:
	$(INSTALL) -D -m 0755 source/libamx_parser_object.so $(AMX_OUTPUT_DIR)/lib/libamx_parser_object.so

doc:
	$(INSTALL) -d $(DOC_OUTPUT_DIR)
	$(DOXYGEN) amx_parser_object.doxy

test:
	make -C ./test

clean:
	make -C ./source clean
	make -C ./test clean

.PHONY: all install documentation test clean
