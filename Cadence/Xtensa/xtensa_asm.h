/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2015-2024 Cadence Design Systems, Inc.
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef XTENSA_ASM_H
#define XTENSA_ASM_H

#include <FreeRTOSConfig.h>

#if ( configNUMBER_OF_CORES > 1 )

/*
*******************************************************************************
* Macro to return core ID into regieter r; trashes register t on some configs.
* Useful for assembly implementations of xthal_get_coreid().
*******************************************************************************
*/
    .macro  coreid  r, t
    rsr.prid    \r
#if XCHAL_SUBSYS_CORE_ID_BITS
    // TODO: try and use extui to save 1 instruction...
    movi    \t,  XCHAL_SUBSYS_CORE_ID_MASK
    and     \r, \r, \t
#endif
    .endm

#endif

/*
*******************************************************************************
* Macro to load a pointer to the current core's current TCB into register r;
* trashes register t on SMP configurations.
*******************************************************************************
*/
#if ( configNUMBER_OF_CORES == 1 )
    .extern pxCurrentTCB
#else
    .extern pxCurrentTCBs
#endif

    .macro  pxctcb  r, t
#if ( configNUMBER_OF_CORES == 1 )
    movi    \r,  pxCurrentTCB
#else
    coreid  \t,  \r
    movi    \r,  pxCurrentTCBs
    addx4   \r,  \t, \r
#endif
    .endm

/*
*******************************************************************************
* Macro to load a pointer to the current core's port_interruptNesting variable;
* trashes register t on SMP configurations.
*******************************************************************************
*/
#if ( configNUMBER_OF_CORES == 1 )
    .extern port_interruptNesting
#else
    .extern port_interruptNestings
#endif

    .macro  pintnest    r, t
#if ( configNUMBER_OF_CORES == 1 )
    movi    \r,  port_interruptNesting
#else
    coreid  \t,  \r
    movi    \r,  port_interruptNestings
    addx4   \r,  \t, \r
#endif
    .endm

#endif /* XTENSA_ASM_H */
