# **Rockchip librkctypto 开发指南**

发布版本：V0.04

日期：2021.12

文件密级：公开资料



**免责声明**

本文档按“现状”提供，瑞芯微电子股份有限公司（“本公司”，下同）不对本文档的任何陈述、信息和内容的准确性、可靠性、完整性、适销性、特定目的性和非侵权性提供任何明示或暗示的声明或保证。本文档仅作为使用指导的参考。

由于产品版本升级或其他原因，本文档将可能在未经任何通知的情况下，不定期进行更新或修改。

**商标声明**

“Rockchip”、“瑞芯微”、“瑞芯”均为本公司的注册商标，归本公司所有。

本文档可能提及的其他所有注册商标或商标，由其各自拥有者所有。

**版权所有© 2018瑞芯微电子股份有限公司**

超越合理使用范畴，非经本公司书面许可，任何单位和个人不得擅自摘抄、复制本文档内容的部分或全部，并不得以任何形式传播。



瑞芯微电子股份有限公司

Fuzhou Rockchip Electronics Co., Ltd.

地址：     福建省福州市铜盘路软件园A区18号

网址：     [www.rock-chips.com](http://www.rock-chips.com)

客户服务电话： +86-591-83991906

客户服务传真： +86-591-83951833

客户服务邮箱： [www.rock-chips.com](http://www.rock-chips.com)

------

**前言**

**概述**

本文档主要介绍Rockchip librkcrypto接口使用说明。librkcrypto提供基于硬件的安全算法接口。


**读者对象**

本文档（本指南）主要适用于以下工程师：

- 技术支持工程师
- 软件开发工程师

**修订记录**

|    日期    | 版本  |  作者  |                    修改说明                    |
| :--------: | :---: | :----: | :--------------------------------------------: |
| 2021-12-15 | V0.03 | 王小滨 |                    draft                    |
------
[TOC]

------

## 适用范围

| 文档适用范围    |     备注      |
| :------------: | :----------: |
|     RK3588     |              |

## 注意事项

* 对称算法的输入数据长度，要求与所选算法的block对齐
* 为了提高效率，建议选用通过物理地址传递数据的算法接口 TODO:待补充

## 数据结构

### rk_cipher_memory

```c
typedef struct {
	uint64_t	phys_addr;
	uint64_t	virt_addr;
} rk_cipher_memory;
```

* phys_addr - 物理地址
* virt_addr - 与phys_addr对应的虚拟地址

### rk_cipher_config

```c
typedef struct {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	operation;
	uint8_t		key[64];
	uint32_t	key_len;
	uint8_t		iv[16];
	void		*reserved;
} rk_cipher_config;
```

* algo - 算法类型，见[RK_CRYPTO_ALGO](#RK_CRYPTO_ALGO)，实际取值范围以API的描述为准，下同
* mode - 算法模式，见[RK_CIPIHER_MODE](#RK_CIPIHER_MODE), 支持ECB/CBC/CTS/CTR/CFB/OFB/XTS
* operation - 加解密模式见[Algorithm operation](#AlgorithmOperation)
* key - 密钥明文，当使用keyladder操作时无效
* key_len - key的Byte长度
* iv - 初始向量，当ECB模式时无效
* reserved - 预留

### rk_ae_config

```c
typedef struct {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	operation;
	uint8_t		key[32];
	uint32_t	key_len;
	uint8_t		iv[16];
	uint32_t	iv_len;
	uint32_t	tag_len;
	uint32_t	aad_len;
	uint32_t	payload_len;
	void		*reserved;
} rk_ae_config;
```

* algo - 算法类型，见[RK_CRYPTO_ALGO](#RK_CRYPTO_ALGO)，支持AES/SM4
* mode - 算法模式，见[RK_CIPIHER_MODE](#RK_CIPIHER_MODE), 支持GCM/CCM
* operation - 加解密模式见[Algorithm operation](#AlgorithmOperation)
* key - 密钥明文，当使用keyladder操作时无效
* key_len - key的Byte长度
* iv - 初始向量
* iv_len - iv的Byte长度
* tag_len - tag的Byte长度
* aad_len - aad的Byte长度
* payload_len - payload的Byte长度
* reserved - 预留

### rk_hash_config

```c
typedef struct {
	uint32_t	algo;
	uint8_t		*key;
	uint32_t	key_len;
} rk_hash_config;
```

* algo - hash算法类型
* key - hash-mac密钥，非hash-mac算法时无效
* key_len - key的Byte长度

### rk_hash_ctx

```c
typedef struct{
	const uint8_t *null_hash;
	uint32_t magic;
	uint32_t algo;
	uint32_t digest_size;
	uint32_t dma_started;
	uint32_t data_lli_head;
}rk_hash_ctx;
```

* null_hash - when hash is null or length is zero ,used this as result
* magic - to check whether the ctx is correct
* algo - hash algo
* digest_size - hash out length according to hash algo
* dma_started - choose use start or restart
* data_lli_heda - to recored the lli that not computed

## 常量

### RK_CRYPTO_ALGO

```c
/* Crypto algorithm */
enum RK_CRYPTO_ALGO {
	RK_ALGO_AES = 1,
	RK_ALGO_DES,
	RK_ALGO_TDES,
	RK_ALGO_SM4,
	RK_ALGO_ALGO_MAX
};
```

### RK_CIPIHER_MODE

```c
/* Crypto mode */
enum RK_CIPIHER_MODE {
	RK_CIPHER_MODE_ECB = 0,
	RK_CIPHER_MODE_CBC = 1,
	RK_CIPHER_MODE_CTS = 2,
	RK_CIPHER_MODE_CTR = 3,
	RK_CIPHER_MODE_CFB = 4,
	RK_CIPHER_MODE_OFB = 5,
	RK_CIPHER_MODE_XTS = 6,
	RK_CIPHER_MODE_CCM = 7,
	RK_CIPHER_MODE_GCM = 8,
	RK_CIPHER_MODE_CMAC = 9,
	RK_CIPHER_MODE_CBC_MAC = 10,
	RK_CIPHER_MODE_MAX
};
```

### RK_OEM_HR_OTP_KEYID

```c
/* Hardware readable keys, handle by keyladder */
enum RK_OEM_HR_OTP_KEYID {
	RK_OEM_HR_OTP_KEY0 = 0,		/* keyladder key0 */
	RK_OEM_HR_OTP_KEY1 = 1,		/* keyladder key1 */
	RK_OEM_HR_OTP_KEY2 = 2,		/* keyladder key2 */
	RK_OEM_HR_OTP_KEY3 = 3,		/* keyladder key3 */
	RK_OEM_HR_OTP_KEYMAX
};
```

### AlgorithmOperation

```c
/* Algorithm operation */
#define RK_MODE_ENCRYPT			1
#define RK_MODE_DECRYPT			0
```

### 其他常量

```c
/* Algorithm block length */
#define DES_BLOCK_SIZE		8
#define AES_BLOCK_SIZE		16
#define SM4_BLOCK_SIZE		16
#define SHA1_HASH_SIZE		20
#define SHA224_HASH_SIZE	28
#define SHA256_HASH_SIZE	32
#define SHA384_HASH_SIZE	48
#define SHA512_HASH_SIZE	64
#define MD5_HASH_SIZE		16
#define SM3_HASH_SIZE		32
#define AES_AE_DATA_BLOCK	128
#define MAX_HASH_BLOCK_SIZE	128
#define MAX_TDES_KEY_SIZE	24
#define MAX_AES_KEY_SIZE	32

#define RK_CRYPTO_MAX_DATA_LEN		(1 * 1024 * 1024)
```

## API

### 数据类型

```c
typedef uint32_t RK_RES;
```

### 返回值

```c
/* API return codes */
#define RK_ALG_SUCCESS			0x00000000
#define RK_ALG_ERR_GENERIC		0xF0000000
#define RK_ALG_ERR_PARAMETER		0xF0000001
#define RK_ALG_ERR_STATE		0xF0000002
#define RK_ALG_ERR_NOT_SUPPORTED	0xF0000003
#define RK_ALG_ERR_OUT_OF_MEMORY	0xF0000004
```

### rk_crypto_malloc

```c
RK_RES rk_crypto_malloc(uint32_t size， rk_cipher_memory *memory);
```

**功能**
申请一块内存，返回内存的物理地址和虚拟地址。

**参数**
* [in] size - 待申请内存的大小
* [out] memory - 返回的内存地址，见[rk_cipher_memory](#rk_cipher_memory)

### rk_crypto_free

```c
void rk_crypto_free(rk_cipher_memory *memory);
```

**功能**
释放通过`rk_crypto_malloc`申请的内存。

**参数**
* [in] memory - 内存地址，见[rk_cipher_memory](#rk_cipher_memory)

### rk_crypto_init

```c
RK_RES rk_crypto_init(void);
```

**功能**
crypto初始化，例如打开设备节点等。

**参数**
* 无

### rk_crypto_destroy

```c
void rk_crypto_destroy(void);
```

**功能**
释放crypto相关资源，例如关闭设备节点等。

**参数**
* 无

### rk_get_random

```c
RK_RES rk_get_random(uint32_t len, uint8_t *data);
```

**功能**
获取硬件随机数。

**参数**
* [in] len - 需获取的随机数Byte长度
* [out] data - 返回的随机数buffer

### rk_hash_init

```c
RK_RES rk_hash_init(rk_hash_ctx *ctx, uint32_t algo);
```

**功能**
初始化hash算法，支持SHA256, SHA384, SHA512。

**参数**

* [out] ctx - hash算法的context, 见[rk_hash_ctx](#rk_hash_ctx)
* [in] algo - hash算法类型

### rk_hash_update

```c
RK_RES rk_hash_update(rk_hash_ctx *ctx, uint8_t *data, uint32_t data_len);
```

**功能**

计算hash值，支持分组多次计算。

**参数**

* [in] ctx - hash算法的context, 首次update的ctx经过`rk_hash_init`初始化
* [in] data - 待计算hash的一组数据
* [in] data_len - data的Byte长度

### rk_hash_final

```c
RK_RES rk_hash_final(rk_hash_ctx *ctx, uint8_t *hash, uint8_t *hash_len);
```

**功能**

获取hash值，在计算完所有的数据后，调用这个接口获取最终的hash值，并清除ctx。如果在计算过程中，需要中断计算，也必须调用该接口结束hash计算。

**参数**

* [in] ctx - hash算法的context
* [out] hash - 输出的hash数据
* [out] hash_len - hash数据的Byte长度

### rk_cipher_init

```c
RK_RES rk_cipher_init(rk_cipher_config *config, uint32_t cipher_handle);
```

**功能**
对称分组算法的初始化，支持TDES，AES, SM4, 支持ECB/CBC/CTS/CTR/CFB/OFB/XTS。

**参数**
* [in] config - 算法、模式、密钥、iv等，见[rk_cipher_config](#rk_cipher_config)
* [out] cipher_handle - 输出的handle

### rk_cipher_update

```c
RK_RES rk_cipher_update(uint32_t cipher_handle, uint8_t *in, uint32_t in_len,
                        uint8_t *out, uint32_t *out_len);
```

**功能**
对称分组算法，开始加解密。

**参数**
* [in] cipher_handle - 输入的handle
* [in] in - 输入数据buffer
* [in] in_len - 输入数据的Byte长度
* [out] out - 输出计算结果
* [out] out_len - 计算结果的Byte长度

### rk_cipher_final

```c
RK_RES rk_cipher_final(uint32_t cipher_handle);
```

**功能**
对称分组算法，结束计算，清除handle。

**参数**
* [in] cipher_handle - 输入的handle

### rk_write_oem_hr_otp

```c
RK_RES rk_write_oem_hr_otp(enum RK_OEM_HR_OTP_KEYID key_id, uint8_t *key, 			     uint32_t key_len);
```

**功能**
把明文密钥写到指定的OEM_HR_OTP区域。
OEM_HR_OTP的相关特性说明，见`Rockchip_Developer_Guide_OTP_SUMMARY_CN.md
`文档。

**参数**
* [in] key_id - 要写的key_id，见[RK_OEM_HR_OTP_KEYID](#RK_OEM_HR_OTP_KEYID)
* [in] key - 明文密钥
* [in] key_len - 明文密钥的Byte长度，支持16、24、32

### rk_set_oem_hr_otp_read_lock

```c
RK_RES rk_set_oem_hr_otp_read_lock(enum RK_OEM_HR_OTP_KEYID key_id);
```

**功能**
设置指定OEM_HR_OTP区域的read lock标志，设置成功后，该区域的密钥CPU软件不可读，可通过rk_keylad_cipher接口使用密钥。
OEM_HR_OTP的相关特性说明，见`Rockchip_Developer_Guide_OTP_SUMMARY_CN.md
`文档。

**参数**
* [in] key_id - 要设置的key_id，见[RK_OEM_HR_OTP_KEYID](#RK_OEM_HR_OTP_KEYID)


### rk_keylad_cipher

```c
RK_RES rk_keylad_cipher(enum RK_OEM_HR_OTP_KEYID key_id,
			rk_cipher_config *config,
			uint8_t *src, uint8_t *dest, uint32_t len);
```

**功能**
选择OEM_HR_OTP区域的密钥，执行cipher单次计算，支持AES, SM4, 支持ECB/CBC/CTS/CTR/CFB/OFB模式，密钥长度支持16、24、32Bytes。

**参数**
* [in] key_id - 要使用的key_id，见[RK_OEM_HR_OTP_KEYID](#RK_OEM_HR_OTP_KEYID)
* [in] config - 算法、模式、密钥、iv等
* [in] src - 待计算的数据buffer, 要求与所用算法的block对齐
* [out] dest - 计算结果buffer
* [in] len - src和dest的Byte长度，最大支持1MB


