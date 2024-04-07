#include "unity_fixture.h"
#include "flash_driver.h"
#include <string.h>

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
}

TEST_TEAR_DOWN(flash_driver)
{
}

TEST(flash_driver, check_link_success)
{
  flash_driver_status status = flash_driver_check_link();

  TEST_ASSERT_EQUAL(FLASH_DRIVER_OK, status);
}

TEST(flash_driver, random_write_and_read_byte_success)
{
  uint8_t output_data = 0x55;
  uint8_t input_data = 0;

  // first element of last sector (4096)
  flash_driver_status status = flash_driver_sector_erase(0xfff000);

  while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
  status = flash_driver_write(0xfff000, &output_data, 1);

  while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
  status |= flash_driver_read(0xfff000, &input_data, 1);

  TEST_ASSERT_EQUAL(FLASH_DRIVER_OK, status);
  TEST_ASSERT_EQUAL(0x55, input_data);
}

TEST(flash_driver, write_and_read_page_success)
{
  uint8_t output_data[256] = { 0x55, 0xaa };
  memcpy(output_data + 125, output_data, 2);
  memcpy(output_data + 254, output_data, 2);
  uint8_t input_data[256];

  // second page of last sector (256, 4096)
  flash_driver_status status = FLASH_DRIVER_OK;
  while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
  status = flash_driver_write(
    0xfff100,
    output_data,
    sizeof(output_data)
  );
  while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
  status |= flash_driver_read(0xfff100, &input_data, sizeof(input_data));

  TEST_ASSERT_EQUAL(FLASH_DRIVER_OK, status);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(output_data, input_data, sizeof(output_data));
}

TEST(flash_driver, write_and_read_multiple_pages_success)
{
  uint8_t output_data[3584] = { 0x55, 0xaa }; // 14 pages
  memset(output_data + 2, 0, sizeof(output_data) - 2);
  uint8_t input_data[3584];
  memset(input_data, 0, sizeof(input_data));

  for (uint16_t i = 125; i < 3584; i++)
    memcpy(output_data + i, output_data, 2);

  // third page of last sector (512, 4096)
  flash_driver_status status = FLASH_DRIVER_OK;
  for (uint16_t i = 0; i < 3584; i += 256)
  {
    while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
    status = flash_driver_write(
      0xfff200 + i,
      output_data + i,
      256
    );
  }
  while (flash_driver_is_busy() == FLASH_DRIVER_BUSY) {}
  status |= flash_driver_read(0xfff200, &input_data, sizeof(input_data));

  TEST_ASSERT_EQUAL(FLASH_DRIVER_OK, status);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(output_data, input_data, sizeof(output_data));
}
