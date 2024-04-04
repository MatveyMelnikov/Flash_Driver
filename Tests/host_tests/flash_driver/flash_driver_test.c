#include "unity_fixture.h"
#include "flash_driver.h"
#include "mock_flash_driver_io.h"
#include <string.h>

// Defines -------------------------------------------------------------------

#define ADD_ADDR_IN_BYTES(arr, addr) \
  arr[0] = (uint8_t)((addr) >> 8); \
  arr[1] = (uint8_t)(addr)

// Static functions ----------------------------------------------------------

static void check_data(
  const uint8_t *const expected,
  const uint8_t *const actual,
  const uint16_t data_size
)
{
  for (uint16_t i = 0; i < data_size; i++)
    TEST_ASSERT_BYTES_EQUAL(expected[i], actual[i]);
}

// Tests ---------------------------------------------------------------------

TEST_GROUP(flash_driver);

TEST_SETUP(flash_driver)
{
  mock_flash_driver_io_create(10);
}

TEST_TEAR_DOWN(flash_driver)
{
  mock_flash_driver_io_verify_complete();
  mock_flash_driver_io_destroy();
}

TEST(flash_driver, check_link_success)
{
  uint8_t check_busy_output_data[] = { 0x05 };
  uint8_t check_busy_input_data[] = { 0x00 };
  uint8_t check_link_output_data[] = { 0x90, 0x0, 0x0, 0x0, 0x0, 0x0 };
  uint8_t check_link_input_data[] = { 0x00, 0x0, 0x0, 0x0, 0xef, 0x17 };

  mock_flash_driver_io_expect_select(true);
  mock_flash_driver_io_expect_write_read(
    check_busy_output_data,
    check_busy_input_data,
    sizeof(check_busy_output_data),
    sizeof(check_busy_input_data)
  );
  mock_flash_driver_io_expect_select(false);

  mock_flash_driver_io_expect_select(true);
  mock_flash_driver_io_expect_write_read(
    check_link_output_data,
    check_link_input_data,
    sizeof(check_link_output_data),
    sizeof(check_link_input_data)
  );
  mock_flash_driver_io_expect_select(false);

  flash_driver_status status = flash_driver_check_link();
  TEST_ASSERT_EQUAL(FLASH_DRIVER_OK, status);
}

TEST(flash_driver, check_link_error)
{
  uint8_t check_busy_output_data[] = { 0x05 };
  uint8_t check_busy_input_data[] = { 0x00 };
  uint8_t check_link_output_data[] = { 0x90, 0x0, 0x0, 0x0, 0x0, 0x0 };
  uint8_t check_link_input_data[] = { 0x00, 0x0, 0x0, 0x0, 0x0, 0x0 };

  mock_flash_driver_io_expect_select(true);
  mock_flash_driver_io_expect_write_read(
    check_busy_output_data,
    check_busy_input_data,
    sizeof(check_busy_output_data),
    sizeof(check_busy_input_data)
  );
  mock_flash_driver_io_expect_select(false);

  mock_flash_driver_io_expect_select(true);
  mock_flash_driver_io_expect_write_read(
    check_link_output_data,
    check_link_input_data,
    sizeof(check_link_output_data),
    sizeof(check_link_input_data)
  );
  mock_flash_driver_io_expect_select(false);

  flash_driver_status status = flash_driver_check_link();
  TEST_ASSERT_EQUAL(FLASH_DRIVER_ERROR, status);
}

TEST(flash_driver, check_link_busy_error)
{
  uint8_t check_busy_output_data[] = { 0x05 };
  uint8_t check_busy_input_data[] = { 0x01 };

  mock_flash_driver_io_expect_select(true);
  mock_flash_driver_io_expect_write_read(
    check_busy_output_data,
    check_busy_input_data,
    sizeof(check_busy_output_data),
    sizeof(check_busy_input_data)
  );
  mock_flash_driver_io_expect_select(false);

  flash_driver_status status = flash_driver_check_link();
  TEST_ASSERT_EQUAL(FLASH_DRIVER_BUSY, status);
}
