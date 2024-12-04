# Copyright (c) 2022-2024, PostgreSQL Global Development Group

# ldap_password_func Makefile

MODULE_big = ldap_password_func
OBJS = ldap_password_func.o playfair.o $(WIN32RES)
PGFILEDESC = "set hook to mutate ldapbindpasswd"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


