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
#include <src/coinninja/address/address_helper.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>

using namespace coinninja;
using namespace coinninja::address;
using namespace coinninja::wallet;

BOOST_AUTO_TEST_SUITE(cn_address_helper_tests)

// Address type for address
BOOST_AUTO_TEST_CASE(address_helper__p2sh_p2wpkh__address)
{
    const std::string address = "35dKN7xvHH3xnBWUrWzJtkjfrAFXk6hyH8";
    const base_coin coin{BIP49};
    address_helper helper{coin};

    payment_output_type type{helper.address_type_for_address(address)};

    BOOST_REQUIRE_EQUAL(type, coinninja::address::payment_output_type::P2SH);
}

BOOST_AUTO_TEST_CASE(address_helper__p2pkh__address)
{
    const std::string address = "1LqBGSKuX5yYUonjxT5qGfpUsXKYYWeabA";
    const base_coin coin{BIP44};
    address_helper helper{coin};

    payment_output_type type{helper.address_type_for_address(address)};

    BOOST_REQUIRE_EQUAL(type, coinninja::address::payment_output_type::P2PKH);
}

BOOST_AUTO_TEST_CASE(address_helper__p2wpkh__address)
{
    const std::string address = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4";
    const base_coin coin{BIP84};
    address_helper helper{coin};

    payment_output_type type{helper.address_type_for_address(address)};

    BOOST_REQUIRE_EQUAL(type, coinninja::address::payment_output_type::P2WPKH);
}

BOOST_AUTO_TEST_CASE(address_helper__p2wsh__address)
{
    const std::string address = "bc1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3qccfmv3";
    const base_coin coin{BIP84};
    address_helper helper{coin};

    payment_output_type type{helper.address_type_for_address(address)};

    BOOST_REQUIRE_EQUAL(type, coinninja::address::payment_output_type::P2WSH);
}

// input byte size
BOOST_AUTO_TEST_CASE(address_helper__bip_84__input_size)
{
    const base_coin coin{BIP84};
    const uint16_t expected_bytes{68};
    address_helper helper{coin};

    auto bytes = helper.bytes_per_input();

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

BOOST_AUTO_TEST_CASE(address_helper__bip_49__input_size)
{
    const base_coin coin{BIP49};
    const uint16_t expected_bytes{91};
    address_helper helper{coin};

    auto bytes = helper.bytes_per_input();

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

// change output byte size
BOOST_AUTO_TEST_CASE(address_helper__bip_49__change_size)
{
    const base_coin coin{BIP49};
    const uint16_t expected_bytes{32};
    address_helper helper{coin};

    auto bytes = helper.bytes_per_change_output();

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

BOOST_AUTO_TEST_CASE(address_helper__bip_84__change_size)
{
    const base_coin coin{BIP84};
    const uint16_t expected_bytes{31};
    address_helper helper{coin};

    auto bytes = helper.bytes_per_change_output();

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

// total bytes
BOOST_AUTO_TEST_CASE(address_helper__bip_49_input__two_bip_49_outputs__total_size)
{
    const std::string address = "35dKN7xvHH3xnBWUrWzJtkjfrAFXk6hyH8";
    const base_coin coin{BIP49};
    const uint16_t expected_bytes{166};
    address_helper helper{coin};

    auto bytes = helper.total_bytes(1, address, true);

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

BOOST_AUTO_TEST_CASE(address_helper__bip_84_input__two_bip_84_outputs__total_size)
{
    const std::string address = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4";
    const base_coin coin{BIP84};
    const uint16_t expected_bytes{141};
    address_helper helper{coin};

    auto bytes = helper.total_bytes(1, address, true);

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

BOOST_AUTO_TEST_CASE(address_helper__bip_49_input__legacy_output__with_bip_49_change__total_size)
{
    const std::string address = "1LqBGSKuX5yYUonjxT5qGfpUsXKYYWeabA";
    const base_coin coin{BIP49};
    const uint16_t expected_bytes{168};
    address_helper helper{coin};

    auto bytes = helper.total_bytes(1, address, true);

    BOOST_REQUIRE_EQUAL(bytes, expected_bytes);
}

BOOST_AUTO_TEST_SUITE_END()