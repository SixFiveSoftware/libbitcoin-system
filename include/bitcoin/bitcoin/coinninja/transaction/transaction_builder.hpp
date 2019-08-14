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

#ifndef COINNINJA_TRANSACTION_BUILDER_HPP
#define COINNINJA_TRANSACTION_BUILDER_HPP

#include <string>
#include <vector>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/transaction_metadata.hpp>
#include <bitcoin/bitcoin/coinninja/transaction/transaction_data.hpp>

namespace coinninja {
namespace transaction {

class transaction_builder {
public:
    /**
     * Constructors
     */
    transaction_builder(bc::wallet::hd_private master_private_key, coinninja::wallet::base_coin coin);

    /**
     * Member methods
     */
    coinninja::transaction::transaction_metadata generate_tx_metadata(const coinninja::transaction::transaction_data &tx_data);

private:
    bc::wallet::hd_private master_private_key;
    coinninja::wallet::base_coin coin;

    bc::chain::transaction transaction_from_data(const coinninja::transaction::transaction_data &data);
    bc::chain::output output_with_address(const std::string &address, const uint64_t &amount);
    bc::chain::output create_p2pkh_output(const bc::wallet::payment_address &payment_address, const uint64_t &amount);
    bc::chain::output create_p2sh_output(const bc::wallet::payment_address &payment_address, const uint64_t &amount);
    bc::chain::output create_segwit_output(const std::string &address, const uint64_t &amount);
    bc::machine::operation::list create_segwit_hash_pattern(const bc::data_chunk &hash);
    void populate_utxos(bc::chain::transaction &tx, const coinninja::transaction::transaction_data &data);
    void sign_inputs(bc::chain::transaction &tx, const coinninja::transaction::transaction_data &data);
};

} // namespace transaction
} // namespace coinninja

#endif