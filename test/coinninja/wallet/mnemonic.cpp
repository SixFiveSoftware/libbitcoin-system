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

#include <test/wallet/mnemonic.hpp>

#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/mnemonic.hpp>

using namespace bc;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(cn_mnemonic_tests)

BOOST_AUTO_TEST_CASE(mnemonic__create_mnemonic__trezor)
{
    for (const mnemonic_result& vector: mnemonic_trezor_vectors)
    {
        data_chunk entropy;
        decode_base16(entropy, vector.entropy);
        const auto mnemonic = coinninja::wallet::create_mnemonic(entropy, vector.language);
        BOOST_REQUIRE(mnemonic.size() > 0);
        BOOST_REQUIRE_EQUAL(join(mnemonic, ","), vector.mnemonic);
        BOOST_REQUIRE(validate_mnemonic(mnemonic));
    }
}

BOOST_AUTO_TEST_CASE(mnemonic__create_mnemonic__bx)
{
    for (const mnemonic_result& vector: mnemonic_bx_new_vectors)
    {
        data_chunk entropy;
        decode_base16(entropy, vector.entropy);
        const auto mnemonic = coinninja::wallet::create_mnemonic(entropy, vector.language);
        BOOST_REQUIRE(mnemonic.size() > 0);
        BOOST_REQUIRE_EQUAL(join(mnemonic, ","), vector.mnemonic);
        BOOST_REQUIRE(validate_mnemonic(mnemonic));
    }
}

BOOST_AUTO_TEST_CASE(mnemonic__all_bip_39_words)
{
    std::vector<std::string> words = coinninja::wallet::all_bip_39_words();
    BOOST_REQUIRE_EQUAL(words.size(), 2048);
    BOOST_REQUIRE_NE(words.at(0), words.at(1));
}

BOOST_AUTO_TEST_CASE(mnemonic__init_with_entropy__creates_mnemonic)
{
    bc::data_chunk entropy(16);
    pseudo_random_fill(entropy);
    const auto mnemonic = coinninja::wallet::create_mnemonic(entropy);
    BOOST_REQUIRE_EQUAL(mnemonic.size(), 12);
}

BOOST_AUTO_TEST_CASE(mnemonic__init_with_bad_entropy__returns_empty_word_list)
{
    const int len{15};
    bc::data_chunk entropy(len);
    for (size_t i{0}; i < len; i++) {
        entropy.at(i) = 0xFF;
    }
    const auto mnemonic = coinninja::wallet::create_mnemonic(entropy);
    BOOST_REQUIRE_EQUAL(mnemonic.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()