# Flash driver
### Launch
* ```make``` - building a production version of the code (does not contain tests);
* ```make -f MakefileTest.mk``` - building a test version. Depending on the [FOR_TARGET](https://github.com/MatveyMelnikov/Flash_Driver/blob/master/MakefileTest.mk) parameter,
either file [MakefileTestTarget.mk](https://github.com/MatveyMelnikov/Flash_Driver/blob/master/MakefileTestTarget.mk) (for the microcontroller) or file [MakefileTestHost.mk](https://github.com/MatveyMelnikov/Flash_Driver/blob/master/MakefileTestHost.mk) (for launching on the development system) is launched;

### Structure
The driver itself is located [here](https://github.com/MatveyMelnikov/Flash_Driver/tree/master/External/flash_driver).
Tests using the [Unity library](https://github.com/MatveyMelnikov/Flash_Driver/tree/master/External/Unity-2.5.2) are implemented [here](https://github.com/MatveyMelnikov/Flash_Driver/tree/master/Tests). The tests are divided into tests for the host (development system) and target (stm32).
The main test module is [mock_flash_driver_io](https://github.com/MatveyMelnikov/Flash_Driver/blob/master/Tests/mocks/Inc/mock_flash_driver_io.h). It replaces the [IO module](https://github.com/MatveyMelnikov/Flash_Driver/blob/master/External/flash_driver/Inc/flash_driver_io.h), 
which is an interface to the I2C hardware interface. The mock allows you to monitor the calls that the driver generates.
![scheme_flash](https://github.com/MatveyMelnikov/Flash_Driver/assets/55649891/95848981-9e00-423b-be14-d503c29e4a11)
