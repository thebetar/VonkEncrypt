#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdint>

using namespace std;

#define MAX_TEXT_SIZE 64
#define SALT_ROUNDS 8
#define RANDOM_STRING_LENGTH 10000

class SymmetricEncryption
{
public:
    string encrypt(string text, int key)
    {
        vector<int> ascii_values;

        // Convert and pad input
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            if (i < text.size())
            {
                // Get char
                char c = text[i];
                // Shift to 0-94 range
                c -= 32;
                // Add to vector
                ascii_values.push_back(c);
            }
            else
            {
                ascii_values.push_back(0);
            }
        }

        int randomiser = key;

        // XOR encryption
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            for (int j = 0; j < SALT_ROUNDS; j++)
            {
                int semi_random = semi_random_pi(i + key + randomiser) % 94;
                ascii_values[i] = ascii_values[i] ^ semi_random;
                randomiser = (randomiser ^ semi_random) % 94;
            }
        }

        string encrypted_text = "";
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            // Get char
            char c = ascii_values[i];
            // Shift back to printable ASCII
            c += 32;
            // Add to string
            encrypted_text.push_back(c);
        }

        return encrypted_text;
    }

    string decrypt(string encrypted_text, int key)
    {
        vector<int> ascii_values;

        // Convert input
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            if (i < encrypted_text.size())
            {
                // Get char
                char c = encrypted_text[i];
                // Shift to 0-94 range
                c -= 32;
                // Add to vector
                ascii_values.push_back(c);
            }
            else
            {
                ascii_values.push_back(0);
            }
        }

        int randomiser = key;

        // XOR decryption (same operation as encryption)
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            for (int j = 0; j < SALT_ROUNDS; j++)
            {
                int semi_random = semi_random_pi(i + key + randomiser) % 94;
                ascii_values[i] = ascii_values[i] ^ semi_random;
                randomiser = (randomiser ^ semi_random) % 94;
            }
        }

        string decrypted_text = "";
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            // Get char
            char c = ascii_values[i];
            // Shift back to printable ASCII
            c += 32;
            // Add to string
            decrypted_text.push_back(c);
        }

        return decrypted_text;
    }

    void set_random_values(vector<char> values)
    {
        this->random_values = values;
    }

private:
    vector<char> random_values;
    int semi_random_pi(int n)
    {
        return random_values[n % random_values.size()];
    }
};

int main()
{
    SymmetricEncryption symmetric_encryption;

    // Load pi values
    ifstream pi_file("pi.txt");
    string pi_string;
    vector<char> random_values;

    if (pi_file.is_open())
    {
        getline(pi_file, pi_string);

        for (int i = 0; i < RANDOM_STRING_LENGTH; i++)
        {
            random_values.push_back(pi_string[i]);
        }
    }

    symmetric_encryption.set_random_values(random_values);

    // Encryption key
    int encryption_key = 0x060899;

    // Get input
    string text;

    printf("Enter text: ");
    getline(cin, text);

    // Check input length
    if (text.size() > MAX_TEXT_SIZE)
    {
        printf("Text is too long\n");
        return 1;
    }

    // Encrypt text
    string encrypted_text = symmetric_encryption.encrypt(text, encryption_key);

    // Decrypt text
    string decrypted_text = symmetric_encryption.decrypt(encrypted_text, encryption_key);

    // Output
    printf("Original text: %s\n", text.c_str());
    printf("Encrypted text: %s\n", encrypted_text.c_str());
    printf("Decrypted text: %s\n", decrypted_text.c_str());
}