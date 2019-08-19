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
#include <bitcoin/bitcoin/coinninja/wallet/key_factory.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <test/test_helpers.hpp>

using namespace bc;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(cn__key_factory__tests)

BOOST_AUTO_TEST_CASE(signing_key__returns_private_key)
{
    const coinninja::wallet::base_coin coin{};
    const auto master_private_key{private_key_for(test_only_words, coin)};
    const auto signing_key{coinninja::wallet::key_factory::signing_key(master_private_key)};
    const auto encoded{signing_key.encoded()};

    const std::string expected_encoded{"xprv9ukW2UsmeQPBDWViiTd81WaQHCm66iHgkoMtBeLEqnPyBhs5tvP54ee4FNRacQcCLkHtgxQx7BobhU3vpmCctRwXu8YLdnEZ2y4L32VCAmN"};

    BOOST_REQUIRE_EQUAL(encoded, expected_encoded);
}

BOOST_AUTO_TEST_CASE(key_factory__verification_key_string)
{
    const coinninja::wallet::base_coin coin{};
    const auto master_private_key{private_key_for(test_only_words, coin)};
    const auto verification_string{coinninja::wallet::key_factory::coinninja_verification_key_hex_string(master_private_key)};

    const std::string expected_string{"024458596b5c97e716e82015a72c37b5d3fe0c5dc70a4b83d72e7d2eb65920633e"};

    BOOST_REQUIRE_EQUAL(verification_string, expected_string);
}

BOOST_AUTO_TEST_SUITE_END()