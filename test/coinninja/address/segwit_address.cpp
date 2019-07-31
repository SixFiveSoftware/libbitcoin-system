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
#include <bitcoin/bitcoin/coinninja/address/segwit_address.hpp>
#include <bitcoin/bitcoin/coinninja/address/bech32.hpp>
#include <string>
#include <vector>

static const std::vector<std::string> valid_checksum = {
  "A12UEL5L",
  "an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1tt5tgs",
  "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw",
  "11qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqc8247j",
  "split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w"
};

static const std::vector<std::string> invalid_checksum = {
  " 1nwldj5",
  "\x7f""1axkwrx",
  "an84characterslonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1569pvx",
  "pzry9x0s0muk",
  "1pzry9x0s0muk",
  "x1b4n0q5v",
  "li1dgmt3",
  "de1lg7wt\xff"
};

struct valid_address_data {
  std::string address;
  size_t scriptPubKeyLen;
  uint8_t scriptPubKey[42];
};

struct invalid_address_data {
  std::string hrp;
  int version;
  size_t program_length;
};

static const std::vector<valid_address_data> valid_address = {
  {
    "BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4",
    22, {
      0x00, 0x14, 0x75, 0x1e, 0x76, 0xe8, 0x19, 0x91, 0x96, 0xd4, 0x54,
      0x94, 0x1c, 0x45, 0xd1, 0xb3, 0xa3, 0x23, 0xf1, 0x43, 0x3b, 0xd6
    }
  },
  {
    "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7",
    34, {
      0x00, 0x20, 0x18, 0x63, 0x14, 0x3c, 0x14, 0xc5, 0x16, 0x68, 0x04,
      0xbd, 0x19, 0x20, 0x33, 0x56, 0xda, 0x13, 0x6c, 0x98, 0x56, 0x78,
      0xcd, 0x4d, 0x27, 0xa1, 0xb8, 0xc6, 0x32, 0x96, 0x04, 0x90, 0x32,
      0x62
    }
  },
  {
    "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7k7grplx",
    42, {
      0x81, 0x28, 0x75, 0x1e, 0x76, 0xe8, 0x19, 0x91, 0x96, 0xd4, 0x54,
      0x94, 0x1c, 0x45, 0xd1, 0xb3, 0xa3, 0x23, 0xf1, 0x43, 0x3b, 0xd6,
      0x75, 0x1e, 0x76, 0xe8, 0x19, 0x91, 0x96, 0xd4, 0x54, 0x94, 0x1c,
      0x45, 0xd1, 0xb3, 0xa3, 0x23, 0xf1, 0x43, 0x3b, 0xd6
    }
  },
  {
    "BC1SW50QA3JX3S",
    4, {
      0x90, 0x02, 0x75, 0x1e
    }
  },
  {
    "bc1zw508d6qejxtdg4y5r3zarvaryvg6kdaj",
    18, {
      0x82, 0x10, 0x75, 0x1e, 0x76, 0xe8, 0x19, 0x91, 0x96, 0xd4, 0x54,
      0x94, 0x1c, 0x45, 0xd1, 0xb3, 0xa3, 0x23
    }
  },
  {
    "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
    34, {
      0x00, 0x20, 0x00, 0x00, 0x00, 0xc4, 0xa5, 0xca, 0xd4, 0x62, 0x21,
      0xb2, 0xa1, 0x87, 0x90, 0x5e, 0x52, 0x66, 0x36, 0x2b, 0x99, 0xd5,
      0xe9, 0x1c, 0x6c, 0xe2, 0x4d, 0x16, 0x5d, 0xab, 0x93, 0xe8, 0x64,
      0x33
    }
  }
};

static std::vector<std::string> invalid_address = {
  "tc1qw508d6qejxtdg4y5r3zarvary0c5xw7kg3g4ty",
  "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t5",
  "BC13W508D6QEJXTDG4Y5R3ZARVARY0C5XW7KN40WF2",
  "bc1rw5uspcuh",
  "bc10w508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kw5rljs90",
  "BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P",
  "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sL5k7",
  "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du",
  "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3pjxtptv",
  "bc1gmk9yu"
};

static const std::vector<invalid_address_data> invalid_address_enc = {
  {"BC", 0, 20},
  {"bc", 0, 21},
  {"bc", 17, 32},
  {"bc", 1, 1},
  {"bc", 16, 41}
};

bool case_insensitive_equal(const std::string& s1, const std::string& s2) {
    size_t i = 0;
    if (s1.size() != s2.size()) return false;
    while (i < s1.size() && i < s2.size()) {
        char c1 = s1[i];
        char c2 = s2[i];
        if (c1 >= 'A' && c1 <= 'Z') c1 = (c1 - 'A') + 'a';
        if (c2 >= 'A' && c2 <= 'Z') c2 = (c2 - 'A') + 'a';
        if (c1 != c2) return false;
        ++i;
    }
    return true;
}

std::vector<uint8_t> segwit_script_pubkey(int witver, std::vector<uint8_t>& witprog) {
    std::vector<uint8_t> script_pubkey{};
    uint8_t witver_byte = (witver ? (0x80 | witver) : 0);
    script_pubkey.push_back(witver_byte);
    script_pubkey.push_back(witprog.size());
    script_pubkey.insert(script_pubkey.end(), witprog.begin(), witprog.end());
    return script_pubkey;
}

BOOST_AUTO_TEST_SUITE(cn_segwit_address_tests)

BOOST_AUTO_TEST_CASE(segwit__valid_checksums)
{
    for (auto checksum: valid_checksum)
    {
        const auto dec = coinninja::address::bech32::decode(checksum);
        BOOST_REQUIRE_NE(dec.first, "");

        const std::string recode = coinninja::address::bech32::encode(dec.first, dec.second);
        const bool recode_equal{case_insensitive_equal(recode, checksum)};
        BOOST_REQUIRE(recode_equal);
    }
}

BOOST_AUTO_TEST_CASE(segwit__invalid_checksums)
{
    for (auto checksum: invalid_checksum)
    {
        const auto dec = coinninja::address::bech32::decode(checksum);
        BOOST_REQUIRE_EQUAL(dec.first, "");
        BOOST_REQUIRE_EQUAL(dec.second.size(), 0);
    }
}

BOOST_AUTO_TEST_CASE(segwit__valid_addresses)
{
    for (auto address: valid_address)
    {
        std::string hrp = "bc";
        auto dec = coinninja::address::segwit_address::decode(hrp, address.address);

        if (dec.first == -1) {
            hrp = "tb";
            dec = coinninja::address::segwit_address::decode(hrp, address.address);
        }

        BOOST_REQUIRE_NE(dec.first, -1); // -1 means failure to decode

        const bc::data_chunk spk{segwit_script_pubkey(dec.first, dec.second)};
        const bool ok = ((spk.size() == address.scriptPubKeyLen) && (memcmp(&spk.at(0), address.scriptPubKey, spk.size()) == 0));
        BOOST_REQUIRE(ok);

        const std::string recode = coinninja::address::segwit_address::encode(hrp, dec.first, dec.second);
        BOOST_REQUIRE_NE(recode, "");

        const bool recode_equal = case_insensitive_equal(recode, address.address);
        BOOST_REQUIRE(recode_equal);
    }
}

BOOST_AUTO_TEST_CASE(segwit__invalid_addresses)
{
    for (auto address: invalid_address)
    {
        const std::string hrp = "bc";
        const auto dec = coinninja::address::segwit_address::decode(hrp, address);
        BOOST_REQUIRE_EQUAL(dec.first, -1); // -1 means failure to decode

        const std::string tbhrp = "tb";
        const auto tbdec = coinninja::address::segwit_address::decode(tbhrp, address);
        BOOST_REQUIRE_EQUAL(dec.first, -1); // -1 means failure to decode

        for (auto data: invalid_address_enc)
        {
            const std::string enc_hrp = data.hrp;
            const auto bytes = bc::data_chunk(data.program_length, 0);  // empty bytes
            const std::string code = coinninja::address::segwit_address::encode(enc_hrp, data.version, bytes);
            BOOST_REQUIRE_EQUAL(code, "");
        }
    }
}

BOOST_AUTO_TEST_CASE(segwit__p2wsh_address__decodes_properly)
{
    const std::string p2wsh_address{"bc1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3qccfmv3"};
    const int expected_version{0};
    const std::string expected_program{"1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262"};
    const int expected_program_size{32};
    const std::string hrp{"bc"};

    const auto dec = coinninja::address::segwit_address::decode(hrp, p2wsh_address);

    BOOST_REQUIRE_EQUAL(expected_version, dec.first);
    BOOST_REQUIRE_EQUAL(expected_program, bc::encode_base16(dec.second));
    BOOST_REQUIRE_EQUAL(expected_program_size, dec.second.size());
}

BOOST_AUTO_TEST_SUITE_END()