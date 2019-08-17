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

#include <src/coinninja/address/address_helper.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/transaction_builder.hpp>
#include <bitcoin/bitcoin/coinninja/address/segwit_address.hpp>
#include <bitcoin/bitcoin/coinninja/address/usable_address.hpp>

using namespace coinninja::wallet;

namespace coinninja {
namespace transaction {

transaction_builder::transaction_builder(bc::wallet::hd_private master_private_key, coinninja::wallet::base_coin coin)
    : master_private_key{master_private_key}, coin{coin}
{ } 

coinninja::transaction::transaction_metadata transaction_builder::generate_tx_metadata(const coinninja::transaction::transaction_data &data)
{
    // create tx
    auto transaction = transaction_from_data(data);

    // encode tx
    std::string encoded_tx{bc::encode_base16(transaction.to_data(true, true))};

    // get txid
    std::string txid{bc::encode_hash(transaction.hash())};

    // return metadata
    if (data.should_add_change_to_transaction())
    {
        coinninja::address::usable_address change{master_private_key, data.change_path};
        auto change_payment_address{change.build_payment_address()};
        auto change_address{change_payment_address.encoded()};
        auto change_path{data.change_path};
        uint vout_index{0};

        using namespace bc::wallet;
        auto pkver = (coin.get_coin() == 0) ? payment_address::mainnet_p2kh : payment_address::testnet_p2kh;
        auto shver = (coin.get_coin() == 0) ? payment_address::mainnet_p2sh : payment_address::testnet_p2sh;

        for (size_t i{0}; i < transaction.outputs().size(); ++i)
        {
            auto output = transaction.outputs().at(i);
            std::string possible_change_address{output.address(pkver, shver).encoded()};
            if (possible_change_address == change_address)
            {
                vout_index = static_cast<uint>(i);
            }
        }
        return {txid, encoded_tx, change_address, change_path, vout_index};
    } else {
        return {txid, encoded_tx};
    }
}

// private
bc::chain::transaction transaction_builder::transaction_from_data(const coinninja::transaction::transaction_data &data)
{
    // create transaction
    bc::chain::transaction transaction{};
    transaction.set_version(1u);

    // populate transaction with payment data
    transaction.outputs().push_back(output_with_address(data.payment_address, data.amount));

    // calculate change
    if (data.should_add_change_to_transaction())
    {
        coinninja::address::usable_address change{master_private_key, data.change_path};
        auto change_address{change.build_payment_address()};
        transaction.outputs().push_back(create_p2sh_output(change_address, data.change_amount));
    }

    populate_utxos(transaction, data);

    transaction.set_locktime(data.locktime);

    sign_inputs(transaction, data);

    return transaction;
}

bc::chain::output transaction_builder::output_with_address(const std::string &address, const uint64_t &amount)
{
    coinninja::address::address_helper helper{coin};
    auto type{helper.address_type_for_address(address)};
    switch (type)
    {
        case coinninja::address::payment_output_type::P2PKH:
        {
            bc::wallet::payment_address payment_address{address};
            return create_p2pkh_output(payment_address, amount);
        }

        case coinninja::address::payment_output_type::P2SH:
        {
            bc::wallet::payment_address payment_address{address};
            return create_p2sh_output(payment_address, amount);
        }

        case coinninja::address::payment_output_type::P2WPKH:
        case coinninja::address::payment_output_type::P2WSH:
        {
            auto net = (coin.get_coin() == 0) ? coin_derivation_coin::MainNet : coin_derivation_coin::TestNet;
            base_coin segwit_coin{coin_derivation_purpose::BIP84, net};
            const std::string hrp{segwit_coin.get_bech32_hrp()};
            return create_segwit_output(address, hrp, amount);
        }
        
        default:
        {
            throw "Illegal payment address";
            break;
        }
    }
}

bc::chain::output transaction_builder::create_p2pkh_output(const bc::wallet::payment_address &payment_address, const uint64_t &amount)
{
    return bc::chain::output(amount, bc::chain::script().to_pay_key_hash_pattern(payment_address.hash()));
}

bc::chain::output transaction_builder::create_p2sh_output(const bc::wallet::payment_address &payment_address, const uint64_t &amount)
{
    return bc::chain::output(amount, bc::chain::script(bc::chain::script().to_pay_script_hash_pattern(payment_address.hash())));
}

bc::chain::output transaction_builder::create_segwit_output(const std::string &address, const std::string &hrp, const uint64_t &amount)
{
    auto decoded = coinninja::address::segwit_address::decode(hrp, address);
    auto witprog{decoded.second};
    return bc::chain::output(amount, create_segwit_hash_pattern(witprog));
}

bc::machine::operation::list transaction_builder::create_segwit_hash_pattern(const bc::data_chunk &hash)
{
    return {
        { bc::machine::opcode::push_size_0 },
        { hash }
    };
}

bc::data_chunk transaction_builder::segwit_script_pubkey(const int &witver, std::vector<uint8_t>& witprog) {
    bc::data_chunk script_pubkey{};
    uint8_t witver_byte = (witver ? (0x80 | witver) : 0);
    script_pubkey.push_back(witver_byte);
    // script_pubkey.push_back(witprog.size());
    script_pubkey.insert(script_pubkey.end(), witprog.begin(), witprog.end());
    return script_pubkey;
}

void transaction_builder::populate_utxos(bc::chain::transaction &tx, const coinninja::transaction::transaction_data &data)
{
    for (auto &utxo : data.unspent_transaction_outputs)
    {
        // for a P2SH(P2WPKH) input
        // previous tx hash
        bc::hash_digest prev_tx_hash;
        bc::decode_hash(prev_tx_hash, utxo.txid);

        // previous utxo index
        auto index{utxo.index};
        
        // build input from previous output
        bc::chain::output_point utxo_to_spend{prev_tx_hash, index};
        bc::chain::input p2sh_p2wpkh_input;
        p2sh_p2wpkh_input.set_previous_output(utxo_to_spend);

        // set sequence
        auto replaceable_seq{bc::max_input_sequence - 2};
        auto non_replaceable_seq{bc::max_input_sequence};

        if (data.get_should_be_rbf()) {
            p2sh_p2wpkh_input.set_sequence(replaceable_seq);
        } else {
            auto seq = (utxo.is_confirmed) ? non_replaceable_seq : replaceable_seq;
            p2sh_p2wpkh_input.set_sequence(seq);
        }

        // add input to tx outputs
        tx.inputs().push_back(p2sh_p2wpkh_input);
    }
}

void transaction_builder::sign_inputs(bc::chain::transaction &tx, const coinninja::transaction::transaction_data &data)
{
    // P2SH(P2WPKH) signing
    for (size_t i{0}; i < data.unspent_transaction_outputs.size(); i++)
    {
        auto utxo{data.unspent_transaction_outputs[i]};
        auto path{utxo.path};
        coinninja::address::usable_address signer{master_private_key, path};

        // set scriptPubKey, per BIP 141.
        auto script_code = bc::chain::script::to_pay_key_hash_pattern(bc::bitcoin_short_hash(signer.build_compressed_public_key()));

        bc::endorsement signature;
        auto secret = signer.build_index_private_key().secret();
        bc::chain::script::create_endorsement(
            signature, 
            secret, 
            script_code, 
            tx, 
            i, 
            bc::machine::sighash_algorithm::all, 
            bc::machine::script_version::zero, 
            utxo.amount);

        if (path.get_purpose() == 49)
        {
            // add scriptSig for P2SH-P2WPKH. Not needed for native SegWit
            bc::data_chunk script_chunk = bc::to_chunk(signer.build_p2wpkh_script().to_data(true));
            tx.inputs()[i].set_script(bc::chain::script(script_chunk, false));
        }
        bc::data_stack witness_data{bc::to_chunk(signature), bc::to_chunk(signer.build_compressed_public_key())};
        tx.inputs()[i].set_witness(bc::chain::witness(witness_data));
    }
}

} // namespace transaction
} // namespace coinninja
