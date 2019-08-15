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
#include <bitcoin/bitcoin/coinninja/transaction/transaction_builder.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/unspent_transaction_output.hpp>
#include <vector>

using namespace coinninja::wallet;
using namespace coinninja::transaction;
using namespace bc::wallet;

bc::wallet::hd_private private_key_for(const word_list &words, const base_coin &coin)
{
    auto seed = bc::to_chunk(decode_mnemonic(words));
    auto net = (coin.get_coin() == 0) ? hd_private::mainnet : hd_private::testnet;
    return hd_private(seed, net);
}

BOOST_AUTO_TEST_SUITE(cn_transaction_builder_tests)

BOOST_AUTO_TEST_CASE(transaction_gets_built_correctly)
{
    // given
    word_list words {
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"abandon"},
        std::string{"about"}
    };
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path input_path{49, 0, 0, 1, 53};
    unspent_transaction_output utxo{"1a08dafe993fdc17fdc661988c88f97a9974013291e759b9b5766b8e97c78f87", 1, 2788424, input_path, true};
    std::vector<unspent_transaction_output> utxos;
    utxos.push_back(utxo);
    uint64_t amount{13584};
    uint64_t fees_amount{3000};
    uint64_t change_amount{2771840};
    derivation_path change_path{49, 0, 0, 1, 56};
    std::string to_address{"3BgxxADLtnoKu9oytQiiVzYUqvo8weCVy9"};
    std::string change_address{"3A98wWjDyJECXTCKJSiXgTZ5hrvRTdHsmg"};

    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 539943};
    
    std::string expected_encoded_tx{"01000000000101878fc7978e6b76b5b959e791320174997af9888c9861c6fd17dc3f99feda081a0100000017160014509060a6bedf13087124c0aeafc6e3db4e1e9a08ffffffff02103500000000000017a9146daec6ddb6faaf01f83f515045822a94d0c2331e87804b2a000000000017a914e0bc3e6f5f4080b4f007c6307ba579595e459a06870247304402205b2d50ca2b20fa290323687c3e60bfd4702f9082544afeeb62d849437d04092002204d6dbdef48a992e20700452eff01966d08dcc767b4e7a205c78617d8b5faa1f7012103d447f34dd13359a8fc64ed3977fcecea3f6802f842f9a9f857de07453b715735273d0800"};
    std::string expected_txid{"9ea15d4a60c33a1be64da5805c399663831f7aee13724bfa702db2c3cfafd5bb"};
    std::string expected_change_address{"3NBJnvo9U5YbJnr1pALFqQEur1wXWJrjoM"};

    hd_private master_private_key = private_key_for(words, coin);
    transaction_builder builder{master_private_key, coin};

    // when
    auto tx_metadata = builder.generate_tx_metadata(tx_data);
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    // then
    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 56);
}

BOOST_AUTO_TEST_SUITE_END()