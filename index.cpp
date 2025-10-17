#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include "SHA256.h"

using namespace std;

// Transaction
class Transaction {
public:
    string sender;
    string receiver;
    double amount;

    Transaction(string s, string r, double a) : sender(s), receiver(r), amount(a) {}

    void printTransaction() {
        cout << sender << " -> " << receiver << " : " << amount << " coins\n";
    }
};

// Block 
class Block {
public:
    int index;
    vector<Transaction> transactions;
    string previousHash;
    string blockHash;
    time_t timestamp;

    // Constructor
    Block(int idx, vector<Transaction> txs, string prevHash) {
        index = idx;
        transactions = txs;
        previousHash = prevHash;
        timestamp = time(nullptr);
        blockHash = calculateHash();
    }

    // Calculate block hash
    string calculateHash() {
        string data = to_string(index) + previousHash + to_string(timestamp);
        for (auto &t : transactions)
            data += t.sender + t.receiver + to_string(t.amount);
        return SHA256::hash(data);
    }

    // Print block info
    void printBlock() {
        cout << "Block " << index
             << " [Hash: " << blockHash.substr(0,10) << "... , Prev: "
             << previousHash.substr(0,10) << "..., Time: " << ctime(&timestamp) << "]\n";
        for (auto &t : transactions)
            t.printTransaction();
        cout << endl;
    }
};


// Blockchain 
class Blockchain {
public:
    vector<Block> chain;

    Blockchain() {
        Transaction genesisTx("Genesis", "Genesis", 0);
        Block genesisBlock(0, {genesisTx}, "0");
        chain.push_back(genesisBlock);
    }

    void addBlock(vector<Transaction> txs) {
        string prevHash = chain.back().blockHash;
        Block newBlock(chain.size(), txs, prevHash);
        chain.push_back(newBlock);
    }

    bool isValid() {
        for (size_t i = 1; i < chain.size(); i++) {
            if (chain[i].previousHash != chain[i-1].blockHash) return false;
            if (chain[i].blockHash != chain[i].calculateHash()) return false;
        }
        return true;
    }

    void printChain() {
        for (auto &b : chain) b.printBlock();
    }

    void mineRandomBlocks(int numBlocks, int txPerBlock) {
        string names[] = {"Alice","Bob","Charlie","Bharath","Sharath","Eve","Mallory"};
        int n = sizeof(names)/sizeof(names[0]);
        srand(time(nullptr));

        for (int i = 0; i < numBlocks; i++) {
            vector<Transaction> txs;
            for (int j = 0; j < txPerBlock; j++) {
                string sender = names[rand() % n];
                string receiver;
                do { receiver = names[rand() % n]; } while (receiver == sender);
                double amount = rand() % 100 + 1;
                txs.push_back(Transaction(sender, receiver, amount));
            }
            addBlock(txs);
        }
    }
};

// MAIN 
int main() {
    Blockchain myChain;
    vector<Transaction> pendingTransactions;

    int choice = 0;
    while (true) {
        cout << "\n==== Blockchain Menu ====\n";
        cout << "1. Add Transaction\n";
        cout << "2. Mine Block\n";
        cout << "3. Print Blockchain\n";
        cout << "4. Validate Blockchain\n";
        cout << "5. Auto-simulate 10 Random Blocks\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string sender, receiver;
            double amount;
            cout << "Sender: "; getline(cin, sender);
            cout << "Receiver: "; getline(cin, receiver);
            cout << "Amount: "; cin >> amount; cin.ignore();
            pendingTransactions.push_back(Transaction(sender, receiver, amount));
            cout << "Transaction added!\n";
        }
        else if (choice == 2) {
            if (pendingTransactions.empty()) cout << "No transactions to mine!\n";
            else {
                myChain.addBlock(pendingTransactions);
                pendingTransactions.clear();
                cout << "Block mined and added!\n";
            }
        }
        else if (choice == 3) {
            cout << "\n=== Full Blockchain ===\n";
            myChain.printChain();
        }
        else if (choice == 4) {
            cout << "Blockchain is " << (myChain.isValid() ? "valid \n" : "invalid \n");
        }
        else if (choice == 5) {
            myChain.mineRandomBlocks(10, 2); 
            cout << "10 random blocks added!\n";
        }
        else if (choice == 6) {
            cout << "Exiting...\n";
            break;
        }
        else cout << "Invalid choice. Try again.\n";
    }

    return 0;
}
