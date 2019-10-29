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
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <vector>

BOOST_AUTO_TEST_SUITE(cn__base_coin__tests)

BOOST_AUTO_TEST_CASE(base_coin__default_initialization)
{
    coinninja::wallet::base_coin coin{};
    BOOST_REQUIRE_EQUAL(coin.get_purpose(), coinninja::wallet::coin_derivation_purpose::BIP49);
    BOOST_REQUIRE_EQUAL(coin.get_coin(), coinninja::wallet::coin_derivation_coin::MainNet);
    BOOST_REQUIRE_EQUAL(coin.get_account(), 0);
}

BOOST_AUTO_TEST_CASE(base_coin__default_initialization__providing_purpose)
{
    coinninja::wallet::base_coin coin{coinninja::wallet::coin_derivation_purpose::BIP84};
    BOOST_REQUIRE_EQUAL(coin.get_purpose(), coinninja::wallet::coin_derivation_purpose::BIP84);
    BOOST_REQUIRE_EQUAL(coin.get_coin(), coinninja::wallet::coin_derivation_coin::MainNet);
    BOOST_REQUIRE_EQUAL(coin.get_account(), 0);
}

BOOST_AUTO_TEST_CASE(base_coin__default_initialization__providing_purpose_and_coin)
{
    coinninja::wallet::base_coin coin{coinninja::wallet::coin_derivation_purpose::BIP84, coinninja::wallet::coin_derivation_coin::TestNet};
    BOOST_REQUIRE_EQUAL(coin.get_purpose(), coinninja::wallet::coin_derivation_purpose::BIP84);
    BOOST_REQUIRE_EQUAL(coin.get_coin(), coinninja::wallet::coin_derivation_coin::TestNet);
    BOOST_REQUIRE_EQUAL(coin.get_account(), 0);
}

BOOST_AUTO_TEST_CASE(base_coin__initialization__providing_purpose_coin_and_account)
{
    coinninja::wallet::base_coin coin{coinninja::wallet::coin_derivation_purpose::BIP84, coinninja::wallet::coin_derivation_coin::TestNet, 1};
    BOOST_REQUIRE_EQUAL(coin.get_purpose(), coinninja::wallet::coin_derivation_purpose::BIP84);
    BOOST_REQUIRE_EQUAL(coin.get_coin(), coinninja::wallet::coin_derivation_coin::TestNet);
    BOOST_REQUIRE_EQUAL(coin.get_account(), 1);
}

BOOST_AUTO_TEST_CASE(base_coin__get_hrp_from_bip84_mainnet__is_valid)
{
    coinninja::wallet::base_coin coin{coinninja::wallet::coin_derivation_purpose::BIP84};
    BOOST_REQUIRE_EQUAL(coin.get_bech32_hrp(), "bc");
}

BOOST_AUTO_TEST_CASE(base_coin__get_hrp_from_bip84_testnet__is_valid)
{
    coinninja::wallet::base_coin coin{coinninja::wallet::coin_derivation_purpose::BIP84, coinninja::wallet::coin_derivation_coin::TestNet};
    BOOST_REQUIRE_EQUAL(coin.get_bech32_hrp(), "bcrt");
}

BOOST_AUTO_TEST_CASE(base_coin__get_hrp_from_non_bip84__is_invalid)
{
    std::vector<coinninja::wallet::coin_derivation_purpose> purposes = {
        coinninja::wallet::coin_derivation_purpose::BIP32,
        coinninja::wallet::coin_derivation_purpose::BIP39,
        coinninja::wallet::coin_derivation_purpose::BIP44,
        coinninja::wallet::coin_derivation_purpose::BIP49,
    };
    for (auto purpose : purposes)
    {
        coinninja::wallet::base_coin coin{purpose};
        BOOST_REQUIRE_EQUAL(coin.get_bech32_hrp(), "");
    }
}

BOOST_AUTO_TEST_SUITE_END()
