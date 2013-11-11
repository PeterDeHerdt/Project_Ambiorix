ifndef AMX_BUILD_DIR
$(error AMX_BUILD_DIR not defined)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

export PKG_CONFIG_PATH=$(AMX_OUTPUT_DIR)/pkg-config
export AMX_CONTAINERS_VERSION=0.0.1
export DOC_OUTPUT_DIR=$(AMX_OUTPUT_DIR)/documentation/amx_containers/

all: amx_containers.pc
	make -C ./source

amx_containers.pc: amx_containers.pc.m4 $(AMX_BUILD_DIR)/amx.config
	cat $(AMX_BUILD_DIR)/amx.config.m4 $< | m4 > $@

install:
	for FILE in $$(find include/ -type f); do \
		$(INSTALL) -p -m 0644 -D $${FILE} $(AMX_OUTPUT_DIR)/$${FILE} || exit 1; \
	done
	$(INSTALL) -D -m 0755 source/libamx_containers.so $(AMX_OUTPUT_DIR)/lib/libamx_containers.so
	$(INSTALL) -D -m 0755 source/libamx_containers.a $(AMX_OUTPUT_DIR)/lib/libamx_containers.a
	$(INSTALL) -D -m 0644 amx_containers.pc $(AMX_OUTPUT_DIR)/pkg-config/amx_containers.pc

doc:
	$(INSTALL) -d $(DOC_OUTPUT_DIR)
	$(DOXYGEN) amx_containers.doxy

test:
	make -C ./test

clean:
	make -C ./source clean
	make -C ./test clean
	rm -f amx_containers.pc

.PHONY: all install documentation test clean
