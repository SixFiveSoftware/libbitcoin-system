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

#include <bitcoin/bitcoin/coinninja/transaction/transaction_data.hpp>
#include <bitcoin/bitcoin/coinninja/address/address_helper.hpp>

namespace coinninja {
namespace transaction {

/**
 * constructors
 */
transaction_data::transaction_data()
    : coin{coinninja::wallet::base_coin(coinninja::wallet::coin_derivation_purpose::BIP49)}
{ }

// memberwise initializer
transaction_data::transaction_data(std::string payment_address,
    coinninja::wallet::base_coin coin,
    std::vector<coinninja::transaction::unspent_transaction_output> unspent_transaction_outputs,
    uint64_t amount,
    uint64_t fee_amount,
    uint64_t change_amount,
    coinninja::wallet::derivation_path change_path,
    uint64_t block_height,
    // bool should_be_rbf
    replaceability_option rbf_replaceability_option
)
    : payment_address{payment_address},
    coin{coin},
    unspent_transaction_outputs{unspent_transaction_outputs},
    amount{amount},
    fee_amount{fee_amount},
    change_amount{change_amount},
    change_path{change_path},
    locktime{block_height},
    rbf_replaceability_option{rbf_replaceability_option}
    { }

// static creator methods
bool transaction_data::create_transaction_data(coinninja::transaction::transaction_data &data,
    std::string payment_address, 
    coinninja::wallet::base_coin coin,
    std::vector<coinninja::transaction::unspent_transaction_output> all_unspent_transaction_outputs,
    uint64_t amount,
    uint16_t fee_rate,
    coinninja::wallet::derivation_path change_path,
    uint64_t block_height,
    replaceability_option rbf_replaceability_option
)
{
    coinninja::transaction::transaction_data return_data{
        payment_address, coin, {}, amount, 0, 0, change_path, block_height, rbf_replaceability_option
    };

    coinninja::address::address_helper helper{coin};

    std::vector<coinninja::transaction::unspent_transaction_output> required_inputs{};
    uint64_t total_from_utxos{0};
    uint16_t number_of_inputs{0};
    uint64_t total_sending_value{0};
    uint64_t current_fee{0};
    int fee_per_input{fee_rate * helper.bytes_per_input()};

    for (coinninja::transaction::unspent_transaction_output &output : all_unspent_transaction_outputs)
    {
        total_sending_value = amount + current_fee;

        if (total_sending_value > total_from_utxos)
        {
            required_inputs.push_back(output);
            number_of_inputs++;
            total_from_utxos += output.amount;
            uint32_t total_bytes{byte_estimate_for(number_of_inputs, payment_address, false, coin)};
            current_fee = fee_rate * total_bytes;
            total_sending_value = amount + current_fee;

            int change_value = total_from_utxos - total_sending_value;
            if ((total_from_utxos < total_sending_value) || change_value < 0)
            {
                continue;
            }

            if (change_value > 0 && (change_value < (fee_per_input + dust_threshold())))
            {
                // is not beneficial to add change, would just dust self with change
                current_fee += change_value;
                break;
            } else if (change_value > 0)
            {
                total_bytes = byte_estimate_for(number_of_inputs, payment_address, true, coin);
                current_fee = fee_rate * total_bytes;
                change_value = total_from_utxos - amount - current_fee;
                return_data.change_amount = change_value;
                break;
            }
        } else
        {
            break;
        }
    }

    return_data.fee_amount = current_fee;
    return_data.unspent_transaction_outputs = required_inputs;

    if (total_from_utxos < total_sending_value)
    {
        data = transaction_data{};
        return false;
    }

    data = return_data;
    return true;
}

bool transaction_data::create_flat_fee_transaction_data(coinninja::transaction::transaction_data &data,
    std::string payment_address,
    coinninja::wallet::base_coin coin,
    std::vector<coinninja::transaction::unspent_transaction_output>all_unspent_transaction_outputs,
    uint64_t amount,
    uint64_t flat_fee,
    coinninja::wallet::derivation_path change_path,
    uint64_t block_height
)
{
    coinninja::transaction::transaction_data return_data{
        payment_address, coin, {}, amount, flat_fee, 0, change_path, block_height,
        coinninja::transaction::replaceability_option::must_be_rbf
    };

    std::vector<coinninja::transaction::unspent_transaction_output> all_utxos_copy{all_unspent_transaction_outputs};
    std::vector<coinninja::transaction::unspent_transaction_output> outputs_to_use{};

    uint64_t total_from_utxos{0};

    do
    {
        // early exit if insufficient funds
        if (all_utxos_copy.size() == 0)
        {
            data = transaction_data{};
            return false;
        }

        // get a utxo
        coinninja::transaction::unspent_transaction_output output{all_utxos_copy.at(0)};
        all_utxos_copy.erase(all_utxos_copy.begin());
        outputs_to_use.push_back(output);

        total_from_utxos += output.amount;

        int possible_change = static_cast<int>(total_from_utxos) - static_cast<int>(amount) - static_cast<int>(flat_fee);
        int temp_change_amount = std::max(0, possible_change);
        return_data.change_amount = static_cast<uint64_t>(temp_change_amount);

        if (total_from_utxos >= amount && temp_change_amount > 0)
        {
            return_data.change_path = change_path;

            if (return_data.change_amount < dust_threshold())
            {
                return_data.change_amount = 0;
            }
        }

    } while (total_from_utxos < (flat_fee + amount));
    
    return_data.unspent_transaction_outputs = outputs_to_use;
    data = return_data;
    return true;
}

bool transaction_data::create_send_max_transaction_data(coinninja::transaction::transaction_data &data,
    std::vector<coinninja::transaction::unspent_transaction_output> all_unspent_transaction_outputs,
    coinninja::wallet::base_coin coin,
    std::string payment_address,
    uint16_t fee_rate,
    uint64_t block_height
)
{
    coinninja::wallet::derivation_path dummy_change{}; // just used to satisfy parameter; not used as there is no change when sending max.
    coinninja::transaction::transaction_data return_data{
        payment_address, coin, all_unspent_transaction_outputs, 0, 0, 0, dummy_change, block_height,
        coinninja::transaction::replaceability_option::must_not_be_rbf
    };

    coinninja::address::address_helper helper{coin};

    uint64_t total_from_utxos{0};
    for (auto output : all_unspent_transaction_outputs)
    {
        total_from_utxos += output.amount;
    }

    auto input_count{all_unspent_transaction_outputs.size()};
    auto byte_count{helper.total_bytes(input_count, payment_address, false)};
    
    auto fee_amount = fee_rate * byte_count;
    return_data.fee_amount = fee_amount;

    int signed_amount_for_validation{static_cast<int>(total_from_utxos) - static_cast<int>(fee_amount)};
    if (signed_amount_for_validation < 0)
    {
        data = transaction_data{};
        return false;
    } else {
        return_data.amount = total_from_utxos - fee_amount;
    }

    data = return_data;
    return true;
}
    
replaceability_option transaction_data::get_rbf_replaceability_option() const
{
    return rbf_replaceability_option;
}

uint32_t transaction_data::get_suggested_sequence() const
{
    switch (rbf_replaceability_option)
    {
    case replaceability_option::must_be_rbf:
    {
        return bc::max_input_sequence - 2;
    }
    
    case replaceability_option::must_not_be_rbf:
    {
        return bc::max_input_sequence;
    }

    case replaceability_option::allowed:
    {
        bool includes_unconfirmed_utxos{false};
        for (const auto &utxo : unspent_transaction_outputs)
        {
            includes_unconfirmed_utxos = includes_unconfirmed_utxos || !utxo.is_confirmed;
        }
        return includes_unconfirmed_utxos ? bc::max_input_sequence - 2 : bc::max_input_sequence;
    }
    
    default:
        return bc::max_input_sequence;
    }
}

bool transaction_data::should_add_change_to_transaction() const
{
    return (change_amount > 0);
}

uint16_t transaction_data::dust_threshold()
{
    return 1000;
}

uint32_t transaction_data::byte_estimate_for(uint16_t input_count, const std::string &payment_address, bool include_change_output, const coinninja::wallet::base_coin &coin)
{
    coinninja::address::address_helper helper{coin};
    return helper.total_bytes(input_count, payment_address, include_change_output);
}

coinninja::wallet::base_coin transaction_data::get_coin()
{
    return coin;
}

} // namespace transaction
} // namespace coinninja
