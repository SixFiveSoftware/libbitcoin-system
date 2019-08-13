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

#include <bitcoin/bitcoin/coinninja/transaction/transaction_metadata.hpp>

namespace coinninja {
namespace transaction {

transaction_metadata::transaction_metadata(
    std::string txid, 
    std::string encoded_tx, 
    std::string *change_address, 
    coinninja::wallet::derivation_path *change_path, 
    uint *vout_index)
    : txid{txid}, encoded_tx{encoded_tx}, change_address{change_address}, change_path{change_path}, vout_index{vout_index}
{ }

transaction_metadata::~transaction_metadata()
{
    delete change_address;
    delete change_path;
    delete vout_index;
}

std::string transaction_metadata::get_txid() { return txid; }
std::string transaction_metadata::get_encoded_tx() { return encoded_tx; }
std::string* transaction_metadata::get_change_address() { return change_address; }
coinninja::wallet::derivation_path* transaction_metadata::get_change_path() { return change_path; }
uint* transaction_metadata::get_vout_index() { return vout_index; }

} // namespace transaction
} // namespace coinninja
