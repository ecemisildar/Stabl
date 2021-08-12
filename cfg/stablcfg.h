/** @file */
//
// Created by arthur on 12.03.20.
//


#ifndef MERGE3STRINGS_CONFIG_H
#define MERGE3STRINGS_CONFIG_H

#include "stdint.h"

/***********************************************************************************************************************
 * SYSTEM CONFIG
 **********************************************************************************************************************/

/**
 * @brief Set the number of phases, should be equal to the length of the enum
 * @todo code: Could be computed from the enum above
 */
#define NUM_PHASES 3U //number of phases in inverter

/**
 * @brief sets active phases by number, phases will be activated from bottom to top (activating only phase 2 isn't possible)
 */
#define NUM_ACTIVE_PHASES 1

/**
 * @brief number of modules per string, currently needs to be the same for all phases
 * @todo config,v3 what if different number of modules are used per phase?
 */
#define NUM_MODULES 9 //number of modules per phase

/**
 * @brief Defines which module type is active
 */
#define BATTERYMODUL_LEV50N   //Daimler Yuasa Modules Trostberg 6S2P
//#define BATTERYMODUL_SAMSUNG  // Samsung SDI Modules 16S

/**
 * @brief determines direction of current controller
 * @details negative means arrow of current sensor points towards M2B, sensor is on positive terminal of M2B
 *          Allowed values are -1 or +1
 */
#define CURRENT_DIRECTION -1


/***********************************************************************************************************************
 * SAFETY
 **********************************************************************************************************************/

/**
 * @brief current error check
 * @details If activated, current errors (max or max slope) set the system to error state. DO NOT RUN WITHOUT
 * Will disable runstate
 */
#define CURR_ERR_CHECK TRUE

/**
 * @brief voltage error check
 * @details If activated, voltage errors (max or max slope or max diff) set the system to error state. DO NOT RUN WITHOUT
 * Will disable runstate
 */
#define VOLT_ERR_CHECK TRUE

/**
 * @brief defines the max current that can be entered by the user, to be defined as a float
 */
#define MAX_CURRENT 150.01f

/**
 * @brief defines the number of temperatures
 */
#define NUM_TEMPERATURES 3

#endif //MERGE3STRINGS_CONFIG_H

/*** end of file ***/
