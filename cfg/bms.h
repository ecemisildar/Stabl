//
// Created by StablGuest on 11/08/2021.
//

#ifndef STABL_BMS_H
#define STABL_BMS_H

/**
 * @brief Active module in C
 */
enum bms_modules{
    BMS_SAMSUNG,
    BMS_DAIMLER,
};

/**
 * @brief Configuration parameters for the the BMS driver
 */
typedef struct {
    uint8_t phase;              /**< 1:0 bits Number of phases, 3:2 Active phases, 4 Current direction, */
    uint8_t modules;            /**< 3:0 Number of modules per string, 7:4 Defines which module type is active @ref  */
    uint8_t safety_errors;      /**< 0 Current error check, 1 voltage error check */
    float max_current;          /**< Max current that can be entered by the user */
    uint8_t temp_sensors;/**< Defines the number of temperatures */
    uint8_t humid_sensors;
}bms_parameters_t;

/**
 * @brief Load the configurations from flash
 */
int configure_driver(bms_parameters_t *dev, MFSDriver *flash_dev);

#endif //STABL_BMS_H
