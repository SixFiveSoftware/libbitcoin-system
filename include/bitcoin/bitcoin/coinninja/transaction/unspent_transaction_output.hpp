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

#ifndef COINNINJA_UNSPENT_TRANSACTION_OUTPUT_HPP
#define COINNINJA_UNSPENT_TRANSACTION_OUTPUT_HPP

#include <string>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>

namespace coinninja {
namespace transaction {

class unspent_transaction_output {
public:

    /**
     * Constructors
     */
    unspent_transaction_output(std::string txid, uint32_t index, uint64_t amount, coinninja::wallet::derivation_path path, bool is_confirmed);

    /**
     * Members
     */
    std::string txid;
    uint32_t index;
    uint64_t amount;
    coinninja::wallet::derivation_path path;
    bool is_confirmed;

};

} // namespace transaction
} // namespace coinninja

#endif