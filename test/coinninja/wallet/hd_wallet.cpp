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

// #include <test/wallet/mnemonic.hpp>

#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/hd_wallet.hpp>

using namespace bc;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(cn__hd_wallet__tests)

BOOST_AUTO_TEST_CASE(hd_wallet__all_bip_39_words)
{
    std::vector<std::string> words = coinninja::wallet::hd_wallet::all_bip_39_words();
    BOOST_REQUIRE_EQUAL(words.size(), 2048);
    BOOST_REQUIRE_NE(words.at(0), words.at(1));
}

BOOST_AUTO_TEST_CASE(hd_wallet__init_with_entropy__creates_mnemonic)
{
    bc::data_chunk entropy(16);
    pseudo_random_fill(entropy);
    coinninja::wallet::base_coin coin{};
    coinninja::wallet::hd_wallet hd_wallet{coin, entropy};
    std::vector<std::string> words = hd_wallet.get_current_words();
    BOOST_REQUIRE_EQUAL(words.size(), 12);
}

BOOST_AUTO_TEST_SUITE_END()