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
#include <bitcoin/bitcoin/coinninja/address/segwit_address.hpp>
#include <bitcoin/bitcoin/coinninja/address/bech32.hpp>
#include <bitcoin/bitcoin/coinninja/address/usable_address.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>
#include <test/test_helpers.hpp>
#include <string>
#include <vector>

using namespace bc::wallet;
using namespace coinninja::address;
using namespace coinninja::wallet;

BOOST_AUTO_TEST_SUITE(cn_usable_address)

BOOST_AUTO_TEST_CASE(usable_address__build_payment_address__non_native_segwit_mainnet)
{
    //given
    base_coin coin;
    derivation_path path0{49, 0, 0, 0, 0};
    derivation_path path1{49, 0, 0, 0, 1};
    derivation_path path2{49, 0, 0, 0, 2};
    derivation_path change_path0{49, 0, 0, 1, 0};
    derivation_path change_path1{49, 0, 0, 1, 1};
    derivation_path change_path2{49, 0, 0, 1, 2};

    std::string expected_address0{"37VucYSaXLCAsxYyAPfbSi9eh4iEcbShgf"};
    std::string expected_address1{"3LtMnn87fqUeHBUG414p9CWwnoV6E2pNKS"};
    std::string expected_address2{"3B4cvWGR8X6Xs8nvTxVUoMJV77E4f7oaia"};
    std::string expected_change_address0{"34K56kSjgUCUSD8GTtuF7c9Zzwokbs6uZ7"};
    std::string expected_change_address1{"3516F2wmK51jVRrggEJsTUBNWMSLLjzvJ2"};
    std::string expected_change_address2{"3Grd7y95JEDTSh9uiVF5q7z2qGzmkP19CV"};

    const auto master_private_key{private_key_for(test_only_words, coin)};

    // when
    usable_address address0{master_private_key, path0};
    usable_address address1{master_private_key, path1};
    usable_address address2{master_private_key, path2};
    usable_address change_address0{master_private_key, change_path0};
    usable_address change_address1{master_private_key, change_path1};
    usable_address change_address2{master_private_key, change_path2};

    // then
    BOOST_REQUIRE_EQUAL(address0.build_payment_address().encoded(), expected_address0);
    BOOST_REQUIRE_EQUAL(address1.build_payment_address().encoded(), expected_address1);
    BOOST_REQUIRE_EQUAL(address2.build_payment_address().encoded(), expected_address2);
    BOOST_REQUIRE_EQUAL(change_address0.build_payment_address().encoded(), expected_change_address0);
    BOOST_REQUIRE_EQUAL(change_address1.build_payment_address().encoded(), expected_change_address1);
    BOOST_REQUIRE_EQUAL(change_address2.build_payment_address().encoded(), expected_change_address2);
}

BOOST_AUTO_TEST_CASE(usable_address__build_payment_address__non_native_segwit_testnet)
{
    //given
    base_coin coin;
    derivation_path path0{49, 1, 0, 0, 0};
    derivation_path path1{49, 1, 0, 0, 1};
    derivation_path path2{49, 1, 0, 0, 2};
    derivation_path change_path0{49, 1, 0, 1, 0};
    derivation_path change_path1{49, 1, 0, 1, 1};
    derivation_path change_path2{49, 1, 0, 1, 2};

    std::string expected_address0{"2Mww8dCYPUpKHofjgcXcBCEGmniw9CoaiD2"};
    std::string expected_address1{"2N55m54k8vr95ggehfUcNkdbUuQvaqG2GxK"};
    std::string expected_address2{"2N9LKph9TKtv1WLDfaUJp4D8EKwsyASYnGX"};
    std::string expected_change_address0{"2MvdUi5o3f2tnEFh9yGvta6FzptTZtkPJC8"};
    std::string expected_change_address1{"2NCtHHE9TjYrYnUWfZv79w9ktk1f2uPUzqu"};
    std::string expected_change_address2{"2N9vVCnXmavTBmxRjoPjoHZ9q9ujEKXXGXe"};

    const auto master_private_key{private_key_for(test_only_words, coin)};

    // when
    usable_address address0{master_private_key, path0};
    usable_address address1{master_private_key, path1};
    usable_address address2{master_private_key, path2};
    usable_address change_address0{master_private_key, change_path0};
    usable_address change_address1{master_private_key, change_path1};
    usable_address change_address2{master_private_key, change_path2};

    // then
    BOOST_REQUIRE_EQUAL(address0.build_payment_address().encoded(), expected_address0);
    BOOST_REQUIRE_EQUAL(address1.build_payment_address().encoded(), expected_address1);
    BOOST_REQUIRE_EQUAL(address2.build_payment_address().encoded(), expected_address2);
    BOOST_REQUIRE_EQUAL(change_address0.build_payment_address().encoded(), expected_change_address0);
    BOOST_REQUIRE_EQUAL(change_address1.build_payment_address().encoded(), expected_change_address1);
    BOOST_REQUIRE_EQUAL(change_address2.build_payment_address().encoded(), expected_change_address2);
}

BOOST_AUTO_TEST_CASE(usable_address__build_payment_address__native_segwit_mainnet)
{
    //given
    base_coin coin;
    derivation_path path0{84, 0, 0, 0, 0};
    derivation_path path1{84, 0, 0, 0, 1};
    derivation_path path2{84, 0, 0, 0, 2};
    derivation_path change_path0{84, 0, 0, 1, 0};
    derivation_path change_path1{84, 0, 0, 1, 1};
    derivation_path change_path2{84, 0, 0, 1, 2};

    std::string expected_address0{"bc1qcr8te4kr609gcawutmrza0j4xv80jy8z306fyu"};
    std::string expected_address1{"bc1qnjg0jd8228aq7egyzacy8cys3knf9xvrerkf9g"};
    std::string expected_address2{"bc1qp59yckz4ae5c4efgw2s5wfyvrz0ala7rgvuz8z"};
    std::string expected_change_address0{"bc1q8c6fshw2dlwun7ekn9qwf37cu2rn755upcp6el"};
    std::string expected_change_address1{"bc1qggnasd834t54yulsep6fta8lpjekv4zj6gv5rf"};
    std::string expected_change_address2{"bc1qn8alfh45rlsj44pcdt0f2cadtztgnz4gq3h3uf"};

    const auto master_private_key{private_key_for(test_only_words, coin)};

    // when
    usable_address address0{master_private_key, path0};
    usable_address address1{master_private_key, path1};
    usable_address address2{master_private_key, path2};
    usable_address change_address0{master_private_key, change_path0};
    usable_address change_address1{master_private_key, change_path1};
    usable_address change_address2{master_private_key, change_path2};

    // then
    BOOST_REQUIRE_EQUAL(address0.build_receive_address().get_address(), expected_address0);
    BOOST_REQUIRE_EQUAL(address1.build_receive_address().get_address(), expected_address1);
    BOOST_REQUIRE_EQUAL(address2.build_receive_address().get_address(), expected_address2);
    BOOST_REQUIRE_EQUAL(change_address0.build_change_address().get_address(), expected_change_address0);
    BOOST_REQUIRE_EQUAL(change_address1.build_change_address().get_address(), expected_change_address1);
    BOOST_REQUIRE_EQUAL(change_address2.build_change_address().get_address(), expected_change_address2);
}

BOOST_AUTO_TEST_CASE(usable_address__build_payment_address__native_segwit_testnet)
{
    //given
    base_coin coin;
    derivation_path path0{84, 1, 0, 0, 0};
    derivation_path path1{84, 1, 0, 0, 1};
    derivation_path path2{84, 1, 0, 0, 2};
    derivation_path change_path0{84, 1, 0, 1, 0};
    derivation_path change_path1{84, 1, 0, 1, 1};
    derivation_path change_path2{84, 1, 0, 1, 2};

    std::string expected_address0{"tb1q6rz28mcfaxtmd6v789l9rrlrusdprr9pqcpvkl"};
    std::string expected_address1{"tb1qd7spv5q28348xl4myc8zmh983w5jx32cjhkn97"};
    std::string expected_address2{"tb1qxdyjf6h5d6qxap4n2dap97q4j5ps6ua8sll0ct"};
    std::string expected_change_address0{"tb1q9u62588spffmq4dzjxsr5l297znf3z6j5p2688"};
    std::string expected_change_address1{"tb1qkwgskuzmmwwvqajnyr7yp9hgvh5y45kg8wvdmd"};
    std::string expected_change_address2{"tb1q2vma00td2g9llw8hwa8ny3r774rtt7aenfn5zu"};

    const auto master_private_key{private_key_for(test_only_words, coin)};

    // when
    usable_address address0{master_private_key, path0};
    usable_address address1{master_private_key, path1};
    usable_address address2{master_private_key, path2};
    usable_address change_address0{master_private_key, change_path0};
    usable_address change_address1{master_private_key, change_path1};
    usable_address change_address2{master_private_key, change_path2};

    // then
    BOOST_REQUIRE_EQUAL(address0.build_receive_address().get_address(), expected_address0);
    BOOST_REQUIRE_EQUAL(address1.build_receive_address().get_address(), expected_address1);
    BOOST_REQUIRE_EQUAL(address2.build_receive_address().get_address(), expected_address2);
    BOOST_REQUIRE_EQUAL(change_address0.build_change_address().get_address(), expected_change_address0);
    BOOST_REQUIRE_EQUAL(change_address1.build_change_address().get_address(), expected_change_address1);
    BOOST_REQUIRE_EQUAL(change_address2.build_change_address().get_address(), expected_change_address2);
}

BOOST_AUTO_TEST_SUITE_END()
