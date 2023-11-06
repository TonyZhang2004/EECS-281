// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

#include "parse_cmd.h"
#include "User.h"
#include "Transaction.h"
#include "Transaction_Comparator.h"

using namespace std;

class User;

class Bank {
public:
	Bank(int argc, char* argv[]);
	~Bank();
	void input_reg();
	uint64_t parse_time(string& str);
	void create_user(string& line);
	void process_comment();
	void process_login();
	void process_logout();
	void process_place();
	void list_transaction();
	void report_revenue();
	void customer_history();
	void summarize_day();
	void calc_time_interval(uint64_t start_time, uint64_t end_time);
	void init();
	bool check_trans(uint64_t timestamp, const string& IP, const string& sender,
		const string& recipient, uint64_t exec_date);
	void execute_transactions();
	void display_transaction(Transaction* trans);
	// sender_fee, recipient_fee
	pair<uint32_t, uint32_t> calc_fee(const User* sender, Transaction* trans);
private:
	unordered_map<string, User*> master_map;
	priority_queue<Transaction*, deque<Transaction*>, Transaction_Comparator> pq;
	// how to maintain the sortedness property? 
	// how to use my custom comparator in lower_bound & sort?
	vector<Transaction*> transaction_list;
	Transaction latest_trans;
	Options option;
	uint32_t curr_trans_id;
};

#endif // !BANK_H
