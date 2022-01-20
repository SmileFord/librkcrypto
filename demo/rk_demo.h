/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RK_DEMO_H_
#define _RK_DEMO_H_

#include "rkcrypto_common.h"

RK_RES demo_otpkey(void);
RK_RES demo_otpkey_virt(void);

RK_RES demo_cipher(void);
RK_RES demo_cipher_virt(void);

RK_RES demo_hash(void);
RK_RES demo_hash_virt(void);

RK_RES demo_hmac(void);
RK_RES demo_hmac_virt(void);

#endif /*_RK_DEMO_H_*/
