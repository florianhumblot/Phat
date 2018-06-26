///file SDDevice.hpp
/// \brief Interface to an SD Card.
/// \details
/// This class serves as an inteface to an SD card.
/// It should work for all High Capacity SD cards (SDHC / SDXC) and micro SD cards that are formatted using FAT32.
/// Due to limitations to software SPI, the transfer rate is about 100kHz on the SPI bus, this could be improved by using a Hardware SPI implementation instead of a software implementation.
/// As long as the hardware implementation inherits from hwlib::spi_bus_bit_banged_sclk_mosi_miso, they should be interchangeable.


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
	
	/// \brief the output pins for SPI
	hwlib::pin_out & MOSI, &SS, &SCLK;
	
	/// \brief the input pins for SPI
	hwlib::pin_in & MISO;
	
	/// \brief The SPI bus itself
	hwlib::spi_bus_bit_banged_sclk_mosi_miso spi_bus;
	
	/// \brief The master boot record of the SD card
	MasterBootRecord MBR;
	
	/// \brief Bootsector of the Partition we're using
	BootSector BPB;
	
	/// \brief current directory index, this is used to keep track of the length of our listing array
	uint8_t currentDirectoryIndex = 1;
	
	/// \brief lookup table for the CRC7 needed by the execute_command function. 
	/// \detail
	/// A CRC code shouldn't be necessary for all commands, however, not all manufacturers follow the specifications, so we use it for all commands sent.
	uint8_t CRCTable[256];
	
	/// \brief Array to keep track of the directory Listing, contains the information about the directories and files found on the SD card.
	std::array<DirectoryEntry, 100> directoryListing;
	
	/// \brief enum containing all the commands needed for the SD card to operate in SPI mode. 
	/// This contains only the command number, which will be OR'd by the execute command function with the value 0x40 to add the starting bits.
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

	/// \brief Reads a certain number of bytes. 
	/// After specifying an amount and a buffer, there is an option to keep the MOSI line high (sendOnes), if set to true, the MOSI line will be held high during the transaction.
	/// If set to false, MOSI will not change state and will keep it's previous setting.
	void readBytes( uint8_t amount, bool sendOnes, uint8_t *buffer );

	/// \brief Prints an array of size n in hexadecimal notation. 
	/// By setting command to true, this will also print the command number (small bitshift on the first byte to remove the 0x40 initiating the transaction)
	void print_hex( uint8_t size, uint8_t *data, bool command );
	
	/// \brief Generates the CRC8 lookup table
	/// Generates the 256 value lookup table for CRC7.
	/// This function was written by user hazelnusse on github (link: https://github.com/hazelnusse/crc7)
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
	
	/// \brief Adds part of the message to the CRC code
	/// Returns the CRC value of the previous CRC with the current message byte
	/// This function was written by user hazelnusse on github (link: https://github.com/hazelnusse/crc7)
	uint8_t CRCAdd( uint8_t CRC, uint8_t message_byte ) {
		return CRCTable[( CRC << 1 ) ^ message_byte];
	}
	
	/// \brief Gets the CRC7 token for a message of "length" bytes
	/// This function will generate the CRC7 token needed for a message of n bytes. 
	/// This function was written by user hazelnusse on github (link: https://github.com/hazelnusse/crc7)
	uint8_t getCRC( uint8_t message[], int length ) {
		int i;
		uint8_t CRC = 0;

		for ( i = 0; i < length; ++i )
			CRC = CRCAdd( CRC, message[i] );

		return CRC;
	}

	/// \brief Prints all bytes in a "n" sized array of uint8_t values as characters
	/// Loops over an array of uint8_t values of size n and prints every value that isn't 0x00 as a character.
	void print_text( uint16_t size, uint8_t *data);

	/// \brief read a block of data from the SD card starting at a given address.
	/// Empties the block by setting all values to 0, and then sends the command needed to start a block read.
	/// This will wait for the response token indicating that the command was accepting, and will then wait for the start transaction token (0xFE)
	/// Once it receives the token, it will fill the block array with the block data.
	void readBlock( std::array<uint8_t, 512> & block, uint32_t address );
	
	/// \brief Print a block in chuncks of 16 bytes.
	/// This function calls the print_text() function with chunks of 16 bytes of data.
	void printBlock( std::array<uint8_t, 512> & block);

	/// \brief Debugs a block of data
	/// This will print the hexadecimal offset of a 16 byte chunk of data from the block, as well as the hexadecimal representation of the data, followed with the Char value of each individual byte.
	void debug_block( uint16_t size, uint8_t * data );

	/// \brief Executes a command on the SD card.
	/// This function handles the sending of a command, it accepts a command listed in the SupportedCommands enum, and a 32 bit argument.
	/// The 32 bit argument is modified to become 4 8 bit values which are then transmitted over SPI.
	/// If the command is an ACMD command, the isAcmd flag must be set to true, and the function will call itself with the prefix command.
	int execute_command( SDDevice::SupportedCommands command, uint32_t arguments, bool isAcmd = 0 );

	/// \brief Debugging function for printing the 32 bit argument of execute command as 4 hexadecimal numbers.
	/// This function is used to read and compare the argument value of execute_command in a hexadecimal notation for easy comparison to the specification.
	void print_shifted_value( uint32_t arguments );

	/// \brief Wait n bytes.
	/// This will set the MOSI line high and send n bytes, this is necessary in multiple places, such as the initialisation cycle, and after each command.
	void wait_bytes( uint_fast8_t wait_amount );
public:

	/// \brief Constructs an SDDevice.
	/// Accepts the pins necessary for the SPI transaction.
	SDDevice( hwlib::pin_out &MOSI, hwlib::pin_out &SS,
			  hwlib::pin_out &SCLK, hwlib::pin_in &MISO );

	/// \brief initializes the card.
	/// \details
	/// Follows the initialisation cycle for setting up an SD card in SPI mode. 
	/// First it waits for 80 clock cycles, and then send a few commands that are described in the Simplified Physical layer specification from the SD Association.
	int init_card();

	/// \brief prints a text file of size n. 
	/// \details
	/// This will read a file starting at a specified address for n bytes.
	void printTextFile( uint32_t address, uint32_t size );

	/// \brief Generates the directory listing by recusively looking through the directories and then looking in each directory it finds.
	/// \details
	/// This populates the directoryListing array with directory entries, this keeps track of their address, size, and miscellaneous information such as timestamps describing created and access times
	void generateDirectoryListing( uint8_t parent = 0 );

	/// \brief Opens and prints a textfile
	/// \details
	/// This will check if the given entry in the directoryListing is a file, and if so will open it and print it's contents.
	/// If the entry at the given index is not a file, it will print an error message.
	void openAndPrintFile( uint8_t filenumber );

	/// \brief Prints the directory listing of a given directory, defaults to the root dir of the card.
	/// \details
	/// Prints all the entries in the directory listing that are part of the given directory index ( ofDirectory, which is the parent )
	/// It will also print the index of the entry to then give the option of opening that file or directory.
	void printDirectoryListing( uint16_t ofDirectory = 0);

	/// \brief checks whether the given index is a directory
	bool filenumberIsADirectory( uint8_t filenumber );

	/// \brief Prints the entire directory listing in the order it was generated.
	/// \details
	/// This will also print the index of each entry.
	void printFullDirectoryListing();
};


#endif //PHAT_SDDEVICE_HPP
