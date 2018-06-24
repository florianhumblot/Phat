//
// Created by djrel on 5/26/2018.
//

#ifndef PHAT_SDDEVICE_HPP
#define PHAT_SDDEVICE_HPP

#include "hwlib.hpp"
#include "MasterBootRecord.hpp"
#include "Partition.hpp"
#include "BootSector.hpp"
#include "DirectoryEntry.hpp"
#include <array>

class SDDevice {
private:
	hwlib::pin_out & MOSI, &SS, &SCLK;
	hwlib::pin_in & MISO;
	hwlib::spi_bus_bit_banged_sclk_mosi_miso spi_bus;
	MasterBootRecord MBR;
	BootSector BPB;
	enum SupportedCommands : uint8_t {
		CMD_GO_IDLE_STATE = 0,
		CMD_SEND_OP_COND = 1,
		CMD_SWITCH_FUNC = 6,
		CMD_SEND_IF_COND = 8,
		CMD_SEND_CSD = 9,
		CMD_SEND_CID = 10,
		CMD_STOP_TRANSMISSION = 12,
		CMD_SEND_STATUS = 13,
		CMD_SET_BLOCKLEN = 16,
		CMD_READ_SINGLE_BLOCK = 17,
		CMD_READ_MULTIPLE_BLOCK = 18,
		CMD_WRITE_BLOCK = 24,
		CMD_WRITE_MULIPLE_BLOCK = 25,
		CMD_PROGRAM_CSD = 27,
		CMD_ERASE_WR_BLK_START_ADDR = 32,
		CMD_ERASE_WR_BLK_END_ADDR = 33,
		CMD_ERASE = 38,
		CMD_APP_CMD = 55,
		CMD_GEN_CMD = 56,
		CMD_READ_OCR = 58,
		CMD_CRC_ON_OFF = 59,
		ACMD_SET_BUS_WIDTH = 6,
		ACMD_SD_STATUS = 13,
		ACMD_SEND_NUM_WR_BOCKS = 22,
		ACMD_SET_WR_BLK_ERASE_COUNT = 23,
		ACMD_SD_SEND_OP_COND = 41,
		ACMD_SET_CLR_CARD_DETECT = 42,
		ACMD_SEND_SCR = 51,
	};
	std::array<uint8_t, 8> data;
	template <unsigned int size>
	bool all_values( std::array<uint8_t, size> data, uint8_t value ) {
		for ( unsigned int i = 0; i < size; i++ ) {
			if ( data[i] != value ) return false;
		}
		return true;
	}
	template <unsigned int size>
	bool not_all_values( std::array<uint8_t, size> data, uint8_t value ) {
		for ( unsigned int i = 0; i < size; i++ ) {
			if ( data[i] == value ) return false;
		}
		return true;
	}
	void readBytes( uint8_t amount, bool sendOnes, uint8_t *buffer );
	void print_binary( uint8_t size, uint8_t *data, bool command );
	uint8_t CRCTable[256];

	void GenerateCRCTable() {
		int i, j;
		uint8_t CRCPoly = 0x89;  // the value of our CRC-7 polynomial

		// generate a table value for all 256 possible byte values
		for ( i = 0; i < 256; ++i ) {
			CRCTable[i] = ( i & 0x80 ) ? i ^ CRCPoly : i;
			for ( j = 1; j < 8; ++j ) {
				CRCTable[i] <<= 1;
				if ( CRCTable[i] & 0x80 )
					CRCTable[i] ^= CRCPoly;
			}
		}
	}
	std::array<DirectoryEntry, 100> directoryListing;
	uint16_t amount_of_directories = 0;
	// adds a message byte to the current CRC-7 to get a the new CRC-7
	uint8_t CRCAdd( uint8_t CRC, uint8_t message_byte ) {
		return CRCTable[( CRC << 1 ) ^ message_byte];
	}

	// returns the CRC-7 for a message of "length" bytes
	uint8_t getCRC( uint8_t message[], int length ) {
		int i;
		uint8_t CRC = 0;

		for ( i = 0; i < length; ++i )
			CRC = CRCAdd( CRC, message[i] );

		return CRC;
	}

	void print_text( uint16_t size, uint8_t *data, uint32_t address );
	void readBlock( std::array<uint8_t, 512> & block, uint32_t address );
	void printBlock( std::array<uint8_t, 512> & block, uint32_t address );
	uint8_t currentDirectoryIndex = 0;
	void debug_block( uint16_t size, uint8_t * data );
	uint32_t processed_addresses[100];
	bool addressHasBeenProcessedAlready( uint32_t address );
public:
	SDDevice( hwlib::pin_out &MOSI, hwlib::pin_out &SS,
			  hwlib::pin_out &SCLK, hwlib::pin_in &MISO );
	int init_card();
	int execute_command( SDDevice::SupportedCommands command, uint32_t arguments, bool isAcmd = 0 );

	void print_shifted_value( uint32_t arguments );

	void wait_bytes( uint_fast8_t wait_amount );

	int getDirectoryListing( uint8_t filenumber = 0);
	void printTextFile( uint32_t address, uint32_t size );
};


#endif //PHAT_SDDEVICE_HPP
