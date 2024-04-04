# File for test cases
# Here you can add platform-specific files (main file, test, etc.)

CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = External/Unity-2.5.2
FLASH_DRIVER = External/flash_driver
FOR_TARGET = N

C_INCLUDES += \
-I$(FLASH_DRIVER)/Inc \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR)

C_SOURCES += \
$(FLASH_DRIVER)/Src/flash_driver.c \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/target_tests.c \
$(TESTS_DIR)/target_tests/flash_driver/flash_driver_test_runner.c \
$(TESTS_DIR)/target_tests/flash_driver/flash_driver_test.c \
$(FLASH_DRIVER)/Src/flash_driver_io.c

include MakefileTestTarget.mk
else
C_INCLUDES += \
-I$(TESTS_DIR)/mocks/Inc
C_SOURCES += \
$(TESTS_DIR)/host_tests.c \
$(TESTS_DIR)/host_tests/flash_driver/flash_driver_test_runner.c \
$(TESTS_DIR)/host_tests/flash_driver/flash_driver_test.c \
$(TESTS_DIR)/mocks/Src/mock_flash_driver_io.c

include MakefileTestHost.mk
endif
