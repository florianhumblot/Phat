#include "hwlib.hpp"
#include "phat/SDDevice.hpp"

int main() {
	// kill the watchdog (ATSAM3X8E specific)
	WDT->WDT_MR = WDT_MR_WDDIS;

	// wait for the COM port to be setup for IO
	hwlib::wait_ms( 1000 );
	//    char test;
	//    hwlib::cin >> test;
	auto SS = hwlib::target::pin_out( 3, 7 );
	auto SCLK = hwlib::target::pin_out( 2, 21 );
	auto MOSI = hwlib::target::pin_out( 2, 22 );
	auto MISO = hwlib::target::pin_in( 2, 25 );
	SS.set( true );
	SCLK.set( false );
	MOSI.set( true );
	hwlib::wait_ms( 500 );
	SDDevice SDCard( MOSI, SS, SCLK, MISO );

	int status = SDCard.init_card();
	if ( status == -1 ) {
		hwlib::cout << "Initialization failed, try again" << hwlib::endl;
		return 0;
	}
	status = SDCard.generateDirectoryListing();
	if ( status == -1 ) {
		hwlib::cout << "The generation of the directory listing failed, try again" << hwlib::endl;
	}
	SDCard.printDirectoryListing();
	for ( ;; ) {
		hwlib::cout << "\n\nWhat folder or file would you like to open? (Select by typing the number to the left of the entry or the '/' character for returning to the root)" << hwlib::endl;
		hwlib::cout << "CAUTION: by selecting a file, the file will be opened and printed.\n" << hwlib::endl;
		char temp;
		uint16_t filenumber = 0;
		do {
			hwlib::cin >> temp;
			if ( ( temp >= 48 && temp <= 57 ) ) {
				hwlib::cout << "temp: " << temp;
				filenumber = (filenumber * 10) + (temp - '0');
				hwlib::cout << " || temp after: " << filenumber << hwlib::endl;
			} else if ( temp == '/' ) {
				break;
			}
		} while ( temp != 13 );
		if ( temp != '/' ) {
			if ( SDCard.filenumberIsADirectory( filenumber ) ) {
				SDCard.printDirectoryListing( filenumber );
			} else {
				status = SDCard.openAndPrintFile( filenumber );
				if ( status == -1 ) {
					hwlib::cout << "Reading the file failed, try again" << hwlib::endl;
					return 0;
				}
			}
		} else {
			SDCard.printDirectoryListing( 0 );
		}


	}
	return 0;
}