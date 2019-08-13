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

#ifndef COINNINJA_TRANSACTION_METADATA_HPP
#define COINNINJA_TRANSACTION_METADATA_HPP

#include <string>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>

namespace coinninja {
namespace transaction {

class transaction_metadata {
public:
    transaction_metadata(
        std::string txid, 
        std::string encoded_tx, 
        std::string *change_address = nullptr, 
        coinninja::wallet::derivation_path *change_path = nullptr,
        uint *vout_index = nullptr);

    // copy constructor
    transaction_metadata(const transaction_metadata &source);

    ~transaction_metadata();

    std::string get_txid();
    std::string get_encoded_tx();
    std::string *get_change_address();
    coinninja::wallet::derivation_path *get_change_path();
    uint *get_vout_index();

private:
    std::string txid;
    std::string encoded_tx;
    std::string *change_address{nullptr};
    coinninja::wallet::derivation_path *change_path{nullptr};
    uint *vout_index{nullptr};
};

} // namespace transaction
} // namespace coinninja

#endif