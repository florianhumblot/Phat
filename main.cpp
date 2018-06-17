#include "hwlib.hpp"
#include "SDDevice.hpp"

int main() {
    // kill the watchdog (ATSAM3X8E specific)
    WDT->WDT_MR = WDT_MR_WDDIS;

    // wait for the COM port to be setup for IO
    hwlib::wait_ms(1000);
//    char test;
//    hwlib::cin >> test;
    auto SS = hwlib::target::pin_out(3, 7);
    auto SCLK = hwlib::target::pin_out(2, 21);
    auto MOSI = hwlib::target::pin_out(2, 22);
    auto MISO = hwlib::target::pin_in(2, 25);
    SS.set(true);
    SCLK.set(true);
    MOSI.set(true);
    hwlib::wait_ms(500);
    SDDevice SDCard(MOSI, SS, SCLK, MISO);

    SDCard.init_card();

    return 0;
}