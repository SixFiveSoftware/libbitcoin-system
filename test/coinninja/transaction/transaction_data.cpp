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
    transaction_data::create_transaction_data(
        tx_data, test_address, coin, outputs, payment_amount, fee_rate, &change_path, 500000
    );

    // then
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
    transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
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
    transaction_data::create_transaction_data(
        tx_data, test_address, coin, utxos, payment_amount, fee_rate, &change_path, 500000
    );

    // then
    BOOST_REQUIRE_EQUAL(tx_data.amount, payment_amount);
    BOOST_REQUIRE_EQUAL(tx_data.fee_amount, expected_fee_amount);
    BOOST_REQUIRE_EQUAL(tx_data.change_amount, expected_change_amount);
    BOOST_REQUIRE_EQUAL(tx_data.unspent_transaction_outputs.size(), expected_number_of_utxos);
    BOOST_REQUIRE_EQUAL(tx_data.locktime, expected_locktime);
}

BOOST_AUTO_TEST_SUITE_END()