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
#include <bitcoin/bitcoin/coinninja/wallet/data_signing.hpp>
#include <test/test_helpers.hpp>

using namespace bc;
using namespace bc::wallet;
using namespace coinninja::wallet;

BOOST_AUTO_TEST_SUITE(coinninja__data_signing)

BOOST_AUTO_TEST_CASE(data_signing__test_signing)
{
    const base_coin coin{};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    const std::string str{"Hello World"};
    const auto data = std::vector<uint8_t>(str.begin(), str.end());
    const std::string expected_signature_string{"3045022100c515fc2ed70810f6b1383cfe8e81b9b41b08682511e92d557f1b1719391b521d02200d9d734fd09ce60586ac48b0a7eb587a50958cd9fa548ffa39088fc6ada12eec"};
    std::string signature_string{data_signing::signature_signing_data(data, master_private_key)};

    BOOST_REQUIRE_EQUAL(signature_string, expected_signature_string);

    bc::data_chunk sig_chunk;
    bc::decode_base16(sig_chunk, signature_string);
    bool verified = coinninja::wallet::data_signing::verify_signed_data(data, sig_chunk, master_private_key);
    BOOST_REQUIRE(verified);

    const std::string bad_str{"dlroW olleH"};
    const auto bad_data = std::vector<uint8_t>(bad_str.begin(), bad_str.end());
    bool verified2 = coinninja::wallet::data_signing::verify_signed_data(bad_data, sig_chunk, master_private_key);
    BOOST_REQUIRE(!verified2);
}

BOOST_AUTO_TEST_SUITE_END()