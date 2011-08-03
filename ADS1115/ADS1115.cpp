// I2Cdev library collection - ADS1115 I2C device class
// Based on Texas Instruments ADS1113/4/5 datasheet, May 2009 (SBAS444B, revised October 2009)
// Note that the ADS1115 uses 16-bit registers, not 8-bit registers.
// 8/2/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "ADS1115.h"

/** Default constructor, uses default I2C address.
 * @see ADS1115_DEFAULT_ADDRESS
 */
ADS1115::ADS1115() {
    devAddr = ADS1115_DEFAULT_ADDRESS;
    devMode = ADS1115_MODE_SINGLESHOT;
    muxMode = ADS1115_MUX_P0_N1;
}

/** Specific address constructor.
 * @param address I2C address
 * @see ADS1115_DEFAULT_ADDRESS
 * @see ADS1115_ADDRESS_ADDR_GND
 * @see ADS1115_ADDRESS_ADDR_VDD
 * @see ADS1115_ADDRESS_ADDR_SDA
 * @see ADS1115_ADDRESS_ADDR_SDL
 */
ADS1115::ADS1115(uint8_t address) {
    devAddr = address;
    devMode = ADS1115_MODE_SINGLESHOT;
    muxMode = ADS1115_MUX_P0_N1;
}

/** Power on and prepare for general usage.
 * This device is ready to use automatically upon power-up. It defaults to
 * single-shot read mode, P0/N1 mux, 2.048v gain, 128 samples/sec, default
 * comparator with hysterysis, active-low polarity, non-latching comparator,
 * and comparater-disabled operation. This stub function is present for device
 * library consistency.
 */
void ADS1115::initialize() {
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool ADS1115::testConnection() {
    return I2Cdev::readWord(devAddr, ADS1115_RA_CONVERSION, buffer) == 1;
}

// CONVERSION register

/** Read differential value based on current MUX configuration.
 * The default MUX setting sets the device to get the differential between the
 * AIN0 and AIN1 pins. There are 8 possible MUX settings, but if you are using
 * all four input pins as single-end voltage sensors, then the default option is
 * not what you want; instead you will need to set the MUX to compare the
 * desired AIN* pin with GND. There are four shortcut methods (getDiff*) to do
 * this conveniently, but you can also do it manually with setMultiplexer()
 * followed by this method.
 *
 * In single-shot mode, this register may not have fresh data. You need to write
 * a 1 bit to the MSB of the CONFIG register to trigger a single read/conversion
 * before this will be populated with fresh data. This technique is not as
 * effortless, but it has enormous potential to save power by only running the
 * comparison circuitry when needed.
 *
 * @return 16-bit signed differential value
 * @see getDiff0();
 * @see getDiff1();
 * @see getDiff2();
 * @see getDiff3();
 * @see setMultiplexer();
 * @see ADS1115_RA_CONVERSION
 * @see ADS1115_MUX_P0_N1
 * @see ADS1115_MUX_P0_N3
 * @see ADS1115_MUX_P1_N3
 * @see ADS1115_MUX_P2_N3
 * @see ADS1115_MUX_P0_NG
 * @see ADS1115_MUX_P1_NG
 * @see ADS1115_MUX_P2_NG
 * @see ADS1115_MUX_P3_NG
 */
int16_t ADS1115::getDifferential() {
    I2Cdev::readWord(devAddr, ADS1115_RA_CONVERSION, buffer);
    return buffer[0];
}
/** Get AIN0/GND differential.
 * This changes the MUX setting to AIN0/GND if necessary, triggers a new
 * measurement (also only if necessary), then gets the differential value
 * currently in the CONVERSION register.
 * @return 16-bit signed differential value
 * @see getDifferential()
 */
int16_t ADS1115::getDiff0() {
    if (muxMode != ADS1115_MUX_P0_NG) setMultiplexer(ADS1115_MUX_P0_NG);
    if (devMode == ADS1115_MODE_SINGLESHOT) setOpStatus(ADS1115_OS_ACTIVE);
    return getDifferential();
}
/** Get AIN1/GND differential.
 * This changes the MUX setting to AIN1/GND if necessary, triggers a new
 * measurement (also only if necessary), then gets the differential value
 * currently in the CONVERSION register.
 * @return 16-bit signed differential value
 * @see getDifferential()
 */
int16_t ADS1115::getDiff1() {
    if (muxMode != ADS1115_MUX_P1_NG) setMultiplexer(ADS1115_MUX_P1_NG);
    if (devMode == ADS1115_MODE_SINGLESHOT) setOpStatus(ADS1115_OS_ACTIVE);
    return getDifferential();
}
/** Get AIN2/GND differential.
 * This changes the MUX setting to AIN2/GND if necessary, triggers a new
 * measurement (also only if necessary), then gets the differential value
 * currently in the CONVERSION register.
 * @return 16-bit signed differential value
 * @see getDifferential()
 */
int16_t ADS1115::getDiff2() {
    if (muxMode != ADS1115_MUX_P2_NG) setMultiplexer(ADS1115_MUX_P2_NG);
    if (devMode == ADS1115_MODE_SINGLESHOT) setOpStatus(ADS1115_OS_ACTIVE);
    return getDifferential();
}
/** Get AIN3/GND differential.
 * This changes the MUX setting to AIN3/GND if necessary, triggers a new
 * measurement (also only if necessary), then gets the differential value
 * currently in the CONVERSION register.
 * @return 16-bit signed differential value
 * @see getDifferential()
 */
int16_t ADS1115::getDiff3() {
    if (muxMode != ADS1115_MUX_P3_NG) setMultiplexer(ADS1115_MUX_P3_NG);
    if (devMode == ADS1115_MODE_SINGLESHOT) setOpStatus(ADS1115_OS_ACTIVE);
    return getDifferential();
}

// CONFIG register

/** Get operational status.
 * @return Current operational status (0 for active conversion, 1 for inactive)
 * @see ADS1115_OS_ACTIVE
 * @see ADS1115_OS_INACTIVE
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_OS_BIT
 */
uint8_t ADS1115::getOpStatus() {
    I2Cdev::readBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_OS_BIT, buffer);
    return buffer[0];
}
/** Set operational status.
 * This bit can only be written while in power-down mode (no conversions active).
 * @param status New operational status (0 does nothing, 1 to trigger conversion)
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_OS_BIT
 */
void ADS1115::setOpStatus(uint8_t status) {
    I2Cdev::writeBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_OS_BIT, status);
}
/** Get multiplexer connection.
 * @return Current multiplexer connection setting
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_MUX_BIT
 * @see ADS1115_CFG_MUX_LENGTH
 */
uint8_t ADS1115::getMultiplexer() {
    I2Cdev::readBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_MUX_BIT, ADS1115_CFG_MUX_LENGTH, buffer);
    muxMode = (uint8_t)buffer[0];
    return muxMode;
}
/** Set multiplexer connection.
 * @param mux New multiplexer connection setting
 * @see ADS1115_MUX_P0_N1
 * @see ADS1115_MUX_P0_N3
 * @see ADS1115_MUX_P1_N3
 * @see ADS1115_MUX_P2_N3
 * @see ADS1115_MUX_P0_NG
 * @see ADS1115_MUX_P1_NG
 * @see ADS1115_MUX_P2_NG
 * @see ADS1115_MUX_P3_NG
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_MUX_BIT
 * @see ADS1115_CFG_MUX_LENGTH
 */
void ADS1115::setMultiplexer(uint8_t mux) {
    if (I2Cdev::writeBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_MUX_BIT, ADS1115_CFG_MUX_LENGTH, mux)) {
        muxMode = mux;
    }
}
/** Get programmable gain amplifier level.
 * @return Current programmable gain amplifier level
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_PGA_BIT
 * @see ADS1115_CFG_PGA_LENGTH
 */
uint8_t ADS1115::getGain() {
    I2Cdev::readBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_PGA_BIT, ADS1115_CFG_PGA_LENGTH, buffer);
    return buffer[0];
}
/** Set programmable gain amplifier level.
 * @param gain New programmable gain amplifier level
 * @see ADS1115_PGA_6P144
 * @see ADS1115_PGA_4P096
 * @see ADS1115_PGA_2P048
 * @see ADS1115_PGA_1P024
 * @see ADS1115_PGA_0P512
 * @see ADS1115_PGA_0P256
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_PGA_BIT
 * @see ADS1115_CFG_PGA_LENGTH
 */
void ADS1115::setGain(uint8_t gain) {
    I2Cdev::writeBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_PGA_BIT, ADS1115_CFG_PGA_LENGTH, gain);
}
/** Get device mode.
 * @return Current device mode
 * @see ADS1115_MODE_CONTINUOUS
 * @see ADS1115_MODE_SINGLESHOT
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_MODE_BIT
 */
uint8_t ADS1115::getMode() {
    I2Cdev::readBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_MODE_BIT, buffer);
    devMode = (uint8_t)buffer[0];
    return devMode;
}
/** Set device mode.
 * @param mode New device mode
 * @see ADS1115_MODE_CONTINUOUS
 * @see ADS1115_MODE_SINGLESHOT
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_MODE_BIT
 */
void ADS1115::setMode(uint8_t mode) {
    if (I2Cdev::writeBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_MODE_BIT, mode)) {
        devMode = mode;
    }
}
/** Get data rate.
 * @return Current data rate
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_DR_BIT
 * @see ADS1115_CFG_DR_LENGTH
 */
uint8_t ADS1115::getRate() {
    I2Cdev::readBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_DR_BIT, ADS1115_CFG_DR_LENGTH, buffer);
    return buffer[0];
}
/** Set data rate.
 * @param rate New data rate
 * @see ADS1115_RATE_8
 * @see ADS1115_RATE_16
 * @see ADS1115_RATE_32
 * @see ADS1115_RATE_64
 * @see ADS1115_RATE_128
 * @see ADS1115_RATE_250
 * @see ADS1115_RATE_475
 * @see ADS1115_RATE_860
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_DR_BIT
 * @see ADS1115_CFG_DR_LENGTH
 */
void ADS1115::setRate(uint8_t rate) {
    I2Cdev::writeBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_DR_BIT, ADS1115_CFG_DR_LENGTH, rate);
}
/** Get comparator mode.
 * @return Current comparator mode
 * @see ADS1115_COMP_MODE_HYSTERESIS
 * @see ADS1115_COMP_MODE_WINDOW
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_MODE_BIT
 */
uint8_t ADS1115::getComparatorMode() {
    I2Cdev::readBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_MODE_BIT, buffer);
    return buffer[0];
}
/** Set comparator mode.
 * @param mode New comparator mode
 * @see ADS1115_COMP_MODE_HYSTERESIS
 * @see ADS1115_COMP_MODE_WINDOW
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_MODE_BIT
 */
void ADS1115::setComparatorMode(uint8_t mode) {
    I2Cdev::writeBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_MODE_BIT, mode);
}
/** Get comparator polarity setting.
 * @return Current comparator polarity setting
 * @see ADS1115_COMP_POL_ACTIVE_LOW
 * @see ADS1115_COMP_POL_ACTIVE_HIGH
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_POL_BIT
 */
uint8_t ADS1115::getComparatorPolarity() {
    I2Cdev::readBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_POL_BIT, buffer);
    return buffer[0];
}
/** Set comparator polarity setting.
 * @param polarity New comparator polarity setting
 * @see ADS1115_COMP_POL_ACTIVE_LOW
 * @see ADS1115_COMP_POL_ACTIVE_HIGH
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_POL_BIT
 */
void ADS1115::setComparatorPolarity(uint8_t polarity) {
    I2Cdev::writeBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_POL_BIT, polarity);
}
/** Get comparator latch enabled value.
 * @return Current comparator latch enabled value
 * @see ADS1115_COMP_LAT_NON_LATCHING
 * @see ADS1115_COMP_LAT_LATCHING
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_LAT_BIT
 */
bool ADS1115::getComparatorLatchEnabled() {
    I2Cdev::readBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_LAT_BIT, buffer);
    return buffer[0];
}
/** Set comparator latch enabled value.
 * @param enabled New comparator latch enabled value
 * @see ADS1115_COMP_LAT_NON_LATCHING
 * @see ADS1115_COMP_LAT_LATCHING
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_LAT_BIT
 */
void ADS1115::setComparatorLatchEnabled(bool enabled) {
    I2Cdev::writeBitW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_LAT_BIT, enabled);
}
/** Get comparator queue mode.
 * @return Current comparator queue mode
 * @see ADS1115_COMP_QUE_ASSERT1
 * @see ADS1115_COMP_QUE_ASSERT2
 * @see ADS1115_COMP_QUE_ASSERT4
 * @see ADS1115_COMP_QUE_DISABLE
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_QUE_BIT
 * @see ADS1115_CFG_COMP_QUE_LENGTH
 */
uint8_t ADS1115::getComparatorQueueMode() {
    I2Cdev::readBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_QUE_BIT, ADS1115_CFG_COMP_QUE_LENGTH, buffer);
    return buffer[0];
}
/** Set comparator queue mode.
 * @param mode New comparator queue mode
 * @see ADS1115_COMP_QUE_ASSERT1
 * @see ADS1115_COMP_QUE_ASSERT2
 * @see ADS1115_COMP_QUE_ASSERT4
 * @see ADS1115_COMP_QUE_DISABLE
 * @see ADS1115_RA_CONFIG
 * @see ADS1115_CFG_COMP_QUE_BIT
 * @see ADS1115_CFG_COMP_QUE_LENGTH
 */
void ADS1115::setComparatorQueueMode(uint8_t mode) {
    I2Cdev::writeBitsW(devAddr, ADS1115_RA_CONFIG, ADS1115_CFG_COMP_QUE_BIT, ADS1115_CFG_COMP_QUE_LENGTH, mode);
}

// *_THRESH registers

/** Get low threshold value.
 * @return Current low threshold value
 * @see ADS1115_RA_LO_THRESH
 */
int16_t ADS1115::getLowThreshold() {
    I2Cdev::readWord(devAddr, ADS1115_RA_LO_THRESH, buffer);
    return buffer[0];
}
/** Set low threshold value.
 * @param threshold New low threshold value
 * @see ADS1115_RA_LO_THRESH
 */
void ADS1115::setLowThreshold(int16_t threshold) {
    I2Cdev::writeWord(devAddr, ADS1115_RA_LO_THRESH, threshold);
}
/** Get high threshold value.
 * @return Current high threshold value
 * @see ADS1115_RA_HI_THRESH
 */
int16_t ADS1115::getHighThreshold() {
    I2Cdev::readWord(devAddr, ADS1115_RA_HI_THRESH, buffer);
    return buffer[0];
}
/** Set high threshold value.
 * @param threshold New high threshold value
 * @see ADS1115_RA_HI_THRESH
 */
void ADS1115::setHighThreshold(int16_t threshold) {
    I2Cdev::writeWord(devAddr, ADS1115_RA_HI_THRESH, threshold);
}