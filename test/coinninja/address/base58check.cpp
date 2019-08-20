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
#include <bitcoin/bitcoin/coinninja/address/base58check.hpp>
#include <test/test_helpers.hpp>
#include <string>

using namespace bc::wallet;
using namespace coinninja::address;

BOOST_AUTO_TEST_SUITE(cn__base58check)

BOOST_AUTO_TEST_CASE(base58check__valid_p2pkh_address)
{
    const auto address{"12vRFewBpbdiS5HXDDLEfVFtJnpA2x8NV8"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2pkh_address)
{
    const auto address{"12vRFewBpbdiS5HXDDLEfVFtJnpA2"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2pkh_with_trailing_chars_removed)
{
    const auto address{"12vRFewBpbdiS5HXDDLEfVFt"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2pkh_with_first_10_chars_removed)
{
    const auto address{"diS5HXDDLEfVFtJnpA2x8NV8"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2pkh_with_extra_leading_digit)
{
    const auto address{"212vRFewBpbdiS5HXDDLEfVFtJnpA2x8NV8"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2pkh_with_different_leading_digit)
{
    const auto address{"42vRFewBpbdiS5HXDDLEfVFtJnpA2x8NV8"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__valid_p2sh_address)
{
    const auto address{"3EH9Wj6KWaZBaYXhVCa8ZrwpHJYtk44bGX"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2sh_with_trailing_chars_removed)
{
    const auto address{"3EH9Wj6KWaZBaYXhVCa8ZrwpHJYtk"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2sh_with_first_5_chars_removed)
{
    const auto address{"j6KWaZBaYXhVCa8ZrwpHJYtk44bGX"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2sh_with_first_10_chars_removed)
{
    const auto address{"ZBaYXhVCa8ZrwpHJYtk44bGX"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2sh_with_extra_leading_digit)
{
    const auto address{"23EH9Wj6KWaZBaYXhVCa8ZrwpHJYtk44bGX"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__invalid_p2sh_with_different_leading_digit)
{
    const auto address{"4EH9Wj6KWaZBaYXhVCa8ZrwpHJYtk44bGX"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__valid_dropbit_p2sh_p2wpkh_address)
{
    const auto address{"3Cd4xEu2VvM352BVgd9cb1Ct5vxz318tVT"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__ethereum_address__is_invalid)
{
    const auto address{"0xF26C29D25a1E1696c5CC54DE4bf2AEc906EB4F79"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__BCH_address__is_invalid)
{
    const auto address{"qr45rul6luexjgg5h8p26c0cs6rrhwzrkg6e0hdvrf"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__song_lyrics__are_invalid)
{
    const auto address{"Jenny86753098675309IgotIt"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__gibberish__is_invalid)
{
    const auto address{"31415926535ILikePi89793238462643"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__foo__is_invalid)
{
    const auto address{"foo"};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_CASE(base58check__empty_string__is_invalid)
{
    const auto address{""};
    bool is_valid{base58check::verify_base_58_check_encoding(address)};
    BOOST_REQUIRE(!is_valid);
}

BOOST_AUTO_TEST_SUITE_END()