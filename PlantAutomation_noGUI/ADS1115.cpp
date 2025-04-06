/**
 * @file ADS1115.cpp
 *
 * @brief Implementation file for the ADS1115 class, representing an Analog-to-Digital Converter (ADC) device.
 */

#include "ADS1115.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/**
 * @brief Constructor for the ADS1115 object.
 * @param address The I2C address of the device.
 * @param muxSelect The analog input multiplexer configuration.
 */
ADS1115::ADS1115(uint8_t address, Mux muxSelect) : m_address(address) {
    // Set default values
    m_buf[0] = 0;
    m_buf[1] = 0;
    m_buf[2] = 0;

    // Set mux to user-specified value
    mux = muxSelect;

    // Set pga to 4.096V
    pga = ADS1115::Pga::FS_4_096V;

    // Set mode to single shot
    mode = ADS1115::Mode::SINGLE_SHOT;

    // Set data rate to 128 SPS
    dataRate = ADS1115::DataRate::SPS_128;

    // Open the I2C device
    if ((m_fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        std::cerr << "Error: Couldn't open device! " << m_fd << std::endl;
        exit(-1);
    }

    if (ioctl(m_fd, I2C_SLAVE, m_address) < 0) {
        std::cerr << "Error: Couldn't find device on address!" << std::endl;
        exit(-1);
    }

    // Print to console the device connected successfully
    std::cout << "Device found at address 0x" << std::hex << static_cast<int>(m_address) << std::endl;
}

/**
 * @brief Destructor for the ADS1115 object.
 */
ADS1115::~ADS1115() {
    close(m_fd);
}

/**
 * @brief Read the analog value from the ADC.
 * @param mux The analog input multiplexer configuration.
 * @param pga The programmable gain amplifier configuration.
 * @param mode The operation mode.
 * @param dataRate The data rate.
 * @return The 16-bit signed integer representing the analog value.
 */
int16_t ADS1115::read(Mux mux, Pga pga, Mode mode, DataRate dataRate) {
    uint16_t config = 0x8000; // Bit 15 needs to be set to start a conversion
    config |= static_cast<uint16_t>(mux);
    config |= static_cast<uint16_t>(pga);
    config |= static_cast<uint16_t>(mode);
    config |= static_cast<uint16_t>(dataRate);

    m_buf[0] = 1; // Configuration register is 1
    m_buf[1] = config >> 8;
    m_buf[2] = config & 0xFF;

    // Write the configuration
    if (::write(m_fd, m_buf, 3) != 3) {
        std::cerr << "Write error" << std::endl;
        exit(-1);
    }

    // Wait for the conversion to complete
    do {
        if (::read(m_fd, m_buf, 2) != 2) {
            std::cerr << "Read conversion" << std::endl;
            exit(-1);
        }
    } while ((m_buf[0] & 0x80) == 0); // Wait until the MSB (bit 7) becomes 1

    // Read the conversion register
    m_buf[0] = 0; // Conversion register address is 0
    if (::write(m_fd, m_buf, 1) != 1) {
        std::cerr << "Write register select" << std::endl;
        exit(-1);
    }
    if (::read(m_fd, m_buf, 2) != 2) {
        std::cerr << "Read conversion" << std::endl;
        exit(-1);
    }

    // Convert the result
    return (m_buf[0] << 8) | m_buf[1];
}

/**
 * @brief Read the analog value from AIN0 (single-ended, single-shot mode).
 * @return The 16-bit signed integer representing the analog value.
 */
int16_t ADS1115::read0() {
    return read(ADS1115::Mux::AIN0_GND, ADS1115::Pga::FS_4_096V, ADS1115::Mode::SINGLE_SHOT, ADS1115::DataRate::SPS_128);
}

/**
 * @brief Read the analog value from AIN1 (single-ended, single-shot mode).
 * @return The 16-bit signed integer representing the analog value.
 */
int16_t ADS1115::read1() {
    return read(ADS1115::Mux::AIN1_GND, ADS1115::Pga::FS_4_096V, ADS1115::Mode::SINGLE_SHOT, ADS1115::DataRate::SPS_128);
}

/**
 * @brief Read the analog value from AIN2 (single-ended, single-shot mode).
 * @return The 16-bit signed integer representing the analog value.
 */
int16_t ADS1115::read2() {
    return read(ADS1115::Mux::AIN2_GND, ADS1115::Pga::FS_4_096V, ADS1115::Mode::SINGLE_SHOT, ADS1115::DataRate::SPS_128);
}

/**
 * @brief Read the analog value from AIN3 (single-ended, single-shot mode).
 * @return The 16-bit signed integer representing the analog value.
 */
int16_t ADS1115::read3() {
    return read(ADS1115::Mux::AIN3_GND, ADS1115::Pga::FS_4_096V, ADS1115::Mode::SINGLE_SHOT, ADS1115::DataRate::SPS_128);
}
