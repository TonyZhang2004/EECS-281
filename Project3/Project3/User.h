#ifndef USER_H
#define USER_H

// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include <string>
#include <unordered_set>

#include "Bank.h"
#include "Transaction.h"
#include "Transaction_Comparator.h"

using namespace std;

class User {
public:
	User(string user_id, string pin,  uint64_t reg_time, uint32_t balance );
	void push_IP_record(const string& IP) { IP_record.insert(IP); }
	string get_user_id() const { return user_id; }
	string get_pin() const { return pin; }
	uint32_t get_balance() const { return balance; }
	uint64_t get_reg_time() const { return reg_time; }
	bool get_log_status() const { return IP_record.size() != 0; }
	bool find_IP(const string& IP) const { return IP_record.find(IP) != IP_record.end(); }
	void remove_IP(const string& IP) { IP_record.erase(IP); }
	void incr_balance(uint32_t amt) { balance += amt; }
	void decr_balance(uint32_t amt) { balance -= amt; }
	void push_incoming(Transaction* trans);
	void push_outgoing(Transaction* trans);
	void incr_total_transactions() { total_transactions++; }
	void summary();
	void display_transaction(Transaction* trans);
private:
	// store the user history using some data structure
	vector<Transaction*> incoming;
	vector<Transaction*> outgoing;
	unordered_set<string> IP_record;
	string user_id;
	string pin;
	uint64_t reg_time;
	uint32_t balance;
	uint32_t total_transactions;
	uint32_t total_incoming;
	uint32_t total_outgoing;
};

#endif // !USER_H

