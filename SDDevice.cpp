//
// Created by djrel on 5/26/2018.
//

#include "hwlib.hpp"
#include "SDDevice.hpp"
#include "MasterBootRecord.hpp"
#include "Partition.hpp"
#include "BootSector.hpp"
#include <array>

int SDDevice::init_card() {
	std::array<uint8_t, 32> buffer = {};
	GenerateCRCTable();
	SS.set( true );
	wait_bytes( 10);
	SS.set( false );
	hwlib::wait_ms( 20 );
	SS.set( false );
	hwlib::wait_ms( 1 );
	execute_command( CMD_GO_IDLE_STATE, 0x00000000, false );
	readBytes( 8, true, buffer.begin() );
	print_binary( 8, buffer.begin(), false );
	SS.set( true );
	wait_bytes( 1 );
	SS.set( false );
	execute_command( CMD_SEND_IF_COND, 0x000001AA, false );
	readBytes( 8, true, buffer.begin() );
	print_binary( 8, buffer.begin(), false );
	SS.set( true );
	wait_bytes( 1 );
	do {
		SS.set( false);
		execute_command( CMD_APP_CMD, 0x0, false );
		readBytes( 8, true, buffer.begin() );
		execute_command( ACMD_SD_SEND_OP_COND, 0x40000000, false );
		readBytes( 8, true, buffer.begin() );
		print_binary( 8, buffer.begin(), false );
		SS.set( true );
	} while ( buffer[0] != 0x0 );
	SS.set( true );
	execute_command( CMD_READ_OCR, 0x0000000, false );
	readBytes( 8, true, buffer.begin() );
	print_binary( 8, buffer.begin(), false );
	SS.set( false );
	execute_command( CMD_SET_BLOCKLEN, 0x0000200, false );
	readBytes( 8, true, buffer.begin() );
	print_binary( 8, buffer.begin(), false );
	SS.set( true );
	std::array<uint8_t, 512> block = {};
	uint32_t address = 0x00000000;
	SS.set( false );
	readBlock( block, address );
	hwlib::cout << "MBR: " << hwlib::endl;
	printBlock( block, address );
	SS.set( true );
	uint8_t data_[16];
	for ( auto & i : data_ ) { i = 0; }
	Partition Partitions[4];
	uint8_t j = 0;
	for ( uint_fast8_t i = 446; i < 462; i++ ) {
		data_[j] = block[i];
		j++;
	}
	Partitions[0] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 462; i < 478; i++ ) {
		data_[j] = block[i];
	}
	Partitions[1] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 478; i < 494; i++ ) {
		data_[j] = block[i];
	}
	Partitions[2] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 494; i < 510; i++ ) {
		data_[j] = block[i];
	}
	Partitions[3] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }

	hwlib::wait_ms( 1 );

	MBR = MasterBootRecord( Partitions[0], Partitions[1], Partitions[2], Partitions[3] );

	hwlib::cout << "Is this a usable drive: " << MBR.isUsableDrive() << hwlib::endl;
	hwlib::cout << "The first address of partition 0: " << MBR.getFatAddress( 0 ) << hwlib::endl;
	hwlib::cout << "The first address of partition 1: " << MBR.getFatAddress( 1 ) << hwlib::endl;
	hwlib::cout << "The first address of partition 2: " << MBR.getFatAddress( 2 ) << hwlib::endl;
	hwlib::cout << "The first address of partition 3: " << MBR.getFatAddress( 3 ) << hwlib::endl;
	address = MBR.getFatAddress( 0 );
	SS.set( false );
	
	readBlock( block, address );
	hwlib::cout << "BootSector: " << hwlib::endl;
	printBlock( block, address);
	SS.set( true );

	uint8_t BPBinfo[16];
	BPBinfo[0] = block[12];
	BPBinfo[1] = block[11];
	BPBinfo[2] = block[13];
	BPBinfo[3] = block[15];
	BPBinfo[4] = block[14];
	BPBinfo[5] = block[16];
	BPBinfo[6] = block[39];
	BPBinfo[7] = block[38];
	BPBinfo[8] = block[37];
	BPBinfo[9] = block[36];
	BPBinfo[10] = block[47];
	BPBinfo[11] = block[46];
	BPBinfo[12] = block[45];
	BPBinfo[13] = block[44];
	BPBinfo[14] = block[18];
	BPBinfo[15] = block[17];

	BPB = BootSector( BPBinfo , address);
	hwlib::cout << "BootSector:\n " << BPB << hwlib::endl;
	address = BPB.GetRootDirAddress();

	SS.set( false );
	hwlib::cout << "Root Dir: " << hwlib::endl;
	readBlock( block, address );
	printBlock( block, address );
	SS.set( true );

	return 0;
}

void SDDevice::wait_bytes( uint_fast8_t wait_amount = 1) {
	readBytes( wait_amount, true, nullptr );
}

int SDDevice::execute_command( SDDevice::SupportedCommands command, uint32_t arguments, bool isAcmd ) {
	
	uint8_t CRC = 0xff;
	uint8_t data_out[6] = {
			static_cast< uint8_t >( ( command | 0x40 ) ),
			static_cast< uint8_t >( ( arguments >> 24 ) ),
			static_cast< uint8_t >( ( arguments >> 16 ) ),
			static_cast< uint8_t >( ( arguments >> 8 ) ),
			static_cast< uint8_t >( ( arguments ) ),
			CRC
	};
	data_out[5] = ( getCRC( data_out, 5 ) << 1 ) | 1;
	if ( isAcmd ) {
		execute_command( CMD_APP_CMD, 0x0, false );
	}
	//if ( command != CMD_READ_SINGLE_BLOCK ) {
		print_binary( 6, data_out, true );
		print_shifted_value( arguments );
	//}

	spi_bus.write_and_read( hwlib::pin_out_dummy, 6, data_out, data.begin() );
	wait_bytes( 1 );
	return 0;
}
void SDDevice::readBlock( std::array<uint8_t, 512>& block, uint32_t address ) {
	//reset block
	for ( uint16_t i = 0; i < 512; i++ ) {
		block[i] = 0x00;
	}
	wait_bytes( 8 );
	execute_command( CMD_READ_SINGLE_BLOCK, address, false );
	do {
		// Wait for the command to be accepted
		readBytes( 1, true, block.begin() );
		print_binary( 1, block.begin(), false );
	} while ( block[0] != 0x00 );

	do {
		// Wait for the transfer to begin, which is when we receive 0xFE
		readBytes( 1, true, block.begin() );
		print_binary( 1, block.begin(), false );
	} while ( block[0] != 0xFE );

	for ( uint_fast8_t i = 0; i <= 496; i += 16 ) {
		readBytes( 16, true, block.begin() + ( address % 512 ));
		address += 0x10;
	}
	// Read CRC bits
	uint8_t CRC_BITS[2] = { 0x00 };
	readBytes( 2, true, CRC_BITS );
	// And finish off by waiting 10 bytes.
	wait_bytes( 10 );
}

void SDDevice::printBlock( std::array<uint8_t, 512>& block, uint32_t start_address ) {
	for ( uint_fast8_t i = 0; i <= 496; i += 16 ) {
		//hwlib::cout << " I == " << i << ": ";
		print_text(16, block.begin() + ( start_address % 512), start_address);
		start_address += 0x10;
	}
}

SDDevice::SDDevice( hwlib::pin_out &MOSI, hwlib::pin_out &SS,
					hwlib::pin_out &SCLK, hwlib::pin_in &MISO )
	: MOSI( MOSI ),
	SS( SS ),
	SCLK( SCLK ),
	MISO( MISO ),
	spi_bus( hwlib::spi_bus_bit_banged_sclk_mosi_miso( SCLK, MOSI, MISO ) ) {}

void SDDevice::readBytes( uint8_t amount, bool sendOnes, uint8_t *buffer ) {
	if ( sendOnes ) {
		hwlib::wait_us( 15 );
		uint8_t data_out[amount];
		for ( auto &c : data_out ) { c = 0xFF; }
		spi_bus.write_and_read( hwlib::pin_out_dummy, amount, data_out, buffer );
		return;
	}
	spi_bus.write_and_read( hwlib::pin_out_dummy, amount, nullptr, buffer );
}

void SDDevice::print_binary( uint8_t size, uint8_t *data, bool command = false ) {
	if ( command ) {
		hwlib::cout << hwlib::setw( 8 ) << hwlib::setfill( ' ' ) << "COMMAND(CMD" << hwlib::dec << +( data[0] & ~( 1UL << 6 ) ) << "): ";
	} else {
		hwlib::cout << hwlib::setw( 9 ) << hwlib::setfill( ' ' ) << "DATA: ";
	}


	for ( uint8_t i = 0; i < size; i++ ) {
		hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << "0x" << +data[i] << " ";
	}
	hwlib::cout << hwlib::endl << hwlib::endl;
}

void SDDevice::print_shifted_value( uint32_t arguments ) {

	hwlib::cout << hwlib::bin << "The shifted values of: " << +arguments << " are: " << hwlib::endl;

	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 24 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 16 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 8 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( arguments ) << hwlib::endl << hwlib::endl;
}

void SDDevice::print_text( uint16_t size, uint8_t * data, uint32_t address ) {
	hwlib::cout << hwlib::setfill( '0' ) << hwlib::setw( 8 ) << +address;
	hwlib::cout << " - ";
	for ( uint8_t j = 0; j < size; j++ ) {
		hwlib::cout << "" << hwlib::hex << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << +data[j] << " ";
	}
	hwlib::cout << " - ";
	for ( uint8_t j = 0; j < size; j++ ) {
		if ( data[j] == 0x00 ) {
			hwlib::cout << ".";
		} else {
			hwlib::cout << ( char ) data[j];
		}
	}
	hwlib::cout << hwlib::endl;

}