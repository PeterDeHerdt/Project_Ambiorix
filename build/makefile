ifndef AMX_ROOT_DIR
$(error AMX_ROOT_DIR not defined)
endif

ifndef AMX_BUILD_DIR
export AMX_BUILD_DIR=$(shell pwd)
endif

include $(AMX_BUILD_DIR)/amx_build.config
include $(AMX_BUILD_DIR)/amx.config

all: amx_log amx_containers

doc: amx_log_doc amx_containers_doc

test: amx_log_test amx_containers_test
	$(LCOV) $(addprefix -a , $(shell find output/coverage -type f -name "*.info")) --output-file amx.info
	$(GENHTML) -o ./output/coverage/report amx.info
ifdef AMX_TEST_COVERAGE_SHOW
	$(BROWSER) ./output/coverage/report/index.html
endif

clean: amx_log_clean amx_containers_clean

amx_log:
ifdef CONFIG_AMX_LOG_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_log
	make -C $(AMX_ROOT_DIR)/libraries/amx_log install
endif

amx_log_doc:
ifdef CONFIG_AMX_LOG_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_log doc
endif

amx_log_test: amx_log
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

amx_containers_doc:
ifdef CONFIG_AMX_CONTAINERS_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers doc
endif

amx_containers_test: amx_containers
ifdef CONFIG_AMX_CONTAINERS_BUILD
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers test
endif

amx_containers_clean:
	make -C $(AMX_ROOT_DIR)/libraries/amx_containers clean
				  	
ifdef NO_DEPENDENCIES
.PHONY: amx_log amx_log_doc amx_log_clean amx_containers amx_containers_doc amx_containers_clean
else
.PHONY: amx_log amx_log_doc amx_log_clean amx_containers_doc amx_containers_clean
endif