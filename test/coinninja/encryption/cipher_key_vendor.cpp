/**
 * Copyright (c) 2019 CoinNinja developers
 *
 * This file is added on to libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/encryption/cipher_key_vendor.hpp>
#include <bitcoin/bitcoin/coinninja/encryption/encryption_cipher_keys.hpp>
#include <bitcoin/bitcoin/coinninja/encryption/cipher_keys.hpp>
#include <vector>

using namespace coinninja::encryption;

BOOST_AUTO_TEST_SUITE(coinninja_encryption_keys)

BOOST_AUTO_TEST_CASE(encryption_cipher_keys)
{
    const std::string uncompressed_public_key{"04904240a0aaec6af6f9b6c331f71feea2a4ed1549c06e5a6409fe92c5824dc4c54e26c2b2e27cfc224a6b782b35a2872b666f568cf37456262fbb065601b4d73a"};
    bc::data_chunk key_data;
    bc::decode_base16(key_data, uncompressed_public_key);

    bc::data_chunk entropy1(16);
    bc::pseudo_random_fill(entropy1);
    bc::data_chunk entropy2(16);
    bc::pseudo_random_fill(entropy2);

    auto keys1{cipher_key_vendor::encryption_cipher_keys_for_uncompressed_public_key(key_data, entropy1)};
    auto keys2{cipher_key_vendor::encryption_cipher_keys_for_uncompressed_public_key(key_data, entropy2)};

    const auto enc1{keys1.get_encryption_key()};
    const auto enc2{keys2.get_encryption_key()};
    BOOST_REQUIRE_NE(bc::encode_base16(enc1), bc::encode_base16(enc2));

    const auto hmac1{keys1.get_hmac_key()};
    const auto hmac2{keys2.get_hmac_key()};
    BOOST_REQUIRE_NE(bc::encode_base16(hmac1), bc::encode_base16(hmac2));
}

BOOST_AUTO_TEST_SUITE_END()
