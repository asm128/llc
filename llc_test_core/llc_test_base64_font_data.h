#ifndef LLC_TEST_BASE64_FONT_DATA_H
#define LLC_TEST_BASE64_FONT_DATA_H

struct SBase64FontFixture {
    ::llc::u0_t        Width;
    ::llc::u0_t        Height;
    ::llc::u2_t        DecodedCount;
    ::llc::u2_t        DecodedHash;
    ::llc::u2_t        LetterHash;
};

stxp SBase64FontFixture BASE64_FONT_FIXTURES[] =
    { { 4, 6, 772, 0xA06ACAE4U, 0x627FA0DAU}
    , { 5, 12, 1924, 0x1B9768FEU, 0xC143B68CU}
    , { 6, 8, 1540, 0xE93FC360U, 0x817070A7U}
    , { 7, 12, 2692, 0x39497F6FU, 0xA9265D04U}
    , { 8, 8, 2052, 0x2E063E6AU, 0xE4C4B362U}
    , { 8, 8, 2052, 0xF243DFC0U, 0xD5E4930AU}
    , { 8, 12, 3076, 0x0A6E7809U, 0x3058C50CU}
    , { 8, 16, 4100, 0xC2DF0332U, 0xF565A9BBU}
    , { 9, 16, 4612, 0x96D1A365U, 0x1F2BD637U}
    , { 10, 18, 5764, 0xF6CE2880U, 0xC9B55782U}
    , { 12, 16, 6148, 0x32FD7683U, 0x90E1163FU}
    , { 16, 8, 4100, 0x2B9C34E6U, 0x7E5AAD2CU}
    , { 16, 12, 6148, 0x4FBAA943U, 0x36327099U}
    };

#endif // LLC_TEST_BASE64_FONT_DATA_H
