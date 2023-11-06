// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

class Transaction {
public:
	Transaction();
	Transaction(string IP, string sender, string recipient,
		uint64_t timestamp, uint64_t exec_date, uint32_t amount, uint32_t trans_id, char mode);
	uint64_t get_timestamp() const { return timestamp; }
	uint64_t get_exec_date() const { return exec_date; }
	uint32_t get_trans_ID() const { return trans_id; }
	uint32_t get_amount() const { return amount; }
	string get_sender() const { return sender; }
	string get_recipient() const { return recipient; }
	bool get_mode() const { return mode; }
private:
	string IP;
	string sender;
	string recipient;
	uint64_t timestamp;
	uint64_t exec_date;
	uint32_t amount;
	uint32_t trans_id;
	// false == o, true == s
	bool mode;
};

#endif // !TRANSACTION_H

