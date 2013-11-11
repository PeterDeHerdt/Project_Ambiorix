ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

export PKG_CONFIG_PATH=$(AMX_OUTPUT_DIR)/pkg-config
export AMX_LOG_VERSION=0.0.1
export DOC_OUTPUT_DIR=$(AMX_OUTPUT_DIR)/documentation/amx_log/

all: amx_log.pc
	make -C ./source

amx_log.pc: amx_log.pc.m4 $(AMX_BUILD_DIR)/amx.config
	cat $(AMX_BUILD_DIR)/amx.config.m4 $< | m4 > $@

install:
	$(INSTALL) -D -p -m 0644 include/amx_log/amx_log_core.h $(AMX_OUTPUT_DIR)/include/amx_log/amx_log_core.h
	$(INSTALL) -D -p -m 0644 include/amx_log/amx_log_zones.h $(AMX_OUTPUT_DIR)/include/amx_log/amx_log_zones.h
	$(INSTALL) -D -p -m 0644 include/amx_log/amx_log_macro.h $(AMX_OUTPUT_DIR)/include/amx_log/amx_log_macro.h
	$(INSTALL) -D -p -m 0644 include/amx_log.h $(AMX_OUTPUT_DIR)/include/amx_log.h
	$(INSTALL) -D -m 0755 source/libamx_log.so $(AMX_OUTPUT_DIR)/lib/libamx_log.so
	$(INSTALL) -D -m 0755 source/libamx_log.a $(AMX_OUTPUT_DIR)/lib/libamx_log.a
	$(INSTALL) -D -m 0644 amx_log.pc $(AMX_OUTPUT_DIR)/pkg-config/amx_log.pc

doc:
	$(INSTALL) -d $(DOC_OUTPUT_DIR)
	$(DOXYGEN) amx_log.doxy

test:
	make -C ./test

clean:
	make -C ./source clean
	make -C ./test clean
	rm -f amx_log.pc

.PHONY: all install documentation test clean
