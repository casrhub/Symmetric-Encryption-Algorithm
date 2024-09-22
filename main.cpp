#include "crow.h"
#include "crow/utility.h"

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <random>
#include <unordered_map>
#include <sstream>

// General function to convert a string (key or plaintext) to its binary representation
std::string stringToBinary(const std::string& input) {
    std::string binaryString = "";

    // Loop through each character in the input string
    for (char c : input) {
        // Convert the character to its binary representation using bitset
        std::bitset<8> binary(c);  // 8 bits for each character (ASCII)
        binaryString += binary.to_string();  // Append binary string representation
    }

    return binaryString;  // Return the complete binary representation
}

// Define a simple 4-bit S-Box (16 elements for all possible 4-bit inputs)
int s_box[16] = {0xE, 0x4, 0xD, 0x1,
                 0x2, 0xF, 0xB, 0x8,
                 0x3, 0xA, 0x6, 0xC,
                 0x5, 0x9, 0x0, 0x7};

// Define an inverse S-Box
int inverse_s_box[16] = {0xE, 0x3, 0x4, 0x8,
                         0x1, 0xC, 0xA, 0xF,
                         0x7, 0xD, 0x9, 0x6,
                         0xB, 0x2, 0x0, 0x5};
// Step 1 substitution
// Function to substitute a 4-bit binary group using an S-box
std::string applySBox(const std::string& binaryInput, const int s_box[]) {
    std::string result = "";

    // Iterate through each 4-bit block
    for (size_t i = 0; i < binaryInput.length(); i += 4) {
        // Extract the 4-bit block
        std::string fourBitBlock = binaryInput.substr(i, 4);

        // Convert the 4-bit block to an integer
        int blockValue = std::bitset<4>(fourBitBlock).to_ulong();

        // Substitute using the S-Box
        int substitutedValue = s_box[blockValue];

        // Convert the substituted value back to a 4-bit binary string
        result += std::bitset<4>(substitutedValue).to_string();
    }

    return result;  // Return the substituted binary string
}

// Function to generate the inverse of a permutation table
std::vector<int> inversePermutationTable(const std::vector<int>& permutationTable) {
    std::vector<int> inverseTable(permutationTable.size());

    // Generate inverse permutation table
    for (size_t i = 0; i < permutationTable.size(); ++i) {
        inverseTable[permutationTable[i] - 1] = i + 1;  // Inverse relationship
    }

    return inverseTable;
}
// Step 2 permutation
// Function to apply permutation to a binary string
std::string applyPermutation(const std::string& binaryInput, const std::vector<int>& permutationTable) {
    std::string permutedOutput = "";

    // Apply the permutation based on the permutation table
    for (int pos : permutationTable) {
        permutedOutput += binaryInput[pos - 1];  // Remember, positions in the table are 1-based
    }

    return permutedOutput;
}

// Function to generate a permutation table using a random walk based on the key
std::vector<int> generatePermutationByRandomWalk(const std::string& key, int size) {
    std::vector<int> permutationTable(size);
    // Initialize the permutation table with identity permutation (1-based indexing)
    for (int i = 0; i < size; ++i) {
        permutationTable[i] = i + 1;
    }

    // Use the key to guide the random walk
    size_t keyLength = key.length();
    if (keyLength == 0) {
        // If key is empty, return the identity permutation
        return permutationTable;
    }

    // Create a random number generator seeded with the key
    std::seed_seq seed(key.begin(), key.end());
    std::mt19937 rng(seed);

    // Perform the random walk
    for (int i = 0; i < size * 2; ++i) { // Multiply by 2 to increase randomness
        // Generate two positions to swap
        int pos1 = rng() % size;
        int pos2 = rng() % size;

        // Swap the elements
        std::swap(permutationTable[pos1], permutationTable[pos2]);
    }

    return permutationTable;
}



// Step 3 XOR 
// Function to perform XOR between two binary strings, repeating the key if necessary
std::string XORStrings(const std::string& binaryKey, const std::string& data) {
    std::string xorResult = "";
    size_t keyLength = binaryKey.length();
    size_t dataLength = data.length();

    // XOR each bit of binaryKey with the corresponding bit of data
    for (size_t i = 0; i < dataLength; ++i) {
        // Repeat the key if necessary
        xorResult += (binaryKey[i % keyLength] == data[i]) ? '0' : '1';
    }

    return xorResult;
}

// Function to convert a binary string to its plain text representation
std::string binaryToString(const std::string& binaryInput) {
    std::string plainText = "";

    // Ensure the binary input length is a multiple of 8
    if (binaryInput.length() % 8 != 0) {
        std::cerr << "Error: Binary input length is not a multiple of 8." << std::endl;
        return plainText;
    }

    // Process each 8-bit block
    for (size_t i = 0; i < binaryInput.length(); i += 8) {
        std::string byte = binaryInput.substr(i, 8);
        char character = static_cast<char>(std::bitset<8>(byte).to_ulong());
        plainText += character;
    }

    return plainText;
}

std::string encryptMessage(const std::string& key, const std::string& plainText, int rounds) {
    std::string result = plainText;

    for (int i = 0; i < rounds; ++i) {
        std::string roundKey = key + std::to_string(i);

        // Convert the round key and result to binary
        std::string binaryKey = stringToBinary(roundKey);
        std::string binaryText = stringToBinary(result);

        // Apply S-Box substitution
        std::string s_result = applySBox(binaryText, s_box);

        // Generate the permutation table from the round key
        // Generate the permutation table using random walk
        std::vector<int> permutationTable = generatePermutationByRandomWalk(roundKey, s_result.size());

        // Apply permutation
        std::string permutedResult = applyPermutation(s_result, permutationTable);

        // XOR operation
        std::string cipherBinary = XORStrings(binaryKey, permutedResult);

        // Prepare for the next round
        result = binaryToString(cipherBinary);
    }

    // Return the final cipher text in binary form
    return stringToBinary(result);
}


std::string decryptMessage(const std::string& key, const std::string& cipherTextBinary, int rounds) {
    std::string result = binaryToString(cipherTextBinary);

    for (int i = rounds - 1; i >= 0; --i) {
        std::string roundKey = key + std::to_string(i);

        // Convert the round key and result to binary
        std::string binaryKey = stringToBinary(roundKey);
        std::string binaryText = stringToBinary(result);

        // XOR operation to reverse
        std::string xorResult = XORStrings(binaryKey, binaryText);

        // Generate the permutation table from the round key
        // Generate the permutation table using random walk
        std::vector<int> permutationTable = generatePermutationByRandomWalk(roundKey, xorResult.size());


        // Apply inverse permutation
        std::vector<int> inverseTable = inversePermutationTable(permutationTable);
        std::string permutedResult = applyPermutation(xorResult, inverseTable);

        // Reverse the S-Box substitution
        std::string originalBinaryText = applySBox(permutedResult, inverse_s_box);

        // Prepare for the next round
        result = binaryToString(originalBinaryText);
    }

    return result;
}

std::string url_decode(const std::string &value) {
    std::ostringstream result;
    size_t length = value.length();

    for (size_t i = 0; i < length; ++i) {
        if (value[i] == '%') {
            if (i + 2 < length) {
                // Extract the next two characters
                std::string hexStr = value.substr(i + 1, 2);
                // Convert hex string to integer
                char decodedChar = static_cast<char>(std::stoi(hexStr, nullptr, 16));
                result << decodedChar;
                i += 2;
            } else {
                // Handle incomplete encoding
                result << '%';
            }
        } else if (value[i] == '+') {
            result << ' ';
        } else {
            result << value[i];
        }
    }
    return result.str();
}

std::unordered_map<std::string, std::string> parseFormData(const std::string& body) {
    std::unordered_map<std::string, std::string> formData;
    std::istringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            // Use the custom url_decode function
            key = url_decode(key);
            value = url_decode(value);
            formData[key] = value;
        }
    }

    return formData;
}

// Global variables
std::string cipherText;  // To store the encrypted message


int main() {
    crow::SimpleApp app;

    // Serve the HTML form
    CROW_ROUTE(app, "/")
    ([]() {
        return R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>Encrypt and Decrypt Message</title>
            </head>
            <body>
<h1>Encrypt a Message</h1>
<form action="/encrypt" method="POST">
    <label for="key">Enter Key:</label>
    <input type="text" id="key" name="key" required />
    <br />
    <label for="message">Enter Message:</label>
    <input type="text" id="message" name="message" required />
    <br />
    <label for="rounds">Enter Number of Rounds:</label>
    <input type="number" id="rounds" name="rounds" value="1" min="1" required />
    <br />
    <button type="submit">Encrypt</button>
</form>


           <h1>Decrypt a Message</h1>
<form action="/decrypt" method="POST">
    <label for="key">Enter Key:</label>
    <input type="text" id="key" name="key" required />
    <br />
    <label for="rounds">Enter Number of Rounds:</label>
    <input type="number" id="rounds" name="rounds" value="1" min="1" required />
    <br />
    <button type="submit">Decrypt</button>
</form>

            </body>
            </html>
        )";
    });

   CROW_ROUTE(app, "/encrypt").methods(crow::HTTPMethod::POST)
([](const crow::request& req) {
    // Parse the form data using parseFormData
    std::unordered_map<std::string, std::string> formData = parseFormData(req.body);

    auto keyIt = formData.find("key");
    auto messageIt = formData.find("message");
    auto roundsIt = formData.find("rounds");

    if (keyIt != formData.end() && messageIt != formData.end() && roundsIt != formData.end()) {
        const std::string& key = keyIt->second;
        const std::string& message = messageIt->second;
        int rounds = std::stoi(roundsIt->second);

        // Debugging outputs
        std::cout << "Parsed Key: " << key << std::endl;
        std::cout << "Parsed Message: " << message << std::endl;
        std::cout << "Parsed Rounds: " << rounds << std::endl;

        // Encrypt the message using the provided key and rounds
        std::string newCipherText = encryptMessage(key, message, rounds);
        cipherText = newCipherText;

        // Provide a link to decrypt the message
        std::ostringstream os;
        os << "<!DOCTYPE html><html><body>";
        os << "<h1>Message Encrypted</h1>";
        os << "<p>Your message has been encrypted. Use the same key and number of rounds to decrypt it.</p>";
        os << "<a href=\"/\">Go to Decryption Page</a>";
        os << "</body></html>";
        return crow::response(200, os.str());
    } else {
        return crow::response(400, "Key, message, and number of rounds must be provided.");
    }
});

  CROW_ROUTE(app, "/decrypt").methods(crow::HTTPMethod::POST)
([](const crow::request& req) {
    // Parse the form data using parseFormData
    std::unordered_map<std::string, std::string> formData = parseFormData(req.body);

    auto keyIt = formData.find("key");
    auto roundsIt = formData.find("rounds");

    if (keyIt != formData.end() && roundsIt != formData.end()) {
        const std::string& key = keyIt->second;
        int rounds = std::stoi(roundsIt->second);

        // Debugging output
        std::cout << "Parsed Key: " << key << std::endl;
        std::cout << "Parsed Rounds: " << rounds << std::endl;

        // Decrypt the message using the provided key and rounds
        std::string decryptedMessage = decryptMessage(key, cipherText, rounds);

        // Return the decrypted message
        std::ostringstream os;
        os << "<!DOCTYPE html><html><body>";
        os << "<h1>Decrypted Message</h1>";
        os << "<p>" << decryptedMessage << "</p>";
        os << "</body></html>";
        return crow::response(200, os.str());
    } else {
        return crow::response(400, "Key and number of rounds must be provided.");
    }
});



    // Run the web server
    app.port(18080).multithreaded().run();
}