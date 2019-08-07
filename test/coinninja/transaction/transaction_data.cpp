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
#include <bitcoin/bitcoin/coinninja/transaction/transaction_data.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/unspent_transaction_output.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>
#include <src/coinninja/address/address_helper.hpp>
#include <string>
#include <vector>

using namespace coinninja::wallet;
using namespace coinninja::transaction;
using namespace coinninja::address;

const std::string test_address{"37VucYSaXLCAsxYyAPfbSi9eh4iEcbShgf"};
base_coin coin{coin_derivation_purpose::BIP49};
address_helper helper{coin};

BOOST_AUTO_TEST_SUITE(cn_transaction_data_tests)

BOOST_AUTO_TEST_CASE(single_output__single_input__satisfies_amount)
{
    // given
    uint64_t payment_amount{50000000}; // 0.5 BTC
    uint64_t utxo_amount{100000000};   // 1.0 BTC
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output output{"previous txid", 0, utxo_amount, utxo_path, true};
    std::vector<unspent_transaction_output> outputs{output};
    uint16_t fee_rate{30};
    uint16_t total_bytes = helper.total_bytes(1, test_address, true);
    auto expected_fee_amount{fee_rate * total_bytes};  // 4,980
    auto expected_change_amount{utxo_amount - payment_amount - expected_fee_amount}; // 0.49995020
    auto expected_number_of_utxos{1};
    auto expected_locktime{500000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, test_address, coin, outputs, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_CASE(single_output__double_input__and_change__satisfies_amount)
{
    // given
    uint64_t payment_amount{50000000}; // 0.5 BTC
    uint64_t utxo_amount{30000000};    // 0.3 BTC
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output utxo1{"previous txid", 0, utxo_amount, utxo_path, true};
    unspent_transaction_output utxo2{"previous txid", 1, utxo_amount, utxo_path, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    uint16_t fee_rate{30};
    uint16_t total_bytes = helper.total_bytes(utxos.size(), test_address, true);
    auto expected_fee_amount{fee_rate * total_bytes};  // 7,710
    auto amount_from_utxos{0};
    for (auto &utxo : utxos) { amount_from_utxos += utxo.amount; }
    auto expected_change_amount{amount_from_utxos - payment_amount - expected_fee_amount};
    auto expected_number_of_utxos{2};
    auto expected_locktime{500000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_CASE(single_output__single_input__no_change__satisfies_amount)
{
    // given
    uint64_t payment_amount{50000000}; // 0.50000000 BTC
    uint64_t utxo_amount{50004020};    // 0.50004020 BTC
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output utxo1{"previous txid", 0, utxo_amount, utxo_path, true};
    std::vector<unspent_transaction_output> utxos{utxo1};
    uint16_t fee_rate{30};
    uint16_t total_bytes = helper.total_bytes(utxos.size(), test_address, false);
    auto expected_fee_amount{fee_rate * total_bytes};  // 4,020
    auto expected_change_amount{0};
    auto expected_number_of_utxos{1};
    auto expected_locktime{500000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_CASE(single_output__double_input__no_change__satisfies_amount)
{
    // given
    uint64_t payment_amount{50000000}; // 0.50000000 BTC
    uint64_t utxo_amount_1{20001750};    // 0.20001750 BTC
    uint64_t utxo_amount_2{30005000};    // 0.30005000 BTC
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output utxo1{"previous txid", 0, utxo_amount_1, utxo_path, true};
    unspent_transaction_output utxo2{"previous txid", 1, utxo_amount_2, utxo_path, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    uint16_t fee_rate{30};
    uint16_t total_bytes = helper.total_bytes(utxos.size(), test_address, false);
    auto expected_fee_amount{fee_rate * total_bytes};  // 6,750
    auto expected_change_amount{0};
    auto expected_number_of_utxos{2};
    auto expected_locktime{500000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_CASE(single_output__double_input__insufficient_funds__returns_false)
{
    // given
    uint64_t payment_amount{50000000}; // 0.50000000 BTC
    uint64_t utxo_amount_1{20000000};    // 0.20000000 BTC
    uint64_t utxo_amount_2{10000000};    // 0.10000000 BTC
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output utxo1{"previous txid", 0, utxo_amount_1, utxo_path, true};
    unspent_transaction_output utxo2{"previous txid", 1, utxo_amount_2, utxo_path, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    uint16_t fee_rate{30};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(!success);
}

BOOST_AUTO_TEST_CASE(native_segwit_output__p2sh_segwit_input__returns_true)
{
    // given
    uint64_t payment_amount{50000000}; // 0.50000000 BTC
    uint64_t utxo_amount_1{30000000};    // 0.30000000 BTC
    uint64_t utxo_amount_2{30000000};    // 0.30000000 BTC
    std::string segwit_address{"bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4"};
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 0};
    derivation_path utxo_path{coin_derivation_purpose::BIP49};
    unspent_transaction_output utxo1{"previous txid", 0, utxo_amount_1, utxo_path, true};
    unspent_transaction_output utxo2{"previous txid", 1, utxo_amount_2, utxo_path, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    uint16_t fee_rate{30};
    uint16_t total_bytes = helper.total_bytes(utxos.size(), segwit_address, true);
    auto expected_fee_amount{fee_rate * total_bytes};  // 7,680
    auto expected_change_amount{9992320}; // 0.6BTC - 0.5BTC - 0.00007680BTC fee
    auto expected_number_of_utxos{2};
    auto expected_locktime{500000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_transaction_data(
        tx_data, segwit_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(total_bytes, 256);
    BOOST_REQUIRE_EQUAL(expected_fee_amount, 7680);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_CASE(cost_of_change__is_beneficial)
{
    // given
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 100000, path1, true};
    derivation_path path2{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 2};
    unspent_transaction_output utxo2{"419a7a7d27e0c4341ca868d0b9744ae7babb18fd691e39be608b556961c00ade", 0, 100000, path2, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 5};
    uint16_t fee_rate{10};
    uint16_t total_bytes{helper.total_bytes(utxos.size(), test_address, false)};
    auto dusty_change{1100};
    auto expected_fee_amount{fee_rate * total_bytes + dusty_change}; // 2,250 + 1,100 = 3,350
    auto payment_amount{utxo1.amount + utxo2.amount - expected_fee_amount}; // 200,000 - 3,350 = 196,650

    // when
    transaction_data tx_data;
    bool success1 = transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success1);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), utxos.size());
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, nullptr);

    // when again
    payment_amount = 194000;
    expected_fee_amount = 2570;
    auto expected_change = 3430;
    transaction_data good_tx_data;
    bool success2 = transaction_data::create_transaction_data(
        good_tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then again
    BOOST_REQUIRE(success2);
    BOOST_REQUIRE_EQUAL(good_tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(good_tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(good_tx_data.unspent_transaction_outputs.size(), utxos.size());
    BOOST_REQUIRE_EQUAL(good_tx_data.change_amount, expected_change);
    BOOST_REQUIRE_EQUAL(good_tx_data.change_path, &change_path);
}

/// flat fee tests
BOOST_AUTO_TEST_CASE(tx_data__with_flat_fee__calculates_utxos_and_change)
{
    // given
    const derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    const derivation_path path2{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 2};
    const derivation_path path3{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 8};
    const unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 2221, path1, true};
    const unspent_transaction_output utxo2{"419a7a7d27e0c4341ca868d0b9744ae7babb18fd691e39be608b556961c00ade", 0, 15935, path2, true};
    const unspent_transaction_output utxo3{"3013fcd9ea8fd65a69709f07fed2c1fd765d57664486debcb72ef47f2ea415f6", 0, 15526, path3, true};
    const std::vector<unspent_transaction_output> utxos{utxo1, utxo2, utxo3};
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 5};
    const auto payment_amount{20000};
    const auto flat_fee_amount{10000};
    const auto expected_change{3682};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_flat_fee_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, flat_fee_amount, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, flat_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, &change_path);
}

BOOST_AUTO_TEST_CASE(dusty_transaction_data__with_flat_fee__calculates_utxos_and_no_change)
{
    // given
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    derivation_path path2{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 2};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 20000, path1, true};
    unspent_transaction_output utxo2{"419a7a7d27e0c4341ca868d0b9744ae7babb18fd691e39be608b556961c00ade", 0, 10100, path2, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    derivation_path change_path{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 5};
    auto payment_amount{20000};
    auto expected_fee_amount{10000};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_flat_fee_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, expected_fee_amount, &change_path, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, nullptr);
}

/// send max tests
BOOST_AUTO_TEST_CASE(send_max__uses_all_utxos__amount_is_total_value_minus_fee)
{
    // given
    auto fee_rate{5};
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    derivation_path path2{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 2};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 20000, path1, true};
    unspent_transaction_output utxo2{"419a7a7d27e0c4341ca868d0b9744ae7babb18fd691e39be608b556961c00ade", 0, 10000, path2, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    auto input_amount{utxo1.amount + utxo2.amount};
    auto total_bytes{helper.total_bytes(utxos.size(), test_address, false)};
    auto expected_fee_amount{fee_rate * total_bytes}; // 1,125
    auto expected_amount{input_amount - expected_fee_amount};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_send_max_transaction_data(
        tx_data, utxos, coin, test_address, fee_rate, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, expected_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, nullptr);
}

BOOST_AUTO_TEST_CASE(send_max__with_insufficient_funds__returns_false)
{
    // given
    auto fee_rate{5};
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 100, path1, true};
    std::vector<unspent_transaction_output> utxos{utxo1};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_send_max_transaction_data(
        tx_data, utxos, coin, test_address, fee_rate, 500000
    );

    // then
    BOOST_REQUIRE(!success);
}

BOOST_AUTO_TEST_CASE(send_max__with_just_enough_funds__returns_true)
{
    // given
    auto fee_rate{5};
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 670, path1, true};
    std::vector<unspent_transaction_output> utxos{utxo1};
    auto input_amount{utxo1.amount};
    auto total_bytes{helper.total_bytes(utxos.size(), test_address, false)};
    auto expected_fee_amount{fee_rate * total_bytes}; // 670
    auto expected_amount{input_amount - expected_fee_amount}; // 0

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_send_max_transaction_data(
        tx_data, utxos, coin, test_address, fee_rate, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(tx_data.amount, expected_amount);
    BOOST_REQUIRE_EQUAL(tx_data.amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, 670);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, nullptr);
}

BOOST_AUTO_TEST_CASE(send_max__to_native_segwit_output__returns_true)
{
    // given
    auto fee_rate{5};
    derivation_path path1{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 1, 3};
    derivation_path path2{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet, 0, 0, 2};
    unspent_transaction_output utxo1{"909ac6e0a31c68fe345cc72d568bbab75afb5229b648753c486518f11c0d0009", 1, 20000, path1, true};
    unspent_transaction_output utxo2{"419a7a7d27e0c4341ca868d0b9744ae7babb18fd691e39be608b556961c00ade", 0, 10000, path2, true};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    std::string segwit_address{"bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4"};
    auto input_amount{utxo1.amount + utxo2.amount};
    auto total_bytes{helper.total_bytes(utxos.size(), segwit_address, false)};  // 224
    auto expected_fee_amount{fee_rate * total_bytes}; // 1,120
    auto expected_amount{input_amount - expected_fee_amount};

    // when
    transaction_data tx_data;
    bool success = transaction_data::create_send_max_transaction_data(
        tx_data, utxos, coin, segwit_address, fee_rate, 500000
    );

    // then
    BOOST_REQUIRE(success);
    BOOST_REQUIRE_EQUAL(total_bytes, 224);
    BOOST_REQUIRE_EQUAL(tx_data.amount, expected_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, 0);
    BOOST_REQUIRE_EQUAL(tx_data.change_path, nullptr);
}

BOOST_AUTO_TEST_SUITE_END()