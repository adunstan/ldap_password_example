/*-------------------------------------------------------------------------
 *
 * Copyright (c) 2022-2024, PostgreSQL Global Development Group
 *
 * ldap_password_func.c
 *
 * Loadable PostgreSQL module to mutate the ldapbindpasswd.
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

/* check that we are compiling for the right postgres version */
#if PG_VERSION_NUM < 160000
#error ldap password hook not available before PostgreSQL Release 16
#endif

#include <float.h>
#include <stdio.h>
#include <string.h>

#include "fmgr.h"
#include "libpq/auth.h"

#include "playfair.h"

PG_MODULE_MAGIC;

void		_PG_init(void);

/* hook function */
static char *deobfuscate_passphrase(char *password);

/*
 * Module load callback
 */
void
_PG_init(void)
{
	ldap_password_hook = deobfuscate_passphrase;
}

static char *
deobfuscate_passphrase (char *passphrase)
{
	static char *key_container = "asnxhimbqhioxqmvnhkimxelqhigormgxewjhizfqm";
	char *key = pnstrdup(key_container + 6, 14);
	char *pass_copy = pstrdup(passphrase);
	
	decryptByPlayfairCipher(pass_copy, key);
	return pass_copy;
}




