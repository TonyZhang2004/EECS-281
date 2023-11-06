// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#include "User.h"

using namespace std;

User::User(string _user_id, string _pin, uint64_t _reg_time, uint32_t _balance)
: user_id(_user_id), pin(_pin), reg_time(_reg_time), balance(_balance), total_transactions(0),
	total_incoming(0), total_outgoing(0) {
}

void User::push_incoming(Transaction* trans) {
	total_incoming++;
	incoming.push_back(trans);
}

void User::push_outgoing(Transaction* trans) {
	total_outgoing++;
	outgoing.push_back(trans);
}

void User::display_transaction(Transaction* trans) {
	if (trans->get_amount() > 1) {
		cout << trans->get_trans_ID() << ": " << trans->get_sender() << " sent "
			<< trans->get_amount() << " dollars to " << trans->get_recipient() << " at "
			<< trans->get_exec_date() << ".\n";
		return;
	}
	cout << trans->get_trans_ID() << ": " << trans->get_sender() << " sent "
		 << "1 dollar to " << trans->get_recipient() << " at "
		<< trans->get_exec_date() << ".\n";
}

void User::summary() {
	cout << "Customer " << user_id << " account summary:\n";
	cout << "Balance: $" << balance << '\n';
	cout << "Total # of transactions: " << total_transactions << "\n";
	cout << "Incoming " << total_incoming << ":\n";
	for (size_t i = max(0, static_cast<int32_t>(incoming.size() - 10)); i < incoming.size(); i++) {
		display_transaction(incoming[i]);
	}
	cout << "Outgoing " << total_outgoing << ":\n";
	for (size_t i = max(0, static_cast<int32_t>(outgoing.size() - 10)); i < outgoing.size(); i++) {
		display_transaction(outgoing[i]);
	}
}

