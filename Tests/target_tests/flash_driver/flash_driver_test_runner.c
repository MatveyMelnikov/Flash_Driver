#include "unity_fixture.h"

TEST_GROUP_RUNNER(flash_driver)
{
  RUN_TEST_CASE(flash_driver, check_link_success);
  RUN_TEST_CASE(flash_driver, random_write_and_read_byte_success);
  RUN_TEST_CASE(flash_driver, write_and_read_page_success);
  RUN_TEST_CASE(flash_driver, write_and_read_multiple_pages_success);
}