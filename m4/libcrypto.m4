dnl Checks for libcrypto required headers and functions
dnl
dnl Version: 20180727

dnl Function to detect whether openssl/evp.h can be used in combination with zlib.h
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE],
  [AC_CACHE_CHECK(
    [if openssl/evp.h can be used in combination with zlib.h],
    [ac_cv_openssl_evp_zlib_compatible],
    [AC_LANG_PUSH(C)
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <zlib.h>
#include <openssl/evp.h>]],
        [[ ]] )],
      [ac_cv_openssl_evp_zlib_compatible=yes],
      [ac_cv_openssl_evp_zlib_compatible=no])
    AC_LANG_POP(C)])
  ])

dnl Function to detect if libcrypto (openssl) EVP functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP],
  [AC_CHECK_HEADERS([openssl/evp.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_evp_h" = xno],
    [ac_cv_libcrypto=no
    ac_cv_libcrypto_evp=no],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE

    AS_IF(
      [test "x$ac_cv_openssl_evp_zlib_compatible" = xyes],
      [ac_cv_libcrypto=yes
      ac_cv_libcrypto_evp=yes],
      [ac_cv_libcrypto=no
      ac_cv_libcrypto_evp=no])
    ])
  ])

dnl Function to detect if libcrypto (openssl) EVP MD functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD],
  [ac_cv_libcrypto_evp_md=yes

  AC_CHECK_LIB(
    crypto,
    EVP_MD_CTX_new,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_dummy=no])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_new" = xyes],
    [dnl Check for the new EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_free,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    ],
    [dnl Check for the old EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_cleanup,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])

    AC_CHECK_LIB(
      crypto,
      ERR_remove_thread_state,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    ])

  AC_CHECK_LIB(
    crypto,
    EVP_DigestInit_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])
  AC_CHECK_LIB(
    crypto,
    EVP_DigestUpdate,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])
  AC_CHECK_LIB(
    crypto,
    EVP_DigestFinal_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])

  AS_IF(
    [test "x$ac_cv_enable_openssl_evp_md" = xyes && "x$ac_cv_libcrypto_evp_md" = xno],
    [AC_MSG_FAILURE(
      [Missing OpenSSL EVP MD support],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_init" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_MD_CTX_INIT],
      [1],
      [Define to 1 if you have the `EVP_MD_CTX_init' function".])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_cleanup" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_MD_CTX_CLEANUP],
      [1],
      [Define to 1 if you have the `EVP_MD_CTX_cleanup' function".])
    ])
  ])

dnl Function to detect if openssl EVP MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD5],
  [AC_CHECK_LIB(
    crypto,
    EVP_md5,
    [ac_cv_libcrypto_md5=libcrypto_evp],
    [ac_cv_libcrypto_md5=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_md5" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_MD5],
      [1],
      [Define to 1 if you have the `EVP_md5' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA1],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha1,
    [ac_cv_libcrypto_sha1=libcrypto_evp],
    [ac_cv_libcrypto_sha1=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha1" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA1],
      [1],
      [Define to 1 if you have the `EVP_sha1' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA224],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha224,
    [ac_cv_libcrypto_sha224=libcrypto_evp],
    [ac_cv_libcrypto_sha224=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha224" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA224],
      [1],
      [Define to 1 if you have the `EVP_sha224' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA256],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha256,
    [ac_cv_libcrypto_sha256=libcrypto_evp],
    [ac_cv_libcrypto_sha256=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha256" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA256],
      [1],
      [Define to 1 if you have the `EVP_sha256' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA512],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha512,
    [ac_cv_libcrypto_sha512=libcrypto_evp],
    [ac_cv_libcrypto_sha512=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha512" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA512],
      [1],
      [Define to 1 if you have the `EVP_sha512' function".])
    ])
  ])

dnl Function to detect if openssl MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_MD5],
  [AC_CHECK_HEADERS([openssl/md5.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_md5_h" = xno],
    [ac_cv_libcrypto_md5=no],
    [ac_cv_libcrypto_md5=libcrypto

    AC_CHECK_LIB(
      crypto,
      MD5_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    AC_CHECK_LIB(
      crypto,
      MD5_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    AC_CHECK_LIB(
      crypto,
      MD5_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_md5" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA1],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha1=no],
    [ac_cv_libcrypto_sha1=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA1_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    AC_CHECK_LIB(
      crypto,
      SHA1_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    AC_CHECK_LIB(
      crypto,
      SHA1_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha1" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA224],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha224=no],
    [ac_cv_libcrypto_sha224=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA224_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    AC_CHECK_LIB(
      crypto,
      SHA224_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    AC_CHECK_LIB(
      crypto,
      SHA224_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha224" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA256],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha256=no],
    [ac_cv_libcrypto_sha256=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA256_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    AC_CHECK_LIB(
      crypto,
      SHA256_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    AC_CHECK_LIB(
      crypto,
      SHA256_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha256" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA512],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha512=no],
    [ac_cv_libcrypto_sha512=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA512_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    AC_CHECK_LIB(
      crypto,
      SHA512_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    AC_CHECK_LIB(
      crypto,
      SHA512_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha512" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl EVP AES functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER],
  [ac_cv_libcrypto_evp_cipher=yes

  dnl Check if all required libcrypto (openssl) EVP CIPHER functions are available
  AC_CHECK_LIB(
    crypto,
    EVP_CIPHER_CTX_new,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_dummy=no])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_new" = xyes],
    [dnl Check for the new EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_free,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    ],
    [dnl Check for the old EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_cleanup,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])

    AC_CHECK_LIB(
      crypto,
      ERR_remove_thread_state,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    ])

  AC_CHECK_LIB(
    crypto,
    EVP_CIPHER_CTX_set_padding,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])

  AC_CHECK_LIB(
    crypto,
    EVP_CipherInit_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])
  AC_CHECK_LIB(
    crypto,
    EVP_CipherUpdate,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])
  AC_CHECK_LIB(
    crypto,
    EVP_CipherFinal_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])

  AS_IF(
    [test "x$ac_cv_enable_openssl_evp_cipher" = xyes && "x$ac_cv_libcrypto_evp_cipher" = xno],
    [AC_MSG_FAILURE(
      [Missing OpenSSL EVP CIPHER support],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_init" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_CIPHER_CTX_INIT],
      [1],
      [Define to 1 if you have the `EVP_CIPHER_CTX_init' function".])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_cleanup" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_CIPHER_CTX_CLEANUP],
      [1],
      [Define to 1 if you have the `EVP_CIPHER_CTX_cleanup' function".])
    ])
  ])

dnl Function to detect if openssl AES functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_AES],
  [AC_CHECK_HEADERS([openssl/aes.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_aes_h" = xno],
    [ac_cv_libcrypto_aes=no],
    [ac_cv_libcrypto_aes=libcrypto
    AC_CHECK_LIB(
      crypto,
      EVP_aes_128_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_192_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_256_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_aes" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if libcrypto (openssl) dependencies are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_LIB],
  [dnl Check if parameters were provided
  AS_IF(
    [test "x$ac_cv_with_openssl" != x && test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_with_openssl" != xauto-detect],
    [AS_IF(
      [test -d "$ac_cv_with_openssl"],
      [CFLAGS="$CFLAGS -I${ac_cv_with_openssl}/include"
      LDFLAGS="$LDFLAGS -L${ac_cv_with_openssl}/lib"],
      [AC_MSG_WARN([no such directory: $ac_cv_with_openssl])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_with_openssl" = xno],
    [ac_cv_libcrypto=no],
    [dnl Check for a pkg-config file
    AS_IF(
      [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
      [PKG_CHECK_MODULES(
        [openssl],
        [openssl >= 1.0],
        [ac_cv_libcrypto=yes
        ac_cv_libcrypto_evp=yes],
        [ac_cv_libcrypto=no
        ac_cv_libcrypto_evp=no])
      ])

    AS_IF(
      [test "x$ac_cv_libcrypto" = xyes],
      [ac_cv_libcrypto_CPPFLAGS="$pkg_cv_openssl_CFLAGS"
      ac_cv_libcrypto_LIBADD="$pkg_cv_openssl_LIBS"],
      [dnl Check for headers
      AC_CHECK_HEADERS([openssl/opensslv.h])

      AX_LIBCRYPTO_CHECK_OPENSSL_EVP

      AS_IF(
        [test "x$ac_cv_libcrypto_evp" != xyes && test "$ac_cv_header_openssl_opensslv" = xyes],
        [ac_cv_libcrypto=yes])
      ])

    dnl Setup libcrypto (openssl) parameters
    AS_IF(
      [test "x$ac_cv_libcrypto" = xyes && test "x$ac_cv_libcrypto_evp" = xyes],
      [AC_DEFINE(
        [HAVE_OPENSSL_EVP_H],
        [1],
        [Define to 1 if you have the <openssl/evp.h> header file.])
      AC_SUBST(
        [HAVE_OPENSSL_EVP_H],
        [1]) ],
      [AC_SUBST(
        [HAVE_OPENSSL_EVP_H],
        [0])
      ])

    AS_IF(
      [test "x$ac_cv_libcrypto" != xno],
      [AC_DEFINE(
        [HAVE_LIBCRYPTO],
        [1],
        [Define to 1 if you have the 'crypto' library (-lcrypto).])

      ac_cv_libcrypto_LIBADD="-lcrypto"

      dnl Enforce the dynamic loader library to be included if available
      AC_CHECK_LIB(
        dl,
        dlopen,
        [AC_SUBST(
          [LIBDL_LIBADD],
          ["-ldl"])],
        [])
      ])
    ])
  ])

dnl Function to detect if libcrypto MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_MD5],
  [ac_cv_libcrypto_md5=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP MD5 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD5])

  dnl Check for libcrypto (openssl) MD5 support
  AS_IF(
    [test "x$ac_cv_libcrypto_md5" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_MD5])
  ])

dnl Function to detect if libcrypto SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA1],
  [ac_cv_libcrypto_sha1=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA1 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA1])

  dnl Check for libcrypto (openssl) SHA1 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha1" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA1])
  ])

dnl Function to detect if libcrypto SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA224],
  [ac_cv_libcrypto_sha224=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA224 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA224])

  dnl Check for libcrypto (openssl) SHA224 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha224" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA224])
  ])

dnl Function to detect if libcrypto SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA256],
  [ac_cv_libcrypto_sha256=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA256 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA256])

  dnl Check for libcrypto (openssl) SHA256 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha256" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA256])
  ])

dnl Function to detect if libcrypto SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA512],
  [ac_cv_libcrypto_sha512=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA512 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA512])

  dnl Check for libcrypto (openssl) SHA512 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha512" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA512])
  ])

dnl Function to detect if libcrypto AES functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_AES],
  [ac_cv_libcrypto_aes=no

  dnl Check for libcrypto (openssl) EVP CIPHER support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_cipher" != xno && test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER

    AS_IF(
      [test "x$ac_cv_libcrypto_evp_cipher" = xyes],
      [ac_cv_libcrypto_aes=libcrypto_evp])
    ])

  dnl Check for libcrypto (openssl) AES support
  AS_IF(
    [test "x$ac_cv_libcrypto_aes" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_AES])
  ])

dnl Function to detect how to enable libcrypto (openssl)
AC_DEFUN([AX_LIBCRYPTO_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [openssl],
    [openssl],
    [search for OpenSSL in includedir and libdir or in the specified DIR, or no if not to use OpenSSL],
    [auto-detect],
    [DIR])

  AX_COMMON_ARG_ENABLE(
    [openssl-evp-cipher],
    [openssl_evp_cipher],
    [enable OpenSSL EVP CIPHER support, or no to disable],
    [auto-detect])

  AX_COMMON_ARG_ENABLE(
    [openssl-evp-md],
    [openssl_evp_md],
    [enable OpenSSL EVP MD support, or no to disable],
    [auto-detect])

  dnl Check for a shared library version
  AX_LIBCRYPTO_CHECK_LIB

  AS_IF(
    [test "x$ac_cv_libcrypto_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCRYPTO_CPPFLAGS],
      [$ac_cv_libcrypto_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcrypto_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCRYPTO_LIBADD],
      [$ac_cv_libcrypto_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AC_SUBST(
      [ax_libcrypto_pc_libs_private],
      [-lcrypto])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AC_SUBST(
      [ax_libcrypto_spec_requires],
      [openssl])
    AC_SUBST(
      [ax_libcrypto_spec_build_requires],
      [openssl-devel])
    ])
  ])

