#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <vector>

// Node class for the Huffman Tree
class HuffmanNode {
public:
  char character;
  unsigned int frequency;
  std::shared_ptr<HuffmanNode> left;
  std::shared_ptr<HuffmanNode> right;

  HuffmanNode(char ch, unsigned int freq)
      : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

  // Compare nodes based on frequency
  bool operator>(const HuffmanNode &other) const {
    return frequency > other.frequency;
  }
};

// Function to print the Huffman codes from the root of the Huffman Tree
void printHuffmanCodes(const std::shared_ptr<HuffmanNode> &root,
                       const std::string &code,
                       std::map<char, std::string> &huffmanCodeMap) {
  if (!root)
    return;
  // '#' is our "blank character" text cant contain these
  if (root->character != '#') {
    std::cout << root->character << ": " << code << '\n';
    huffmanCodeMap[root->character] = code;
  }
  printHuffmanCodes(root->left, code + "0", huffmanCodeMap);
  printHuffmanCodes(root->right, code + "1", huffmanCodeMap);
}

// Function to print the Huffman Tree structure
void printHuffmanTree(const std::shared_ptr<HuffmanNode> &root,
                      const std::string &prefix = "", bool isLeft = true) {
  if (!root)
    return;
  std::cout << prefix;
  std::cout << (isLeft ? "├──" : "└──");
  if (root->character != '#') {
    std::cout << "\'" << root->character << "\' (" << root->frequency << ")\n";
  } else {
    std::cout << "# (" << root->frequency << ")\n";
  }
  printHuffmanTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
  printHuffmanTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
}

int main() {
  std::string s =
      "Come Nerevar, friend or traitor, come. Come and look upon the Heart and "
      "Akulakahn, and bring Wraithguard, I have need of it. Come to the Heart "
      "chamber, I wait for you there, where we last met, countless ages ago. "
      "Come to me through fire and war, I welcome you! Welcome Moon-and-Star, "
      "I have prepared a place for you! Come, bring Wraithguard to the Heart "
      "chamber, together, let us free the cursed false gods! Welcome Nerevar, "
      "together we shall speak for the law and the land and drive the mongrel "
      "dogs of the Empire from Morrowind! Is this how you honor the 6th house "
      "and the tribe unmourned? Come to me openly, and not by stealth. Dagoth "
      "Ur welcomes you Nerevar, my old friend... but to this place where "
      "destiny is made, why have you come unprepared? Welcome, Moon-and-Star, "
      "to this place where YOUR destiny is made. What a fool you are, I'm a "
      "god! How can you kill a god? What a grand and intoxicating innocence! "
      "How could you be so naive? There is no escape, no recall or "
      "intervention can work in this place! Come! Lay down your weapons! It is "
      "not too late for my mercy...";

  std::map<char, unsigned int> frequencyMap;
  for (char c : s) {
    frequencyMap[c]++;
  }

  auto cmp = [](const std::shared_ptr<HuffmanNode> &left,
                const std::shared_ptr<HuffmanNode> &right) {
    return left->frequency > right->frequency;
  };
  std::priority_queue<std::shared_ptr<HuffmanNode>,
                      std::vector<std::shared_ptr<HuffmanNode>>, decltype(cmp)>
      pq(cmp);

  for (const auto &[ch, freq] : frequencyMap) {
    pq.push(std::make_shared<HuffmanNode>(ch, freq));
  }

  while (pq.size() > 1) {
    auto left = pq.top();
    pq.pop();
    auto right = pq.top();
    pq.pop();
    auto combinedNode =
        std::make_shared<HuffmanNode>('#', left->frequency + right->frequency);
    combinedNode->left = left;
    combinedNode->right = right;
    pq.push(combinedNode);
  }

  auto root = pq.top();

  std::cout << "Huffman Codes:\n";
  std::map<char, std::string> huffmanCodeMap;
  printHuffmanCodes(root, "", huffmanCodeMap);

  std::cout << "\nHuffman Tree:\n";
  printHuffmanTree(root);

  // Encoding the original text
  std::string encodedText;
  for (char c : s) {
    encodedText += huffmanCodeMap[c];
  }

  // Calculating the size of the original and compressed text
  auto originalSize = s.size() * 8;         // size in bits
  auto compressedSize = encodedText.size(); // size in bits

  std::cout << "Original text:\n" << s << '\n';
  std::cout << "Compressed text:\n" << encodedText << '\n';
  std::cout << "\nOriginal text size (in bits): " << originalSize << '\n';
  std::cout << "Compressed text size (in bits): " << compressedSize << '\n';
  std::cout << "Original text bits per character: 8 (duh)" << '\n';
  std::cout << "Compressed text bits per character: "
            << (double)compressedSize / (double)s.size() << '\n';
  auto compressionRatio = (double)compressedSize / (double)originalSize;
  std::cout << "Compression Ratio: " << compressionRatio << '\n';

  return 0;
}
