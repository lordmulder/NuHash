![](etc/images/nuhash_logo.png)


Introduction
============

**NuHash** is a fast, portable and secure hashing library, released under the CC0 license.

The "core" library (*libnuhash*) is written in pure **C99**, but a high-level wrapper for **C++** is also provided.

Supported platforms include Linux, Windows, *BSD, Illumos, Haiku OS, GNU Hurd and MacOS X.


Getting Started
===============

The following example shows how to use the *libnuhash* “stream” API in C99 code:

```c
#include <nuhash.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    /* state variables */
    nuhash_t context;
    uint8_t buffer[BUFSIZ], digest[NUHASH_BYTES];
    char hexstr[NUHASH_CHARS];

    /* initialization */
    nuhash_init(&context);

    /* input data processing */
    while(more_data())
    {
        const size_t len = read_data(buffer, BUFSIZ);
        nuhash_update(&context, buffer, len);
    }

    /* finalization */
    nuhash_final(&context, digest);

    /* print the result */
    puts(nuhash_tohex(digest, 0, hexstr));
} 
```

If all input data is available at once, the *libnuhash* “simple” API can be used instead:

```c
#include <nuhash.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    /* state variables */
    uint8_t digest[NUHASH_BYTES];
    char hexstr[NUHASH_CHARS];

    /* compute hash */
    nuhash_compute(get_data(), length(), digest);

    /* print the result */
    puts(nuhash_tohex(digest, 0, hexstr));
} 
```


Command-line Usage
==================

The **`nuhash`** command-line tool provides an interface similar to [`sha256sum`](https://linux.die.net/man/1/sha256sum) and friends:

```
Usage:
  nuhash.exe [options] [<file_1> [... <file_n>]]

Options:
  -b --binary     Output digest as binary, default is hex-string.
  -h --help       Print the help screen and exit.
  -i --ignore     Ignore interrupt signals.
  -l --line-mode  Process the input file(s) as text, line by line.
  -p --progress   Print progress, while computing the hash.
  -s --stop       Stop on all errors, default is to keep going.
  -t --self-test  Run the built-in self-test.
  -u --uppercase  Print digest as uppercase, default is lowercase.
  -v --version    Print version information and exit.
```

### Example output:

```
abe9d2f9f55d62951540397875f1104d0374a7ce3ca9e21cfde6efd638b75765885f186477669b79d78d13b6d1abe0a2 foo
06573eaba880f77db3a9e6a085e3a4f8c1bf6f2997d6789bc38439d1f581e0907b21200aff43df37de27d241dc64d19e bar
e26b0338b7a3d662397cbc1ff9f81ac8885ee71f861f5897fc3a7b113ea314994f749caf7618a4bfc1ac90727ef64f94 baz
d113572666421a1e2cf6e17481f1f94b2b006e8c48504fea03e4b24d04930db4302368dc0e7bf44b104f7118bdd62f2d qux
```


Application Programming Interface (API)
=======================================

C99 API
-------

### Constants

The C99 API defines the following constants:

* **`NUHASH_WORDS`:** The size of the “raw” hash value (digest), in [`uint64_t`](https://cplusplus.com/reference/cstdint/) words.

* **`NUHASH_BYTES`:** The size of the “raw” hash value (digest), in bytes.

* **`NUHASH_CHARS`:** The size of the hash value (digest) encoded as a null-terminated “hex” string, in characters. This is twice the size of a “raw” hash value *plus* an additional slot for the terminating `\0` character.

### Simple

The ***simple*** API can be used to compute a hash value in an *“all at once”* fashion.

Use this API, if *all* input data is available at once. It can compute the hash value with as *single* function call.

#### `nuhash_compute()`

Compute the hash value from the given input data and return the resulting hash value (digest).

* Syntax:
  ```c
  uint8_t *nuhash_compute(const uint8_t *const src, const size_t len, uint8_t *const out);
  ```

* Parameters:
  - **`src`:** Pointer to the source array containing all input data to be processed (sequence of bytes).
  - **`len`:** The length of the input data, in bytes.
  - **`out`:** Pointer to the destination array where the hash value is stored, **must** be `NUHASH_BYTES` in size.

* Return value:
  - This function returns the given `out` pointer.

#### `nuhash_compute_with_key()`

Compute the hash value from the given input data and a user-defined key; returns the hash value (digest).

* Syntax:
  ```c
  uint8_t *nuhash_compute_with_key(const uint8_t *const key, const size_t key_len,
                      const uint8_t *const src, const size_t in_len, uint8_t *const out);
  ```

* Parameters:
  - **`key`:** Pointer to the source array containing a user-defined key (sequence of bytes).
  - **`key_len`:** The length of the user-defined key, in bytes.
  - **`src`:** Pointer to the source array containing all input data to be processed (sequence of bytes).
  - **`in_len`:** The length of the input data, in bytes.
  - **`out`:** Pointer to the destination array where the hash value is stored, **must** be `NUHASH_BYTES` in size.

* Return value:
  - This function returns the given `out` pointer.

### Stream API

The ***stream*** API can be used to compute a hash value in a *“step by step”* fashion.

Use this API, if **not** all input data is available at once. The input data can be processed in chunks of arbitrary size.

#### `nuhash_t`

An object containing the state of an ongoing hash computation.

The application **shall** treat this type as an *opaque* data structure; it is **not** meant to be accessed directly!

#### `nuhash_init()`

Initialize or reset the state for a new hash computation.

This function is supposed to be called exactly *once* at the beginning of a new hash computation!

* Syntax:
  ```c
  void nuhash_init(nuhash_t *const ctx);
  ```

* Parameters:
  - **`ctx`:** Pointer to the `nuhash_t` instance to be initialized.

#### `nuhash_init_with_key()`

Initialize or reset the state for a new hash computation with a user-defined key.

This function is supposed to be called exactly *once* at the beginning of a new hash computation!

* Syntax:
  ```c
  void nuhash_init_with_key(nuhash_t *const ctx, const uint8_t *const key, const size_t len);
  ```

* Parameters:
  - **`ctx`:** Pointer to the `nuhash_t` instance to be initialized.
  - **`key`:** Pointer to the source array containing the user-defined key (sequence of bytes).
  - **`len`:** The length of the user-defined key, in bytes.

#### `nuhash_update()`

Process the next chunk of input data and update the state.

This function is supposed to be called *repeatedly* until all the input data has been processed!

* Syntax:
  ```c
  void nuhash_update(nuhash_t *const ctx, const uint8_t *const src, const size_t len);
  ```

* Parameters:
  - **`ctx`:** Pointer to the `nuhash_t` instance to be updated.
  - **`src`:** Pointer to the source array containing the next chunk of input data (sequence of bytes).
  - **`len`:** The length of the input data, in bytes.

#### `nuhash_final()`

Finish the hash computation and return the resulting hash value (digest).

This function is supposed to be called exactly *once* after all the input data has been processed! After calling this function, the `nuhash_t` instance is **invalidated**; it must be [*reset*](#nuhash_init) in order to start a new hash computation.

* Syntax:
  ```c
  uint8_t *nuhash_final(nuhash_t *const ctx, uint8_t *const out);
  ```

* Parameters:
  - **`ctx`:** Pointer to the `nuhash_t` instance to be updated.
  - **`out`:** Pointer to the destination array where the hash value is stored, **must** be `NUHASH_BYTES` in size.

* Return value:
  - This function returns the given `out` pointer.

### Utilities

The C99 API also provides a few additional utility functions that may be helpful.

#### `nuhash_tohex()`

Converts a “raw” hash value (digest) into a null-terminated “hex” string.

* Syntax:
  ```c
  char *nuhash_tohex(const uint8_t *const hash, const int upper, char *const out);
  ```

* Parameters:
  - **`hash`:** Pointer to the source array containing the “raw” hash value, **must** be `NUHASH_BYTES` in size.
  - **`upper`:** If non-zero, generate upper-case characters; otherwise generate lower-case characters.
  - **`out`:** Pointer to the destination array where the “hex” string is stored, **must** be `NUHASH_CHARS` in size.

* Return value:
  - This function returns the given `out` pointer.

#### `nuhash_version()`

Returns the version number and the build date of the current *libnuhash* library.

* Syntax:
  ```c
  char *nuhash_version(uint16_t version[3U], char *const build);
  ```

* Parameters:
  - **`version`:** Pointer to the destination array where the *major*, *minor* and *patch* version are stored.
  - **`build`:** Pointer to the destination array where the build date is stored, **must** be 12 characters in size.

* Return value:
  - This function returns the given `build` pointer.

### Thread-safety

All functions provided by ***libnuhash*** are *thread-safe* and *reentrant*, in the sense that there is **no** implicitly shared “global” state; the functions operate strictly on the given input parameters. Specifically, the functions of the “stream” API maintain **all** state of the hash computation within the given `nuhash_t` instance. This means that these functions are *thread-safe* and *reentrant* as long as each “concurrent” invocation uses its own separate `nuhash_t` instance; **no** synchronization is required in that case. However, *libnuhash* makes **no** attempt to coordinate the access to the *same* `nuhash_t` instance between concurrent threads. If the *same* `nuhash_t` instance needs to be accessed by *multiple* concurrent threads (which generally is **not** advised), then the application is responsible for *serializing* all access to that “shared” instance, e.g. by using a [*mutex*](https://pubs.opengroup.org/onlinepubs/009604499/functions/pthread_mutex_lock.html) lock! Reentrancy can **not** be achieved with a single “shared” `nuhash_t`.



Source Code
===========

The source code is available from these [Git](https://git-scm.com/) mirrors:

* `git clone https://github.com/lordmulder/NuHash.git` ([Browse](https://github.com/lordmulder/NuHash))
* `git clone https://bitbucket.org/muldersoft/nuhash.git` ([Browse](https://bitbucket.org/muldersoft/nuhash/))
* `git clone https://gitlab.com/lord_mulder/nuhash.git` ([Browse](https://gitlab.com/lord_mulder/nuhash))
* `git clone https://repo.or.cz/nuhash.git` ([Browse](https://repo.or.cz/nuhash.git))
* `git clone https://punkindrublic.mooo.com:3000/Muldersoft/NuHash.git` ([Browse](https://punkindrublic.mooo.com:3000/Muldersoft/NuHash))


License
=======

This work has been released under the **CC0 1.0 Universal** license.

For details, please refer to:  
<https://creativecommons.org/publicdomain/zero/1.0/legalcode>

Acknowledgment
--------------

Licenses for third-party components used by NuHash:

* [**Hash icons**](https://www.flaticon.com/free-icons/hash)  
  Created by Vectors Market &ndash; Flaticon. Free for personal and commercial use with attribution.

&marker;
