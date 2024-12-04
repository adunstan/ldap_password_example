
Example of LDAP password hook for PostgreSQL
============================================

This code demonstrates how to build and deploy a library implementing
the PostgreSQL `ldap_password_hook`. Such a module is designed to
obfuscate the password so that you don't need to store the
ldap_bind_password in plain text in the configuration file.

### Algorithm

Tha algorithm implemented here is a very old cipher called 
[Playfair](https://en.wikipedia.org/wiki/Playfair_cipher).

This cipher requires significantly more work to break than the Rot13
cipher used in the core PostgreSQL test module for this feature. But
it is still very breakable, and users are advised to replace its use
in this module with one of their choosing. The author takes no
responsibility for the use of this module as is. You have been warned.

The Playfair key is not passed in to the library, rather it is stored
in the object files. If you insist on using (at your own risk) the
Playfair code, you should at least replace the key_container strings
in `ldap_password_func.c` and `pf_encrypt.c`. These need to match.

The Playfair code was adapted from
https://www.scaler.com/topics/playfair-cipher-program-in-c/

The current code is classic Playfair, which uses a 5x5 matrix and
elides I and J, as well as folding to lower case. That's pretty
limiting. A paper at
https://ijs.uobaghdad.edu.iq/index.php/eijs/article/view/3864/1642
shows an enhanced Playfair scheme, using a 10x10 matrix and a second
permutation key, which makes it more resistant to certain types of
attack. as well as dealing with the limited alphabet and case
issue, to some extent. The paper also reviews some related work.

Instead of improving the Playfair code, you can and probably should
implement an alternative scheme. For example, you could make the
library fetch the password from some password vault. The point of this
repo is not to provide a completely robust obfuscation scheme, but to
demonstrate how to build and deploy a library utilising the
hook. Again, you have been warned.

### Building

Place `pg_config` in your path and execute the following:

    - `make`
	- `make -f Makefile.exe`
	
This should build two artefacts: `ldap_password_func.so` and
`obfuscate` (or if you build with on Windows using Msys,
`ldap_password_func.dll` and `obfuscate.exe`).

The latter is a standalone program to obfuscate a password for use
with the shared library.

You should build these airgapped from your production instances. The
only part that should ever go on your production servers is the shared
library.

### Deployment

- Put the shared library in the `PKGLIBDIR` directory of your Postgres
  installation (see `pg_config --pkglibdir`)
- Get the bindpassword for your ldap installation. Currently this
  library only supports passwords that only consist of lower case
  ASCII characters, without a `j`.
- On your airgapped machine where you build the objects, get the
  obfuscated version of the password, by passing it as the first
  argument to the `obfuscate` program.
- In your cluster's `postgresql.conf`, add `ldap_password_func` to the
  `shared_preload_libraries` setting.
- In your cluster's `pg_hba.conf` file, use the obfuscated password
  for the `ldapbindpasswd` setting where it's required.
- Restart the server
  
### Notes

- Building and deployment for Windows using Visual C are left as an
  exercise for the reader.
- Supporting characters other than lower case ASCII is also left as an
  exercise for the reader. You'd be better off using a different
  algorithm rather than try to make this one better.
  





