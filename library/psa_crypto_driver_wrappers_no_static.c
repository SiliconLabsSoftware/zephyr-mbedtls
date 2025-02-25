/*
 *  Functions to delegate cryptographic operations to an available
 *  and appropriate accelerator.
 *  Warning: This file is now auto-generated.
 */
/*  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */


/* BEGIN-common headers */
#include "common.h"
#include "psa_crypto_aead.h"
#include "psa_crypto_cipher.h"
#include "psa_crypto_core.h"
#include "psa_crypto_driver_wrappers_no_static.h"
#include "psa_crypto_hash.h"
#include "psa_crypto_mac.h"
#include "psa_crypto_pake.h"
#include "psa_crypto_rsa.h"

#include "mbedtls/platform.h"
/* END-common headers */

#if defined(MBEDTLS_PSA_CRYPTO_C)

/* BEGIN-driver headers */
/* Headers for mbedtls_test opaque driver */
#if defined(PSA_CRYPTO_DRIVER_TEST)
#include "test/drivers/test_driver.h"

#endif
/* Headers for mbedtls_test transparent driver */
#if defined(PSA_CRYPTO_DRIVER_TEST)
#include "test/drivers/test_driver.h"

#endif
/* Headers for p256 transparent driver */
#if defined(MBEDTLS_PSA_P256M_DRIVER_ENABLED)
#include "../3rdparty/p256-m/p256-m_driver_entrypoints.h"

#endif

/* Include TF-M builtin key driver */
#if defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "tfm_builtin_key_loader.h"
#endif /* PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER */

#if defined(PSA_CRYPTO_DRIVER_CC3XX)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "cc3xx.h"
#endif /* PSA_CRYPTO_DRIVER_CC3XX */

#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE)
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_psa_driver_features.h"
#include "sli_se_transparent_types.h"
#include "sli_se_transparent_functions.h"
#if defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
#include "sli_se_opaque_types.h"
#include "sli_se_opaque_functions.h"
#endif /* SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS */
#endif /* PSA_CRYPTO_DRIVER_SILABS_HSE */

#if defined(PSA_CRYPTO_DRIVER_SILABS_VSE)
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_psa_driver_features.h"
#include "sli_cryptoacc_transparent_types.h"
#include "sli_cryptoacc_transparent_functions.h"
#if defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
#include "sli_cryptoacc_opaque_types.h"
#include "sli_cryptoacc_opaque_functions.h"
#endif /* SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS */
#endif /* PSA_CRYPTO_DRIVER_SILABS_VSE */

#if defined(PSA_CRYPTO_DRIVER_SILABS_SI91X)
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_psa_driver_features.h"
#include "sli_si91x_crypto_driver_functions.h"
#if defined(SLI_PSA_DRIVER_FEATURE_WRAPPED_KEYS)
#include "sl_si91x_psa_wrap.h"
#endif /* Secure key storage driver **/
#endif /* PSA_CRYPTO_DRIVER_SILABS_SI91X */

/* END-driver headers */

/* Auto-generated values depending on which drivers are registered.
 * ID 0 is reserved for unallocated operations.
 * ID 1 is reserved for the Mbed TLS software driver. */
/* BEGIN-driver id definition */
enum {
    PSA_CRYPTO_MBED_TLS_DRIVER_ID = 1,
    MBEDTLS_TEST_OPAQUE_DRIVER_ID,
    MBEDTLS_TEST_TRANSPARENT_DRIVER_ID,
    P256_TRANSPARENT_DRIVER_ID,
#if defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER)
    PSA_CRYPTO_TFM_BUILTIN_KEY_LOADER_DRIVER_ID,
#endif /* PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER */
#if defined(PSA_CRYPTO_DRIVER_CC3XX)
    PSA_CRYPTO_CC3XX_DRIVER_ID,
#endif /* PSA_CRYPTO_DRIVER_CC3XX */
#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE)
    PSA_CRYPTO_SILABS_HSE_TRANSPARENT_DRIVER_ID,
#if defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
    PSA_CRYPTO_SILABS_HSE_OPAQUE_DRIVER_ID,
#endif
#endif /* PSA_CRYPTO_DRIVER_SILABS_HSE */
#if defined(PSA_CRYPTO_DRIVER_SILABS_VSE)
    PSA_CRYPTO_SILABS_VSE_TRANSPARENT_DRIVER_ID,
#endif /* PSA_CRYPTO_DRIVER_SILABS_VSE */
#if defined(PSA_CRYPTO_DRIVER_SILABS_SI91X)
    PSA_CRYPTO_SILABS_SI91X_DRIVER_ID,
#endif /* PSA_CRYPTO_DRIVER_SILABS_SI91X */
};

/* END-driver id */

/* BEGIN-Common Macro definitions */

/* END-Common Macro definitions */

/* Support the 'old' SE interface when asked to */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
/* PSA_CRYPTO_DRIVER_PRESENT is defined when either a new-style or old-style
 * SE driver is present, to avoid unused argument errors at compile time. */
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#include "psa_crypto_se.h"
#endif

/** Get the key buffer size required to store the key material of a key
 *  associated with an opaque driver.
 *
 * \param[in] attributes  The key attributes.
 * \param[out] key_buffer_size  Minimum buffer size to contain the key material
 *
 * \retval #PSA_SUCCESS
 *         The minimum size for a buffer to contain the key material has been
 *         returned successfully.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         The type and/or the size in bits of the key or the combination of
 *         the two is not supported.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The key is declared with a lifetime not known to us.
 */
psa_status_t psa_driver_wrapper_get_key_buffer_size(
    const psa_key_attributes_t *attributes,
    size_t *key_buffer_size )
{
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION( psa_get_key_lifetime(attributes) );
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);
    size_t buffer_size = 0;
    MBEDTLS_MAYBE_UNUSED size_t input_buffer_size = *key_buffer_size;

    *key_buffer_size = 0;
    switch( location )
    {
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
#if defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
            /* Emulate property 'builtin_key_size' */
            if( psa_key_id_is_builtin(
                    MBEDTLS_SVC_KEY_ID_GET_KEY_ID(
                        psa_get_key_id( attributes ) ) ) )
            {
                *key_buffer_size = sizeof( psa_drv_slot_number_t );
                return( PSA_SUCCESS );
            }
#endif /* MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS */
            *key_buffer_size = mbedtls_test_opaque_size_function( key_type,
                                                                  key_bits );
            return( ( *key_buffer_size != 0 ) ?
                    PSA_SUCCESS : PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_TEST */

#if defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER)
        case TFM_BUILTIN_KEY_LOADER_KEY_LOCATION:
            return tfm_builtin_key_loader_get_key_buffer_size(psa_get_key_id(attributes),
                                                              key_buffer_size);
#endif /* PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER */

        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE( key_type, key_bits );
            if( buffer_size == 0 ||
                ( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) && buffer_size == 1 ) )
                return( PSA_ERROR_NOT_SUPPORTED );
            *key_buffer_size = buffer_size;
            return( PSA_SUCCESS );

#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE) && defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE( key_type, key_bits );
            if( buffer_size == 0 ||
                ( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) && buffer_size == 1 ) )
                buffer_size = input_buffer_size;
            // Remove public key format byte
            if( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY( key_type ) ) {
                buffer_size--;
            }
            // Compensate for word alignment demands
            buffer_size = sli_se_word_align( buffer_size );
            if( PSA_BITS_TO_BYTES( key_bits ) & 0x3 || PSA_BITS_TO_BYTES( key_bits ) == 0 ) {
                if( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) ) {
                    // Allocate extra word for public keys, since alignment constrains
                    // May require that
                    buffer_size += sizeof(uint32_t);
                }
            }
            // Add wrapped context overhead
            buffer_size += sizeof(sli_se_opaque_wrapped_key_context_t);
            *key_buffer_size = buffer_size;
            return ( PSA_SUCCESS );
#endif

#if defined(PSA_CRYPTO_DRIVER_SILABS_VSE) && defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
        case PSA_KEY_LOCATION_SL_CRYPTOACC_OPAQUE:
            buffer_size = sizeof(sli_cryptoacc_opaque_key_context_t);
            *key_buffer_size = buffer_size;
            return ( PSA_SUCCESS );
#endif

#if defined(PSA_CRYPTO_CRIVER_SILABS_SI91X) && defined(SLI_PSA_DRIVER_FEATURE_WRAPPED_KEYS)
        case PSA_KEY_VOLATILE_PERSISTENT_WRAPPED:
          buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE( key_type, key_bits );
          if( buffer_size == 0 ||
              ( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) && buffer_size == 1 ) )
              return( PSA_ERROR_NOT_SUPPORTED );
          *key_buffer_size = buffer_size;
          return( PSA_SUCCESS );
          break;
#endif

        default:
            (void)key_type;
            (void)key_bits;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size, size_t *data_length )

{

    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(
                                      psa_get_key_lifetime( attributes ) );

    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( psa_get_key_lifetime(attributes), &drv, &drv_context ) )
    {
        if( ( drv->key_management == NULL ) ||
            ( drv->key_management->p_export_public == NULL ) )
        {
            return( PSA_ERROR_NOT_SUPPORTED );
        }

        return( drv->key_management->p_export_public(
                    drv_context,
                    *( (psa_key_slot_number_t *)key_buffer ),
                    data, data_size, data_length ) );
    }
#endif /* MBEDTLS_PSA_CRYPTO_SE_C */

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
#if defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER)
        case TFM_BUILTIN_KEY_LOADER_KEY_LOCATION:
#endif /* defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER) */
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)

#if (defined(PSA_CRYPTO_DRIVER_TEST) )
            status = mbedtls_test_transparent_export_public_key
                (attributes,
                                key_buffer,
                                key_buffer_size,
                                data,
                                data_size,
                                data_length
            );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif

#if defined(PSA_CRYPTO_DRIVER_CC3XX)
            status = cc3xx_export_public_key(
                         attributes,
                         key_buffer,
                         key_buffer_size,
                         data,
                         data_size,
                         data_length );
            return( status );
#endif /* PSA_CRYPTO_DRIVER_CC3XX */

#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE)
            status = sli_se_transparent_export_public_key( attributes,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           data,
                                                           data_size,
                                                           data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif // PSA_CRYPTO_DRIVER_SILABS_HSE

#if defined(PSA_CRYPTO_DRIVER_SILABS_VSE)
            status = sli_cryptoacc_transparent_export_public_key( attributes,
                                                                  key_buffer,
                                                                  key_buffer_size,
                                                                  data,
                                                                  data_size,
                                                                  data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif // PSA_CRYPTO_DRIVER_SILABS_VSE

#if defined(PSA_CRYPTO_DRIVER_SILABS_SI91X) && defined(SLI_PSA_DRIVER_FEATURE_ECDH) 
            status = sli_si91x_psa_export_public_key_ecdh( attributes,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           data,
                                                           data_size,
                                                           data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif // PSA_CRYPTO_DRIVER_SILABS_SI91X

#if (defined(MBEDTLS_PSA_P256M_DRIVER_ENABLED) )
            status = p256_transparent_export_public_key
                (attributes,
                                key_buffer,
                                key_buffer_size,
                                data,
                                data_size,
                                data_length
            );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif


#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            /* Fell through, meaning no accelerator supports this operation */
            return( psa_export_public_key_internal( attributes,
                                                    key_buffer,
                                                    key_buffer_size,
                                                    data,
                                                    data_size,
                                                    data_length ) );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)

#if (defined(PSA_CRYPTO_DRIVER_TEST) )
        case 0x7fffff:
            return( mbedtls_test_opaque_export_public_key
            (attributes,
                            key_buffer,
                            key_buffer_size,
                            data,
                            data_size,
                            data_length
        ));
#endif

#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE) && defined(SLI_PSA_DRIVER_FEATURE_OPAQUE_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return( sli_se_opaque_export_public_key( attributes,
                                                     key_buffer,
                                                     key_buffer_size,
                                                     data,
                                                     data_size,
                                                     data_length ) );
#endif

#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }

}

psa_status_t psa_driver_wrapper_get_builtin_key(
    psa_drv_slot_number_t slot_number,
    psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length )
{

    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION( psa_get_key_lifetime(attributes) );
    switch( location )
    {
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)

#if (defined(PSA_CRYPTO_DRIVER_TEST) )
        case 0x7fffff:
            return( mbedtls_test_opaque_get_builtin_key
            (slot_number,
                            attributes,
                            key_buffer,
                            key_buffer_size,
                            key_buffer_length
        ));
#endif

#if defined(PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER)
        case TFM_BUILTIN_KEY_LOADER_KEY_LOCATION:
            return( tfm_builtin_key_loader_get_builtin_key(
                        slot_number,
                        attributes,
                        key_buffer, key_buffer_size, key_buffer_length ) );
#endif /* PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER */

#if defined(PSA_CRYPTO_DRIVER_SILABS_HSE) && defined(SLI_PSA_DRIVER_FEATURE_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return( sli_se_opaque_get_builtin_key(
                        slot_number,
                        attributes,
                        key_buffer, key_buffer_size, key_buffer_length ) );
#endif

#if defined(PSA_CRYPTO_DRIVER_SILABS_VSE) && defined(SLI_PSA_DRIVER_FEATURE_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SL_CRYPTOACC_OPAQUE:
            return( sli_cryptoacc_opaque_get_builtin_key(
                        slot_number,
                        attributes,
                        key_buffer, key_buffer_size, key_buffer_length ) );
#endif

#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            (void) slot_number;
            (void) key_buffer;
            (void) key_buffer_size;
            (void) key_buffer_length;
            return( PSA_ERROR_DOES_NOT_EXIST );
    }

}

#endif /* MBEDTLS_PSA_CRYPTO_C */
