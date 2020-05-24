#include <iostream>
#include <sstream>
#include <assert.h>
#include <string>
#include "aes.hpp"
using namespace std;
using namespace crypto;


auto aes = new Aes(AesKeyWidth::AES_128);

void print_vlist(vector<u8>& vector, string seperator) {
    for (auto it = vector.cbegin(); it != vector.cend(); it++) {
        cout << "0x" << hex << (uint)*it;
        if ( it+1 != vector.cend() ) {
            cout << seperator;
        }
    }
}
void test_circular_shift() {
    vector<u8> tested = {0x67, 0x20, 0x46, 0x75};
    vector<u8> expected = {0x20, 0x46, 0x75, 0x67};
    aes->circular_byte_left_shift(tested, 0, 3);

    cout << "[" << __FUNCTION__ << "] " << "Tested: ";
    print_vlist(tested, " ");
    cout <<" | Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
}

void test_byte_substitution() {
    vector<u8> tested = {0x20, 0x46, 0x75, 0x67};
    vector<u8> expected = {0xB7, 0x5A, 0x9D, 0x85};
    aes->byte_substitution(tested, 0, 3);
    
    cout << "[" << __FUNCTION__ << "] " << "Tested: ";
    print_vlist(tested, " ");
    cout <<" | Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
}

void test_add_round_constant() {
    u8 tested = 0xB7;
    u8 expected = 0xB6;
    aes->add_round_constant(tested, 0);
    
    cout << "["<< __FUNCTION__ <<"] "
        << "Tested: 0x" << std::hex <<  (uint)tested << " | Expected: 0x" << std::hex << (uint)expected << "\n";
    assert(tested == expected);
}

void test_operation_g() {
    vector<u8> state = {0x67, 0x20, 0x46, 0x75};
    vector<u8> expected = {0xB6, 0x5A, 0x9D, 0x85};
    vector<u8> tested = aes->operation_g(state, 0, 3, 0);

    cout << "[" << __FUNCTION__ << "] " << "Tested: ";
    print_vlist(tested, " ");
    cout <<" | Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
}

void test_generate_next_roundkey() {
    vector<u8> start_state = { 0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75 };
    vector<u8> tested = aes->generate_next_roundkey(0, start_state);
    vector<u8> expected =    { 0xE2, 0x32, 0xFC, 0xF1, 0x91, 0x12, 0x91, 0x88, 0xB1, 0x59, 0xE4, 0xE6, 0xD6, 0x79, 0xA2, 0x93 };

    cout << "[" << __FUNCTION__ << "] " <<   "Tested:   ";
    print_vlist(tested, " ");
    cout << "\n[" << __FUNCTION__ << "] " << "Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
} 



void test_apply_round_to_state() {
    vector<u8> tested = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
    vector<u8> tested_round_key = { 0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75 };
    vector<u8> expected = {0x00, 0x1f, 0x0e, 0x54, 0x3c, 0x4e, 0x08, 0x59, 0x6e, 0x22, 0x1b, 0x0b, 0x47, 0x74, 0x31, 0x1a};

    aes->add_round_key_to_state(tested_round_key, tested);
    

    cout << "[" << __FUNCTION__ << "] " <<   "Tested:   ";
    print_vlist(tested, " ");
    cout << "\n[" << __FUNCTION__ << "] " << "Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
}

void test_mix_columns() {
    vector<u8> tested =   {0x63, 0x2f, 0xaf, 0xa2, 0xeb, 0x93, 0xc7, 0x20, 0x9f, 0x92, 0xab, 0xcb, 0xa0, 0xc0, 0x30, 0x2b};
    vector<u8> expected = {0xba, 0x75, 0xf4, 0x7a, 0x84, 0xa4, 0x8d, 0x32, 0xe8, 0x8d, 0x06, 0x0e, 0x1b, 0x40, 0x7d, 0x5d};

    aes->mix_columns(tested);
    

    cout << "[" << __FUNCTION__ << "] " <<   "Tested:   ";
    print_vlist(tested, " ");
    cout << "\n[" << __FUNCTION__ << "] " << "Expected: ";
    print_vlist(expected, " ");
    cout << "\n";
    assert(tested == expected);
}

void test_full_round() {
    vector<u8> message =     { 0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F };
    vector<u8> key = { 0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75 };
    

    aes->add_round_key_to_state(key, message);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Add round key     :   ";
    print_vlist(message, " ");

    aes->byte_substitution(message, 0, message.size() - 1);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Byte substitution :   ";
    print_vlist(message, " ");

    aes->shift_rows(message);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Shift rows        :   ";
    print_vlist(message, " ");


    aes->mix_columns(message);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Mix columns       :   ";
    print_vlist(message, " ");

    vector<u8> first_round_key = aes->generate_next_roundkey(0, key);
    aes->add_round_key_to_state(first_round_key, message);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Add round key     :   ";
    print_vlist(message, " ");

}

void test_full_cycle() {
    vector<u8> message =     { 0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F };
    vector<u8> key = { 0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75 };
    
    vector<u8> result = aes->encrypt(key, message);

    cout << "\n[" << __FUNCTION__ << "] " <<   "Final output      :   ";
    print_vlist(result, " ");
    cout <<"\n";

}



int main(int argc, char** argv) {
    test_circular_shift();
    test_byte_substitution();
    test_add_round_constant();
    test_operation_g();
    test_generate_next_roundkey();
    test_apply_round_to_state();
    test_mix_columns();
    test_full_round();
    test_full_cycle();
}