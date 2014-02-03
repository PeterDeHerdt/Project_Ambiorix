ifndef AMX_ROOT_DIR
$(error AMX_ROOT_DIR not defined)
endif

ifndef AMX_BUILD_DIR
export AMX_BUILD_DIR=$(shell pwd)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

export AMX_VERSION=0.0.1
export DOC_OUTPUT_DIR=$(AMX_OUTPUT_DIR)/documentation/amx/

all: amx_log amx_containers amx_parser amx_parser_object glibcmalloc

doc: amx_log_doc amx_containers_doc amx_parser_doc amx_parser_object_doc
	$(INSTALL) -d $(DOC_OUTPUT_DIR)
	$(DOXYGEN) amx.doxy

test: amx_log_test amx_containers_test amx_parser_test amx_parser_object_test
	$(LCOV) $(addprefix -a , $(shell find output/coverage -type f -name "*.info")) --output-file amx.info
	$(GENHTML) -o ./output/coverage/report amx.info
ifdef AMX_TEST_COVERAGE_SHOW
	$(BROWSER) ./output/coverage/report/index.html
endif

clean: amx_log_clean amx_containers_clean amx_parser_clean amx_parser_object_clean glibcmalloc

amx_log:
ifdef CONFIG_AMX_LOG_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_log
	make -C $(AMX_ROOT_DIR)/libraries/amx_log install
endif

amx_log_doc: amx_log
ifdef CONFIG_AMX_LOG_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_log doc
endif

ifdef CONFIG_GLIBC_MALLOC_BUILD
amx_log_test: amx_log glibcmalloc
else
amx_log_test: amx_log
endif
ifdef CONFIG_AMX_LOG_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_log test
endif

amx_log_clean:
	make -C $(AMX_ROOT_DIR)/libraries/amx_log clean

ifdef NO_DEPENDENCIES
amx_containers: 
else
amx_containers: amx_log
endif
ifdef CONFIG_AMX_CONTAINERS_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers install
endif

amx_containers_doc: amx_containers
ifdef CONFIG_AMX_CONTAINERS_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers doc
endif

ifdef CONFIG_GLIBC_MALLOC_BUILD
amx_containers_test: amx_containers glibcmalloc
else
amx_containers_test: amx_containers
endif
ifdef CONFIG_AMX_CONTAINERS_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers test
endif

amx_containers_clean:
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers clean

glibcmalloc:
ifdef CONFIG_GLIBC_MALLOC_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/glibcmalloc
	make -C $(AMX_ROOT_DIR)/libraries/glibcmalloc install
endif

glibcmalloc_clean:
	make -C $(AMX_ROOT_DIR)/libraries/glibcmalloc clean

amx_parser:
ifdef CONFIG_AMX_PARSER_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser install
endif

amx_parser_doc: amx_parser
ifdef CONFIG_AMX_PARSER_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser doc
endif

ifdef CONFIG_GLIBC_MALLOC_BUILD
amx_parser_test: amx_parser glibcmalloc
else
amx_parser_test: amx_parser
endif
ifdef CONFIG_AMX_PARSER_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser test
endif

amx_parser_clean:
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser clean

amx_parser_object:
ifdef CONFIG_AMX_PARSER_OBJECT_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser/amx_parser_object
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser/amx_parser_object install
endif

amx_parser_object_doc: amx_parser_obect
ifdef CONFIG_AMX_PARSER_OBJECT_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser/amx_parser_object doc
endif

ifdef CONFIG_GLIBC_MALLOC_BUILD
amx_parser_object_test: amx_parser_object glibcmalloc
else
amx_parser_object_test: amx_parser_object
endif
ifdef CONFIG_AMX_PARSER_OBJECT_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser_object test
endif

amx_parser_object_clean:
	make -C $(AMX_ROOT_DIR)/libraries/amx_parser/amx_parser_object clean


ifdef NO_DEPENDENCIES
.PHONY: amx_log amx_log_clean amx_containers amx_containers_clean amx_parser amx_parser_clean amx_parser_object_clean
else
.PHONY: amx_log amx_log_clean amx_containers_clean amx_parser amx_parser_clean amx_parser_object_clean
endif
