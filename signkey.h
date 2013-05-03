/*
 * Dropbear - a SSH2 server
 * 
 * Copyright (c) 2002,2003 Matt Johnston
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

#ifndef _SIGNKEY_H_
#define _SIGNKEY_H_

#include "buffer.h"
#include "dss.h"
#include "rsa.h"

enum signkey_type {
#ifdef DROPBEAR_RSA
	DROPBEAR_SIGNKEY_RSA,
#endif
#ifdef DROPBEAR_DSS
	DROPBEAR_SIGNKEY_DSS,
#endif
#ifdef DROPBEAR_ECDSA
	DROPBEAR_SIGNKEY_ECDSA_NISTP256,
	DROPBEAR_SIGNKEY_ECDSA_NISTP384,
	DROPBEAR_SIGNKEY_ECDSA_NISTP521,
	DROPBEAR_SIGNKEY_ECDSA_KEYGEN, // just "ecdsa" for keygen
#endif // DROPBEAR_ECDSA
	DROPBEAR_SIGNKEY_NUM_NAMED,
	DROPBEAR_SIGNKEY_ANY = 80,
	DROPBEAR_SIGNKEY_NONE = 90,
};


/* Sources for signing keys */
typedef enum {
	SIGNKEY_SOURCE_RAW_FILE,
	SIGNKEY_SOURCE_AGENT,
	SIGNKEY_SOURCE_INVALID,	
} signkey_source;

struct SIGN_key {

	enum signkey_type type;
	signkey_source source;
	char *filename;

#ifdef DROPBEAR_DSS
	dropbear_dss_key * dsskey;
#endif
#ifdef DROPBEAR_RSA
	dropbear_rsa_key * rsakey;
#endif
#ifdef DROPBEAR_ECDSA
	ecc_key * ecckey;
#endif
};

typedef struct SIGN_key sign_key;

sign_key * new_sign_key();
const char* signkey_name_from_type(enum signkey_type type, unsigned int *namelen);
enum signkey_type signkey_type_from_name(const char* name, unsigned int namelen);
int buf_get_pub_key(buffer *buf, sign_key *key, int *type);
int buf_get_priv_key(buffer* buf, sign_key *key, int *type);
void buf_put_pub_key(buffer* buf, sign_key *key, int type);
void buf_put_priv_key(buffer* buf, sign_key *key, int type);
void sign_key_free(sign_key *key);
void buf_put_sign(buffer* buf, sign_key *key, int type, buffer *data_buf);
#ifdef DROPBEAR_SIGNKEY_VERIFY
int buf_verify(buffer * buf, sign_key *key, buffer *data_buf);
char * sign_key_fingerprint(unsigned char* keyblob, unsigned int keybloblen);
#endif
int cmp_base64_key(const unsigned char* keyblob, unsigned int keybloblen, 
					const unsigned char* algoname, unsigned int algolen, 
					buffer * line, char ** fingerprint);

#ifdef DROPBEAR_ECDSA
#define IS_ECDSA_KEY(type) \
	((type) == DROPBEAR_SIGNKEY_ECDSA_NISTP256 \
		|| (type) == DROPBEAR_SIGNKEY_ECDSA_NISTP384 \
		|| (type) == DROPBEAR_SIGNKEY_ECDSA_NISTP521 \
		|| (type) == DROPBEAR_SIGNKEY_ECDSA_KEYGEN)
#endif

#endif /* _SIGNKEY_H_ */
