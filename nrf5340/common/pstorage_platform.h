/**
 * Copyright (c) 2017 - 2017, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 /** @cond To make doxygen skip this file */

/** @file
 *  This header contains defines with respect persistent storage that are specific to
 *  persistent storage implementation and application use case.
 */
#ifndef PSTORAGE_PL_H__
#define PSTORAGE_PL_H__

#include <stdint.h>
#include "nrf.h"
#include "flash_nrf5x.h"

static __INLINE uint16_t pstorage_flash_page_size()
{
  return (uint16_t)NRF_FICR->INFO.CODEPAGESIZE;
}

#define PSTORAGE_FLASH_PAGE_SIZE     pstorage_flash_page_size()          /**< Size of one flash page. */
#define PSTORAGE_FLASH_EMPTY_MASK    0xFFFFFFFF                          /**< Bit mask that defines an empty address in flash. */

static __INLINE uint32_t pstorage_flash_page_end()
{
  uint32_t bootloader_addr = 0;   //NRF_UICR->NRFFW[0]; TBD

  return ((bootloader_addr != PSTORAGE_FLASH_EMPTY_MASK) ? (bootloader_addr / PSTORAGE_FLASH_PAGE_SIZE) : NRF_FICR->INFO.CODESIZE);  //TBD
}

#define PSTORAGE_FLASH_PAGE_END     pstorage_flash_page_end()

#define PSTORAGE_NUM_OF_PAGES       4 //3                                                           /**< Number of flash pages allocated for the pstorage module excluding the swap page, configurable based on system requirements. */
#define PSTORAGE_MIN_BLOCK_SIZE     0x0010                                                      /**< Minimum size of block that can be registered with the module. Should be configured based on system requirements, recommendation is not have this value to be at least size of word. */

#define PSTORAGE_DATA_START_ADDR    ((PSTORAGE_FLASH_PAGE_END - PSTORAGE_NUM_OF_PAGES) \
                                    * PSTORAGE_FLASH_PAGE_SIZE)                                 /**< Start address for persistent data, configurable according to system requirements. */
#define PSTORAGE_DATA_END_ADDR      (PSTORAGE_FLASH_PAGE_END * PSTORAGE_FLASH_PAGE_SIZE)        /**< End address for persistent data, configurable according to system requirements. */
#define PSTORAGE_SWAP_ADDR          PSTORAGE_DATA_END_ADDR

#define PSTORAGE_MAX_BLOCK_SIZE     PSTORAGE_FLASH_PAGE_SIZE                                    /**< Maximum size of block that can be registered with the module. Should be configured based on system requirements. And should be greater than or equal to the minimum size. */
#define PSTORAGE_CMD_QUEUE_SIZE     10                                                          /**< Maximum number of flash access commands that can be maintained by the module for all applications. Configurable. */

/**@breif Define this flag in case Raw access to persistent memory is to be enabled. Raw mode
 * unlike the data mode is for uses other than storing data from various mode. This mode is
 * employed when unpdating firmware or similar uses. Therefore, this mode shall be enabled
 * only for these special usecases and typically disabled.
 */
#define PSTORAGE_RAW_MODE_ENABLE

/** Abstracts persistently memory block identifier. */
typedef uint32_t pstorage_block_t;

typedef struct
{
    uint32_t            module_id;      /**< Module ID.*/
    pstorage_block_t    block_id;       /**< Block ID.*/
} pstorage_handle_t;

typedef uint32_t pstorage_size_t;      /** Size of length and offset fields. */

/**@brief Handles Flash Access Result Events. To be called in the system event dispatcher of the application. */
void pstorage_sys_event_handler (uint32_t sys_evt);

#endif // PSTORAGE_PL_H__

/** @} */
/** @endcond */
