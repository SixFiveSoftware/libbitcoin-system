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

#ifndef COINNINJA_TRANSACTION_DATA_HPP
#define COINNINJA_TRANSACTION_DATA_HPP

#include <string>
#include <vector>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/unspent_transaction_output.hpp>

namespace coinninja {
namespace transaction {

/**
 * replaceability_option
 * 
 * Provides intention of Replace-By-Fee (RBF) behavior for the outgoing transaction.
 * 
 * must_be_rbf The transaction inputs must be set to a sequence lower than 0xFFFFFFFE.
 * must_not_be_rbf The transaction inputs must be set to a sequence greater than 0xFFFFFFFE.
 * allowed The transaction inputs will be set to not be RBF, iff the parent utxo is unconfirmed.
 */
enum replaceability_option {
    must_be_rbf = 0,
    must_not_be_rbf = 1,
    allowed = 2
};

class transaction_data {
public:
    /**
     * constructors
     */
    transaction_data();

    // memberwise initializer
    transaction_data(std::string payment_address,
        coinninja::wallet::base_coin coin,
        std::vector<coinninja::transaction::unspent_transaction_output> unspent_transaction_outputs,
        uint64_t amount,
        uint64_t fee_amount,
        uint64_t change_amount,
        coinninja::wallet::derivation_path change_path,
        uint64_t block_height,
        replaceability_option replaceability_option = coinninja::transaction::replaceability_option::must_not_be_rbf
        );
    
    // static creation methods
    /**
     * Create transaction data object using a fee rate, calculating fee via number of inputs and outputs.
     * 
     * @param data Reference to value created by this method. Caller must store in locally-kept value.
     * @param payment_address Outgoing payment address as std::string
     * @param coin Coin object represting current user's wallet
     * @param all_unspent_transaction_outputs A vector of all available UTXOs, those needed will be selected during construction
     * @param amount The amount which you would like to send to the recipient
     * @param fee_rate The fee rate to be multiplied by transaction size
     * @param change_path The derivation path for receiving change, if any
     * @param block_height The current block height, used to calculate locktime (block_height + 1)
     * @param replaceability_option A value passed to the transaction builder later. Default value is allowed, allowing rbf to be determined from utxo confirmation. Lightning funding should always use `must_not_be_rbf`.
     * @return True with a populated reference as the first parameter if the object is able to satisfy amount+fee with UTXOs, or false if insufficient funds
     */
    static bool create_transaction_data(coinninja::transaction::transaction_data &data,
        std::string payment_address, 
        coinninja::wallet::base_coin coin,
        std::vector<coinninja::transaction::unspent_transaction_output> all_unspent_transaction_outputs,
        uint64_t amount,
        uint16_t fee_rate,
        coinninja::wallet::derivation_path change_path,
        uint64_t block_height,
        replaceability_option replaceability_option = coinninja::transaction::replaceability_option::allowed
        );

    /**
     * Create transaction data object with a flat fee, versus calculated via number of inputs/outputs.
     * 
     * @param paymentAddress The address to which you want to send currency.
     * @param coin The coin representing the current user's wallet.
     * @param all_unspent_transaction_outputs An array of all available UTXOs, which will be selected by this method.
     * @param amount The amount which you would like to send to the receipient.
     * @param flat_fee The flat-fee to pay, NOT a rate. This fee, added to amount, will equal the total deducted from the wallet.
     * @param change_path The derivation path for receiving change, if any.
     * @param block_height The current block height, used to calculate the locktime (blockHeight + 1).
     * @return True with a populated reference as the first parameter if the object is able to satisfy amount+fee with UTXOs, or false if insufficient funds
     */
    static bool create_flat_fee_transaction_data(coinninja::transaction::transaction_data &data,
        std::string payment_address,
        coinninja::wallet::base_coin coin,
        std::vector<coinninja::transaction::unspent_transaction_output>all_unspent_transaction_outputs,
        uint64_t amount,
        uint64_t flat_fee,
        coinninja::wallet::derivation_path change_path,
        uint64_t block_height
    );

    /**
     * Create transaction data object by sending max to destinationa address.
     * 
     * @param all_unspent_transaction_outputs An array of all available UTXOs, which will be selected by this method.
     * @param coin The coin representing the current user's wallet.
     * @param paymentAddress The address to which you want to send currency.
     * @param fee_rate The fee rate to be multiplied by transaction size
     * @param block_height The current block height, used to calculate the locktime (blockHeight + 1).
     * @return True with a populated reference as the first parameter if the object is able to satisfy amount+fee with UTXOs, or false if insufficient funds
     */
    static bool create_send_max_transaction_data(coinninja::transaction::transaction_data &data,
        std::vector<coinninja::transaction::unspent_transaction_output> all_unspent_transaction_outputs,
        coinninja::wallet::base_coin coin,
        std::string payment_address,
        uint16_t fee_rate,
        uint64_t block_height
    );

    /**
     * Members
     */
    std::string payment_address{};
    std::vector<coinninja::transaction::unspent_transaction_output> unspent_transaction_outputs{};
    uint64_t amount{};
    uint64_t fee_amount{};
    uint64_t change_amount{};
    coinninja::wallet::derivation_path change_path{};
    uint64_t locktime{};

    /**
     * Member methods
     */
    replaceability_option get_rbf_replaceability_option() const;
    uint32_t get_suggested_sequence() const;
    bool should_add_change_to_transaction() const;
    coinninja::wallet::base_coin get_coin();

private:
    coinninja::wallet::base_coin coin;
    replaceability_option rbf_replaceability_option;
    static uint16_t dust_threshold();
    static uint16_t byte_estimate_for(uint8_t input_count, const std::string &payment_address, bool include_change_output, const coinninja::wallet::base_coin &coin);
};

} // namespace transaction
} // namespace coinninja

#endif