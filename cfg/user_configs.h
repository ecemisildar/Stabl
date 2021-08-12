//
// Created by StablGuest on 11/08/2021.
//
#ifndef STABL_USER_CONFIGS_H
#define STABL_USER_CONFIGS_H

/**
 * @brief OPCODES for configuration
 */
typedef enum user_commands_e {
    OPCODE_NUM_PHASES = 0x01,
    OPCODE_NUM_ACTIVE_PHASES = 0x02,
    OPCODE_NUM_MODULES = 0x03,
    OPCODE_BATTERY_MODULES = 0x04,
    OPCODE_CURRENT_DIRECTION = 0x05,
    OPCODE_CURR_ERR_CHECK = 0x06,
    OPCODE_VOLT_ERR_CHECK = 0x07,
    OPCODE_MAX_CURRENT = 0x08,
    OPCODE_NUM_TEMPERATURES = 0x09,
}user_commands_t;

/**
 * @brief Set the registry
 */
static inline int _set (user_commands_t cmd, uint8_t *buffer, uint8_t len ) {
    return mfsWriteRecord(&mfs1, cmd, len, buffer); // write into buffer
}

/**
 * @brief Get the registry
 *
 */
static int _get(user_commands_t cmd, uint8_t *buffer, uint8_t len) {
    mfsReadRecord(&mfs1, cmd, &len, buffer);
}
#endif //STABL_USER_CONFIGS_H
