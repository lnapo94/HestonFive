
ifdef CONFIG_CONTRIB_HESTONFIVE

# Targets provided by this project
.PHONY: hestonfive clean_hestonfive

# Add this to the "contrib_testing" target
testing: hestonfive
clean_testing: clean_hestonfive

MODULE_CONTRIB_USER_HESTONFIVE=contrib/user/HestonFive

hestonfive: external
	@echo
	@echo "==== Building HestonFive ($(BUILD_TYPE)) ===="
	@echo " Using GCC    : $(CC)"
	@echo " Target flags : $(TARGET_FLAGS)"
	@echo " Sysroot      : $(PLATFORM_SYSROOT)"
	@echo " BOSP Options : $(CMAKE_COMMON_OPTIONS)"
	@[ -d $(MODULE_CONTRIB_USER_HESTONFIVE)/build/$(BUILD_TYPE) ] || \
		mkdir -p $(MODULE_CONTRIB_USER_HESTONFIVE)/build/$(BUILD_TYPE) || \
		exit 1
	@cd $(MODULE_CONTRIB_USER_HESTONFIVE)/build/$(BUILD_TYPE) && \
		CC=$(CC) CFLAGS="$(TARGET_FLAGS)" \
		CXX=$(CXX) CXXFLAGS="$(TARGET_FLAGS)" \
		cmake $(CMAKE_COMMON_OPTIONS) ../.. || \
		exit 1
	@cd $(MODULE_CONTRIB_USER_HESTONFIVE)/build/$(BUILD_TYPE) && \
		make -j$(CPUS) install || \
		exit 1

clean_hestonfive:
	@echo
	@echo "==== Clean-up HestonFive Application ===="
	@[ ! -f $(BUILD_DIR)/usr/bin/hestonfive ] || \
		rm -f $(BUILD_DIR)/etc/bbque/recipes/HestonFive*; \
		rm -f $(BUILD_DIR)/usr/bin/hestonfive*
	@rm -rf $(MODULE_CONTRIB_USER_HESTONFIVE)/build
	@echo

else # CONFIG_CONTRIB_HESTONFIVE

hestonfive:
	$(warning contib module HestonFive disabled by BOSP configuration)
	$(error BOSP compilation failed)

endif # CONFIG_CONTRIB_HESTONFIVE

