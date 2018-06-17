//
// Created by djrel on 5/26/2018.
//

#include "hwlib.hpp"
#include "SDDevice.hpp"
#include <array>

int SDDevice::init_card() {
    std::array<uint8_t, 32> buffer = {};
    GenerateCRCTable();
    SS.set(true);
    wait_bytes(10);
    SS.set(false);
    hwlib::wait_ms(20);
    SS.set(false);
	hwlib::wait_ms(1);
    execute_command(CMD_GO_IDLE_STATE, 0x00000000, false);
    readBytes(8, true, buffer.begin());
    print_binary(8, buffer.begin(), false);
    hwlib::wait_ms(5);
    SS.set(true);
	hwlib::wait_ms(1);
    wait_bytes(1);
    SS.set(false);
	hwlib::wait_ms(1);
    execute_command(CMD_SEND_IF_COND, 0x000001AA, false);
    readBytes(8, true, buffer.begin());
    print_binary(8, buffer.begin(), false);
    SS.set(true);
	hwlib::wait_ms(1);
    wait_bytes(1);
	do{
		SS.set(false);
		hwlib::wait_ms(1);
		execute_command(CMD_APP_CMD, 0x0, false);
		readBytes(8, true, buffer.begin());
		print_binary(8, buffer.begin(), false);
		execute_command(ACMD_SD_SEND_OP_COND, 0x40000000, false);
		readBytes(8, true, buffer.begin());
		print_binary(8, buffer.begin(), false);
		SS.set(true);
		hwlib::wait_ms(1);
	} while ( buffer[0] != 0x0);
	wait_bytes(1);
	SS.set(false);
	hwlib::wait_ms(1);
	execute_command(CMD_READ_OCR, 0x0000000, false);
	readBytes(8, true, buffer.begin());
	print_binary(8, buffer.begin(), false);
	SS.set(true);
	hwlib::wait_ms(1);

    return 0;
}

void SDDevice::wait_bytes(uint_fast8_t wait_amount = 1){
    readBytes(wait_amount, true, nullptr);
}

int SDDevice::execute_command(SDDevice::SupportedCommands command, uint32_t arguments, bool isAcmd) {
    uint8_t CRC = 0xff;
    print_shifted_value(arguments);
    uint8_t data_out[6] = {
            static_cast<uint8_t >((command | 0x40)),
            static_cast<uint8_t >((arguments >> 24)),
            static_cast<uint8_t >((arguments >> 16)),
            static_cast<uint8_t >((arguments >> 8)),
            static_cast<uint8_t >((arguments)),
            CRC
    };
    data_out[5] = (getCRC(data_out, 5) << 1) | 1;
    if (isAcmd) {
        execute_command(CMD_APP_CMD, 0x0, false);
    }

    print_binary(6, data_out, true);

    spi_bus.write_and_read(hwlib::pin_out_dummy, 6, data_out, data.begin());
    wait_bytes(1);
    return 0;
}


SDDevice::SDDevice(hwlib::pin_out &MOSI, hwlib::pin_out &SS,
                   hwlib::pin_out &SCLK, hwlib::pin_in &MISO)
        : MOSI(MOSI),
          SS(SS),
          SCLK(SCLK),
          MISO(MISO),
          spi_bus(hwlib::spi_bus_bit_banged_sclk_mosi_miso(SCLK, MOSI, MISO)) {}

void SDDevice::readBytes(uint8_t amount, bool sendOnes, uint8_t *buffer) {
    if (sendOnes) {
        hwlib::wait_us(15);
        uint8_t data_out[amount];
        for (auto &c : data_out) { c = 0xFF; }
        spi_bus.write_and_read(hwlib::pin_out_dummy, amount, data_out, buffer);
        return;
    }
    spi_bus.write_and_read(hwlib::pin_out_dummy, amount, nullptr, buffer);
}

void SDDevice::print_binary(uint8_t size, uint8_t *data, bool command = false) {
    if (command) {
        hwlib::cout << hwlib::setw(8) << hwlib::setfill(' ') << "COMMAND(CMD" << hwlib::dec << +(data[0] & ~(1UL << 6)) <<"): ";
    } else {
        hwlib::cout << hwlib::setw(9) << hwlib::setfill(' ') << "DATA: ";
    }


    for (uint8_t i = 0; i < size; i++) {
        hwlib::cout << hwlib::setw(2) << hwlib::setfill('0') << hwlib::hex << "0x" << +data[i] << " ";
    }
    hwlib::cout << hwlib::endl << hwlib::endl;
}

void SDDevice::print_shifted_value(uint32_t arguments){

    hwlib::cout << hwlib::bin << "The shifted values of: " << +arguments << " are: " << hwlib::endl;

    hwlib::cout << hwlib::setw(2) << hwlib::setfill('0') << hwlib::hex << +static_cast<uint8_t >((arguments >> 24)) << " ";
    hwlib::cout << hwlib::setw(2) << hwlib::setfill('0') << hwlib::hex << +static_cast<uint8_t >((arguments >> 16)) << " ";
    hwlib::cout << hwlib::setw(2) << hwlib::setfill('0') << hwlib::hex << +static_cast<uint8_t >((arguments >> 8)) << " ";
    hwlib::cout << hwlib::setw(2) << hwlib::setfill('0') << hwlib::hex << +static_cast<uint8_t >(arguments) << hwlib::endl << hwlib::endl;
}