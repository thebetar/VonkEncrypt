#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdint>

using namespace std;

#define MAX_TEXT_SIZE 32
#define SALT_ROUNDS 4
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
                // Convert to ASCII (a number between 32 and 126)
                char c = text[i];
                // Shift down to 0
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

        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            for (int j = 0; j < SALT_ROUNDS; j++)
            {
                // XOR with semi-random value
                int semi_random = semi_random_pi(i + key + randomiser);
                ascii_values[i] = ascii_values[i] + semi_random;
                // Keep value within printable ASCII range for whitespace
                ascii_values[i] = ascii_values[i] % 94;
                // Update randomiser
                randomiser = (randomiser + semi_random) % 94;
            }
        }

        string encrypted_text = "";

        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            // Convert back to char
            char c = ascii_values[i];
            // Shift back to ASCII range
            c += 32;
            // Add to encrypted text
            encrypted_text.push_back(c);
        }

        return encrypted_text;
    }

    string decrypt(string encrypted_text, int key)
    {
        vector<int> ascii_values;

        // Convert and pad input
        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            if (i < encrypted_text.size())
            {
                // Convert to ASCII (a number between 32 and 126)
                char c = encrypted_text[i];
                // Shift down to 0
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

        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            for (int j = 0; j < SALT_ROUNDS; j++)
            {
                // XOR with semi-random value
                int semi_random = semi_random_pi(i + key + randomiser);
                // Fix: Handle negative numbers correctly with modulo
                ascii_values[i] = ((ascii_values[i] - semi_random) + 94) % 94;
                // Update randomiser
                randomiser = (randomiser + semi_random) % 94;
            }
        }

        string decrypted_text = "";

        for (int i = 0; i < MAX_TEXT_SIZE; i++)
        {
            // Convert back to char
            char c = ascii_values[i];
            // Shift back to ASCII range
            c += 32;
            // Add to encrypted text
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

    // RSA key generation
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