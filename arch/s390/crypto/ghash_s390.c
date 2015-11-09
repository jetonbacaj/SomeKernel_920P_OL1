/*
 * Cryptographic API.
 *
 * s390 implementation of the GHASH algorithm for GCM (Galois/Counter Mode).
 *
 * Copyright IBM Corp. 2011
 * Author(s): Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#include <crypto/internal/hash.h>
#include <linux/module.h>

#include "crypt_s390.h"

#define GHASH_BLOCK_SIZE	16
#define GHASH_DIGEST_SIZE	16

struct ghash_ctx {
<<<<<<< HEAD
	u8 key[GHASH_BLOCK_SIZE];
};

struct ghash_desc_ctx {
	u8 icv[GHASH_BLOCK_SIZE];
	u8 key[GHASH_BLOCK_SIZE];
=======
	u8 icv[16];
	u8 key[16];
};

struct ghash_desc_ctx {
>>>>>>> G920FXXU3COI9
	u8 buffer[GHASH_BLOCK_SIZE];
	u32 bytes;
};

static int ghash_init(struct shash_desc *desc)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
<<<<<<< HEAD
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);

	memset(dctx, 0, sizeof(*dctx));
	memcpy(dctx->key, ctx->key, GHASH_BLOCK_SIZE);
=======

	memset(dctx, 0, sizeof(*dctx));
>>>>>>> G920FXXU3COI9

	return 0;
}

static int ghash_setkey(struct crypto_shash *tfm,
			const u8 *key, unsigned int keylen)
{
	struct ghash_ctx *ctx = crypto_shash_ctx(tfm);

	if (keylen != GHASH_BLOCK_SIZE) {
		crypto_shash_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	memcpy(ctx->key, key, GHASH_BLOCK_SIZE);
<<<<<<< HEAD
=======
	memset(ctx->icv, 0, GHASH_BLOCK_SIZE);
>>>>>>> G920FXXU3COI9

	return 0;
}

static int ghash_update(struct shash_desc *desc,
			 const u8 *src, unsigned int srclen)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
<<<<<<< HEAD
=======
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);
>>>>>>> G920FXXU3COI9
	unsigned int n;
	u8 *buf = dctx->buffer;
	int ret;

	if (dctx->bytes) {
		u8 *pos = buf + (GHASH_BLOCK_SIZE - dctx->bytes);

		n = min(srclen, dctx->bytes);
		dctx->bytes -= n;
		srclen -= n;

		memcpy(pos, src, n);
		src += n;

		if (!dctx->bytes) {
<<<<<<< HEAD
			ret = crypt_s390_kimd(KIMD_GHASH, dctx, buf,
=======
			ret = crypt_s390_kimd(KIMD_GHASH, ctx, buf,
>>>>>>> G920FXXU3COI9
					      GHASH_BLOCK_SIZE);
			if (ret != GHASH_BLOCK_SIZE)
				return -EIO;
		}
	}

	n = srclen & ~(GHASH_BLOCK_SIZE - 1);
	if (n) {
<<<<<<< HEAD
		ret = crypt_s390_kimd(KIMD_GHASH, dctx, src, n);
=======
		ret = crypt_s390_kimd(KIMD_GHASH, ctx, src, n);
>>>>>>> G920FXXU3COI9
		if (ret != n)
			return -EIO;
		src += n;
		srclen -= n;
	}

	if (srclen) {
		dctx->bytes = GHASH_BLOCK_SIZE - srclen;
		memcpy(buf, src, srclen);
	}

	return 0;
}

<<<<<<< HEAD
static int ghash_flush(struct ghash_desc_ctx *dctx)
=======
static int ghash_flush(struct ghash_ctx *ctx, struct ghash_desc_ctx *dctx)
>>>>>>> G920FXXU3COI9
{
	u8 *buf = dctx->buffer;
	int ret;

	if (dctx->bytes) {
		u8 *pos = buf + (GHASH_BLOCK_SIZE - dctx->bytes);

		memset(pos, 0, dctx->bytes);

<<<<<<< HEAD
		ret = crypt_s390_kimd(KIMD_GHASH, dctx, buf, GHASH_BLOCK_SIZE);
		if (ret != GHASH_BLOCK_SIZE)
			return -EIO;

		dctx->bytes = 0;
	}

=======
		ret = crypt_s390_kimd(KIMD_GHASH, ctx, buf, GHASH_BLOCK_SIZE);
		if (ret != GHASH_BLOCK_SIZE)
			return -EIO;
	}

	dctx->bytes = 0;
>>>>>>> G920FXXU3COI9
	return 0;
}

static int ghash_final(struct shash_desc *desc, u8 *dst)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
<<<<<<< HEAD
	int ret;

	ret = ghash_flush(dctx);
	if (!ret)
		memcpy(dst, dctx->icv, GHASH_BLOCK_SIZE);
=======
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);
	int ret;

	ret = ghash_flush(ctx, dctx);
	if (!ret)
		memcpy(dst, ctx->icv, GHASH_BLOCK_SIZE);
>>>>>>> G920FXXU3COI9
	return ret;
}

static struct shash_alg ghash_alg = {
	.digestsize	= GHASH_DIGEST_SIZE,
	.init		= ghash_init,
	.update		= ghash_update,
	.final		= ghash_final,
	.setkey		= ghash_setkey,
	.descsize	= sizeof(struct ghash_desc_ctx),
	.base		= {
		.cra_name		= "ghash",
		.cra_driver_name	= "ghash-s390",
		.cra_priority		= CRYPT_S390_PRIORITY,
		.cra_flags		= CRYPTO_ALG_TYPE_SHASH,
		.cra_blocksize		= GHASH_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct ghash_ctx),
		.cra_module		= THIS_MODULE,
	},
};

static int __init ghash_mod_init(void)
{
	if (!crypt_s390_func_available(KIMD_GHASH,
				       CRYPT_S390_MSA | CRYPT_S390_MSA4))
		return -EOPNOTSUPP;

	return crypto_register_shash(&ghash_alg);
}

static void __exit ghash_mod_exit(void)
{
	crypto_unregister_shash(&ghash_alg);
}

module_init(ghash_mod_init);
module_exit(ghash_mod_exit);

<<<<<<< HEAD
MODULE_ALIAS_CRYPTO("ghash");
=======
MODULE_ALIAS("ghash");
>>>>>>> G920FXXU3COI9

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GHASH Message Digest Algorithm, s390 implementation");
