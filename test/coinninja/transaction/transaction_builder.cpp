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
#include <bitcoin/bitcoin/coinninja/transaction/transaction_builder.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/unspent_transaction_output.hpp>
#include <test/test_helpers.hpp>
#include <vector>

using namespace coinninja::wallet;
using namespace coinninja::transaction;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(cn_transaction_builder_tests)

BOOST_AUTO_TEST_CASE(transaction_builder__transaction_gets_built_correctly)
{
    // given
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

    hd_private master_private_key = private_key_for(test_only_words, coin);
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

BOOST_AUTO_TEST_CASE(transaction_builder__builds_tx__with_two_inputs)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path path1{49, 0, 0, 1, 56};
    unspent_transaction_output utxo1{"24cc9150963a2369d7f413af8b18c3d0243b438ba742d6d083ec8ed492d312f9", 1, 2769977, path1, false};
    derivation_path path2{49, 0, 0, 1, 57};
    unspent_transaction_output utxo2{"ed611c20fc9088aa5ec1c86de88dd017965358c150c58f71eda721cdb2ac0a48", 1, 314605, path2, false};
    std::vector<unspent_transaction_output> utxos{utxo1, utxo2};
    uint64_t amount{3000000};
    uint64_t fees_amount{4000};
    uint64_t change_amount{80582};
    derivation_path change_path{49, 0, 0, 1, 58};
    std::string to_address{"3CkiUcj5vU4TGZJeDcrmYGWH8GYJ5vKcQq"};
    transaction_data tx_data = transaction_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 540220};

    const std::string expected_encoded_tx{"01000000000102f912d392d48eec83d0d642a78b433b24d0c3188baf13f4d769233a965091cc24010000001716001436386ac950d557ae06bfffc51e7b8fa08474c05ffdffffff480aacb2cd21a7ed718fc550c158539617d08de86dc8c15eaa8890fc201c61ed010000001716001480e1e7dc2f6436a60abec5e9e7f6b62b0b9985c4fdffffff02c0c62d000000000017a914795c7bc23aebac7ddea222bb13c5357b32ed0cd487c63a01000000000017a914a4a2fab6264d22efbfc997f30738ccc6db0f8c05870247304402202a1dfa92a9dba16fa476c738197316009665f1b705e5626b2729b136bb64aaa102203041d91270d91124cb9341c6d1bfb2c7aa3372ef85f412fa00b8bf4fa7091f2b0121027c3fde52baba263e526ee5acc051f7fd69000eb633b8cf7decd1334db8fb44ee02483045022100a3843ddb39dd088e8d9657eaed5454a27737112c821eb6f674414e02f295d39402206de16b7c5b1ff054d102451a9242b10fccf81828003c377046bd11fa6c025179012103cbd9a8066a39e1d05ec26b72116e84b8b852b6784a6359ebb35f5794445245883c3e0800"};
    const std::string expected_txid{"f94e7111736dd2a5fd1c5bbcced153f90d17ee1b032f166dda785354f4063651"};
    const std::string expected_change_address{"3GhXz1NGhwQusEiBYKKhTqQYE6MKt2utDN"};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 58);
}

BOOST_AUTO_TEST_CASE(transaction_builder__build_single_utxo_mainnet)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path path{49, 0, 0, 0, 0};
    unspent_transaction_output utxo{"3480e31ea00efeb570472983ff914694f62804e768a6c6b4d1b6cd70a1cd3efa", 1, 449893, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{218384};
    const uint64_t fees_amount{668};
    const uint64_t change_amount{230841};
    derivation_path change_path{49, 0, 0, 1, 0};
    const std::string to_address{"3ERQiyXSeUYmxxqKyg8XwqGo4W7utgDrTR"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 500000};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const std::string expected_encoded_tx{"01000000000101fa3ecda170cdb6d1b4c6a668e70428f6944691ff83294770b5fe0ea01ee380340100000017160014f990679acafe25c27615373b40bf22446d24ff44ffffffff02105503000000000017a9148ba60342bf59f73327fecab2bef17c1612888c3587b98503000000000017a9141cc1e09a63d1ae795a7130e099b28a0b1d8e4fae8702483045022100ee83c25a1dd4f7c31724cbdb3ea4b12c65dbc7c9c5c602033614d436f9a5a947022008f84fbc34fd9fafe7b1581d8a980c558bdd1429b4a6badb126235aea5bf9c230121039b3b694b8fc5b5e07fb069c783cac754f5d38c3e08bed1960e31fdb1dda35c2420a10700"};
    const std::string expected_txid{"800b1a9284624eed1eeafe68bad3f588877ce20b1ecc884dccf882d22ab03799"};
    const std::string expected_change_address{"34K56kSjgUCUSD8GTtuF7c9Zzwokbs6uZ7"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 0);
}

BOOST_AUTO_TEST_CASE(transaction_builder__build_p2kh__no_change__mainnet)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path path{49, 0, 0, 1, 7};
    unspent_transaction_output utxo{"f14914f76ad26e0c1aa5a68c82b021b854c93850fde12f8e3188c14be6dc384e", 1, 33253, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{23147};
    const uint64_t fees_amount{10108};
    derivation_path change_path{49, 0, 0, 1, 2};
    const std::string to_address{"1HT6WtD5CAToc8wZdacCgY4XjJR4jV5Q5d"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, 0, change_path, 500000};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const std::string expected_encoded_tx{"010000000001014e38dce64bc188318e2fe1fd5038c954b821b0828ca6a51a0c6ed26af71449f10100000017160014b4381165b195b3286079d46eb2dc8058e6f02241ffffffff016b5a0000000000001976a914b4716e71b900b957e49f749c8432b910417788e888ac02483045022100f8a78ff2243c591ffb7af46ed670b173e5e5dd3f19853493f5c3bda85425f8ef02203d152fdc632388da527c4a58b796a8a40d1a9d15176d80dedfef96a38ecc9ae7012103a45ef894ab9e6f2e55683561181be9e69b20207af746d60b95fab33476dc932420a10700"};
    const std::string expected_txid{"77cf4bddf3d133fc37a08e18c47607702e0aec095606f364081d22a4680c3e97"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_vout_index(), nullptr);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_address(), nullptr);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path(), nullptr);
}

BOOST_AUTO_TEST_CASE(transaction_builder__single_utxo__no_change__mainnet)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path path{49, 0, 0, 0, 0};
    unspent_transaction_output utxo{"9a8bb554e1dded3a39867b7b3b651b1f9b57b4fb5b8823f421d1c8fc3d1ffac2", 1, 154254, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{152254};
    const uint64_t fees_amount{2000};
    const uint64_t change_amount{0};
    derivation_path change_path{49, 0, 0, 1, 0};
    const std::string to_address{"3Aftutd9VvzLcGxD9VraNhWiHyjR5pvn5N"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 541133};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const std::string expected_encoded_tx{"01000000000101c2fa1f3dfcc8d121f423885bfbb4579b1f1b653b7b7b86393aeddde154b58b9a0100000017160014f990679acafe25c27615373b40bf22446d24ff44ffffffff01be5202000000000017a9146282dd071fc766f3749136dd0fee99c4f3db17038702483045022100f9ffebc8107b7342b4256d6879fe48c3858c9a141c2e405ef064fd416efdb01702206991995c00c6f6cf2add7710c959fe9cabf6e9e94f82a596813cef0b12bc37430121039b3b694b8fc5b5e07fb069c783cac754f5d38c3e08bed1960e31fdb1dda35c24cd410800"};
    const std::string expected_txid{"17a335f1ff0c8d69c080e2f9b98c427244fce36ed749ad427684ac84cebc0512"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_vout_index(), nullptr);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_address(), nullptr);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path(), nullptr);
}

BOOST_AUTO_TEST_CASE(transaction_builder__testnet__metadata_creation)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::TestNet};
    derivation_path path{49, 1, 0, 0, 0};
    unspent_transaction_output utxo{"1cfd000efbe248c48b499b0a5d76ea7687ee76cad8481f71277ee283df32af26", 0, 1250000000, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{9523810};
    const uint64_t fees_amount{830};
    const uint64_t change_amount{1240475360};
    derivation_path change_path{49, 1, 0, 1, 0};
    const std::string to_address{"2N8o4Mu5PRAR27TC2eai62CRXarTbQmjyCx"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 644};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const std::string expected_encoded_tx{"0100000000010126af32df83e27e27711f48d8ca76ee8776ea765d0a9b498bc448e2fb0e00fd1c000000001716001438971f73930f6c141d977ac4fd4a727c854935b3ffffffff02625291000000000017a914aa8f293a04a7df8794b743e14ffb96c2a30a1b2787e026f0490000000017a914251dd11457a259c3ba47e5cca3717fe4214e0298870247304402207eb5d799dea2bd9bbed68f94559ae7447aedb600dd59e68721975868ea16f87e022009f477da2f5755742cf31e125e5f8724d67a10cfbe5b370d7b8eaf06baf694ca012103a1af804ac108a8a51782198c2d034b28bf90c8803f5a53f76276fa69a4eae77f84020000"};
    const std::string expected_txid{"590d24fe55b7d96f25513aad79df3eb267f6dadef28d6448d3023eaec42cbe98"};
    const std::string expected_change_address{"2MvdUi5o3f2tnEFh9yGvta6FzptTZtkPJC8"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_hardened_coin(), 0x80000001);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_change(), 1);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 0);
}

BOOST_AUTO_TEST_CASE(transaction_builder__send_to_native_segwit__builds_properly)
{
    base_coin coin{coin_derivation_purpose::BIP49, coin_derivation_coin::MainNet};
    derivation_path path{49, 0, 0, 0, 80};
    unspent_transaction_output utxo{"94b5bcfbd52a405b291d906e636c8e133407e68a75b0a1ccc492e131ff5d8f90", 0, 10261, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{5000};
    const uint64_t fees_amount{1000};
    const uint64_t change_amount{4261};
    derivation_path change_path{49, 0, 0, 1, 102};
    const std::string to_address{"bc1ql2sdag2nm9csz4wmlj735jxw88ym3yukyzmrpj"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 500000};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const std::string expected_encoded_tx{"01000000000101908f5dff31e192c4cca1b0758ae60734138e6c636e901d295b402ad5fbbcb594000000001716001442288ee31111f7187e8cfe8c82917c4734da4c2effffffff028813000000000000160014faa0dea153d9710155dbfcbd1a48ce39c9b89396a51000000000000017a914aa71651e8f7c618a4576873254ec80c4dfaa068b8702483045022100fc142e1aa34627b880363427e07fc8de82542eba5593030160fbc33d22101c4302207478c3407a15daf613f458eb32223fb6d89a62b93b1a701c404a1a2f3977aee701210270d4003d27b5340df1895ef3a5aee2ae2fe3ed7383c01ba623723e702b6c83c120a10700"};
    const std::string expected_txid{"ff3033d6f7029ec366a9fe146d9941dddaa6edb3cd6543a6d285f84c5d4d22c3"};
    const std::string expected_change_address{"3HEEdyeVwoGZf86jq8ovUhw9FiXkwCdY79"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 102);
}

BOOST_AUTO_TEST_CASE(transaction_builder__from_native_segwit__to_native_segwit)
{
    base_coin coin{coin_derivation_purpose::BIP84, coin_derivation_coin::MainNet};
    derivation_path path{84, 0, 0, 0, 1};
    unspent_transaction_output utxo{"a89a9bed1f2daca01a0dca58f7fd0f2f0bf114d762b38e65845c5d1489339a69", 0, 96537, path, true};
    std::vector<unspent_transaction_output> utxos{utxo};
    const uint64_t amount{9755};
    const uint64_t fees_amount{846};
    const uint64_t change_amount{85936};
    derivation_path change_path{84, 0, 0, 1, 1};
    const std::string to_address{"bc1qjv79zewlvyyyd5y0qfk3svexzrqnammllj7mw6"};
    transaction_data tx_data{to_address, coin, utxos, amount, fees_amount, change_amount, change_path, 590582};

    const auto master_private_key{private_key_for(test_only_words, coin)};
    transaction_builder builder{master_private_key, coin};

    const auto expected_encoded_tx{"01000000000101699a3389145d5c84658eb362d714f10b2f0ffdf758ca0d1aa0ac2d1fed9b9aa80000000000ffffffff021b26000000000000160014933c5165df610846d08f026d18332610c13eef7fb04f0100000000001600144227d834f1aae95273f0c87495f4ff0cb36654520247304402206dff721cc1aacc56bc9d41ed48bdcbae277d184112b90e7db175e452113f049b0220243e3f4006ccd6bf8a20cc5d95c2ab6e949a7bc2e3c299b9bfc241615431deb3012103e775fd51f0dfb8cd865d9ff1cca2a158cf651fe997fdc9fee9c1d3b5e995ea77f6020900"};
    const auto expected_txid{"dff5b92552f363dffbdd680737647a3c9b6a0b0a0e1fe1d8e26f9d66f07e5126"};
    const auto expected_change_address{"bc1qggnasd834t54yulsep6fta8lpjekv4zj6gv5rf"};

    auto tx_metadata{builder.generate_tx_metadata(tx_data)};
    auto encoded_tx{tx_metadata.get_encoded_tx()};
    auto txid{tx_metadata.get_txid()};
    auto change_address{tx_metadata.get_change_address()};

    BOOST_REQUIRE_EQUAL(tx_data.payment_address, to_address);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_change_address(), expected_change_address);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_purpose(), 84);
    BOOST_REQUIRE_EQUAL(encoded_tx, expected_encoded_tx);
    BOOST_REQUIRE_EQUAL(txid, expected_txid);
    BOOST_REQUIRE_EQUAL(*tx_metadata.get_vout_index(), 1);
    BOOST_REQUIRE_EQUAL(tx_metadata.get_change_path()->get_index(), 1);
}

BOOST_AUTO_TEST_SUITE_END()