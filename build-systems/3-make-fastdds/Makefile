SRC_DIR = /opt/fastdds
INSTALL_DIR = $(SRC_DIR)/install
FOONATHAN_MEMORY_VENDOR_PATH = $(SRC_DIR)/foonathan_memory_vendor

foonathan_memory:
	echo "Cloning foonathan_memory_vendor"
	git clone https://github.com/eProsima/foonathan_memory_vendor.git $(FOONATHAN_MEMORY_VENDOR_PATH)
	@mkdir $(FOONATHAN_MEMORY_VENDOR_PATH)/build
	@cd $(FOONATHAN_MEMORY_VENDOR_PATH)/build && \
	cmake .. -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) -DBUILD_SHARED_LIBS=ON && \
	cmake --build . --target install
