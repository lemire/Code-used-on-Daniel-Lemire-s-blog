#ifndef SSE_TYPE_H
#define SSE_TYPE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// classes
#define CLASS_IN (1u)
#define CLASS_CS (2u)
#define CLASS_CH (3u)
#define CLASS_HS (4u)

// types
#define TYPE_A (1)
#define TYPE_NS (2u)
#define TYPE_MD (3u)
#define TYPE_MF (4u)
#define TYPE_CNAME (5u)
#define TYPE_SOA (6u)
#define TYPE_MB (7u)
#define TYPE_MG (8u)
#define TYPE_MR (9u)
#define TYPE_NULL (10u)
#define TYPE_WKS (11u)
#define TYPE_PTR (12u)
#define TYPE_HINFO (13u)
#define TYPE_MINFO (14u)
#define TYPE_MX (15u)
#define TYPE_TXT (16u)
#define TYPE_RP (17u)
#define TYPE_AFSDB (18u)
#define TYPE_X25 (19u)
#define TYPE_ISDN (20u)
#define TYPE_RT (21u)
#define TYPE_NSAP (22u)
#define TYPE_NSAP_PTR (23u)
#define TYPE_SIG (24u)
#define TYPE_KEY (25u)
#define TYPE_PX (26u)
#define TYPE_GPOS (27u)
#define TYPE_AAAA (28u)
#define TYPE_LOC (29u)
#define TYPE_NXT (30u)
#define TYPE_SRV (33u)
#define TYPE_NAPTR (35u)
#define TYPE_KX (36u)
#define TYPE_CERT (37u)
#define TYPE_A6 (38u)
#define TYPE_DNAME (39u)
#define TYPE_APL (42u)
#define TYPE_DS (43u)
#define TYPE_SSHFP (44u)
#define TYPE_IPSECKEY (45u)
#define TYPE_RRSIG (46u)
#define TYPE_NSEC (47u)
#define TYPE_DNSKEY (48u)
#define TYPE_DHCID (49u)
#define TYPE_NSEC3 (50u)
#define TYPE_NSEC3PARAM (51u)
#define TYPE_TLSA (52u)
#define TYPE_SMIMEA (53u)
#define TYPE_HIP (55u)
#define TYPE_CDS (59u)
#define TYPE_CDNSKEY (60u)
#define TYPE_OPENPGPKEY (61u)
#define TYPE_CSYNC (62u)
#define TYPE_ZONEMD (63u)
#define TYPE_SVCB (64u)
#define TYPE_HTTPS (65u)
#define TYPE_SPF (99u)
#define TYPE_NID (104u)
#define TYPE_L32 (105u)
#define TYPE_L64 (106u)
#define TYPE_LP (107u)
#define TYPE_EUI48 (108u)
#define TYPE_EUI64 (109u)
#define TYPE_URI (256u)
#define TYPE_CAA (257u)
#define TYPE_AVC (258u)
#define TYPE_DLV (32769u)

bool sse_type(const char *type_string, uint16_t *type);
bool sse_upper_type(const char *type_string, uint16_t *type);
bool swar_upper_len(const char *str, size_t len, uint16_t *type);
bool sse_upper_len(const char *str, size_t len, uint16_t *type);
bool sse_table(const char *type_string, uint16_t *type);
size_t sse_length(const char *type_string);
#endif // SSE_TYPE_H
