// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "Bank.h"

Bank::Bank(int argc, char* argv[]) {
	get_mode(argc, argv, this->option);
	curr_trans_id = 0;
}

Bank::~Bank() {
	for (pair<string, User*> mp : master_map) {
		delete mp.second;
	}
	for (Transaction* trans : transaction_list) {
		delete trans;
	}
}

void Bank::input_reg() {
	ifstream source;
	source.open(this->option.filename);
	string line;
	while (getline(source, line)) {
		create_user(line);
	}
}

uint64_t Bank::parse_time(string& str) {
	string token;
	uint64_t res = 0;
	size_t pos = 0;
	uint32_t val = 0;
	while ((pos = str.find(":")) != string::npos) {
		token = str.substr(0, pos);
		val = 0;
		for (size_t i = 0; i < token.size(); i++) {
			val = val * 10 + (token[i] - '0');
		}
		str.erase(0, pos + 1); // erase the digits and the colon
		res = res * 100ULL + val;
	}
	val = 0;
	for (size_t i = 0; i < str.size(); i++) {
		val = val * 10 + (str[i] - '0');
	}
	res = res * 100ULL + val;
	return res;
}

void Bank::create_user(string& line) {
	size_t pos = 0;
	string str;
	string user_id;
	string pin;
	uint32_t balance = 0;
	uint64_t reg_time = 0;

	// timestamp
	pos = line.find("|");
	str = line.substr(0, pos);
	reg_time = parse_time(str);
	line.erase(0, pos + 1);

	// user_id
	pos = line.find("|");
	user_id = line.substr(0, pos);
	line.erase(0, pos + 1);

	// pin
	pos = line.find("|");
	pin = line.substr(0, pos);
	line.erase(0, pos + 1);

	// balance
	for (size_t i = 0; i < line.size(); i++) {
		balance = balance * 10 + (line[i] - '0');
	}

	User* res = new User(user_id, pin, reg_time, balance);
	this->master_map.insert({ user_id, res });
}

void Bank::process_comment() {
	string junk;
	getline(cin, junk);
}

void Bank::process_login() {
	// ex. login mertg 080400 231.43.171.61
	// process the str input
	string user_id;
	string pin;
	string IP;
	cin >> user_id >> pin >> IP;
	string junk;
	getline(cin, junk);
	// attempt to login
	auto iter = master_map.find(user_id);
	if (iter != master_map.end() && iter->second->get_pin() == pin) {
		if (this->option.verbose)
			cout << "User " << user_id << " logged in.\n";
		// login processing...
		iter->second->push_IP_record(IP);
	}
	else {
		if (this->option.verbose)
			cout << "Failed to log in " << user_id << ".\n";
	}
}

void Bank::process_logout() { 
	// out mertg 231.43.171.61
	// process the str input
	
	string user_id;
	string IP;
	cin >> user_id >> IP;
	string junk;
	getline(cin, junk);
	auto iter = master_map.find(user_id);
	if (iter == master_map.end() || !iter->second->find_IP(IP)) {
		if (option.verbose) {
			cout << "Failed to log out " << user_id << ".\n";
		}
		return;
	}
	else {
		iter->second->remove_IP(IP);
		if (option.verbose) {
			cout << "User " << user_id << " logged out.\n";
		}
	}
}

void Bank::process_place() {
	// place 08:03:01:40:22:34 134.31.78.43 mertg paoletti 2000 09:03:01:40:22:34 s
	// parse input str
	string junk;
	string tmp;
	string IP;
	string sender;
	string recipient;
	string amt; // parse amount into uint32_t
	string ex_date;
	char mode;
	cin >> tmp >> IP >> sender >> recipient >> amt >> ex_date >> mode;
	getline(cin, junk);
	uint64_t exec_date = parse_time(ex_date);
	uint64_t timestamp = parse_time(tmp);
	uint32_t amount = 0;
	for (size_t i = 0; i < amt.size(); i++) {
		amount = amount * 10 + (amt[i] - '0');
	}

	bool valid = check_trans(timestamp, IP, sender, recipient, exec_date);
	if (!valid) {
		return;
	}
	// place the transaction
	Transaction* trans = new Transaction(IP, sender, recipient, timestamp,
		exec_date, amount, curr_trans_id, mode);
	curr_trans_id++;
	while (!pq.empty() && pq.top()->get_exec_date() < timestamp) {
		execute_transactions();
	}
	pq.push(trans);
	if (option.verbose) {
		cout << "Transaction placed at " << timestamp << ": $" << amount <<
			" from " << sender << " to " << recipient << " at " << exec_date << ".\n";
	}
}

bool Bank::check_trans(uint64_t timestamp, const string& IP, const string& sender,
	const string& recipient, uint64_t exec_date) {
	// checking errors
	if (latest_trans.get_timestamp() > timestamp) { // 1. timestamp earlier than the previous place
		cerr << "A place command with a timestamp earlier than the previous place.\n";
		exit(1);
	}
	if (exec_date < timestamp) { // 2. exec_date before its own timestamp
		cerr << "A place command which contains an execution date before its own timestamp.\n";
		exit(1);
	}
	if (exec_date - timestamp > 3000000ULL) { // 3. exec_date is 3 days within the placement time
		if (option.verbose) {
			cout << "Select a time less than three days in the future.\n";
		}
		return false;
	}
	auto sender_pair = master_map.find(sender);
	auto recipient_pair = master_map.find(recipient);
	if (sender_pair == master_map.end()) { // 4. sender ID invalid
		if (option.verbose) {
			cout << "Sender " << sender << " does not exist.\n";
		}
		return false;
	}
	if (recipient_pair == master_map.end()) { // 5. recipient ID invalid
		if (option.verbose) {
			cout << "Recipient " << recipient << " does not exist.\n";
		}
		return false;
	}
	if (exec_date < sender_pair->second->get_reg_time() ||
		exec_date < recipient_pair->second->get_reg_time()) { // 6. exec_date not later than both parties' reg_time
		if (option.verbose) {
			cout << "At the time of execution, sender and/or recipient have not registered.\n";
		}
		return false;
	}
	if (!sender_pair->second->get_log_status()) { // 7. sender does not have an active user session
		if (option.verbose) {
			cout << "Sender " << sender << " is not logged in.\n";
		}
		return false;
	}
	if (!sender_pair->second->find_IP(IP)) { // 8. IP does not match
		if (option.verbose) {
			cout << "Fraudulent transaction detected, aborting request.\n";
		}
		return false;
	}
	return true;
}

void Bank::execute_transactions() {
	Transaction* trans = pq.top();
	pq.pop();
	User* sender = master_map.find(trans->get_sender())->second;
	User* recipient = master_map.find(trans->get_recipient())->second;
	pair<uint32_t, uint32_t> fees = calc_fee(sender, trans);
	uint32_t money = trans->get_amount();
	if (sender->get_balance() < trans->get_amount() + fees.first
		|| recipient->get_balance() < fees.second) {
		if (option.verbose) {
			cout << "Insufficient funds to process transaction "
				<< trans->get_trans_ID() << ".\n";
		}
		return;
	}
	sender->decr_balance(money + fees.first);
	sender->push_outgoing(trans);
	sender->incr_total_transactions();
	recipient->incr_balance(money - fees.second);
	recipient->push_incoming(trans);
	recipient->incr_total_transactions();
	if (option.verbose) {
		cout << "Transaction executed at " << trans->get_exec_date() << ": $" 
			<< trans->get_amount() << " from " << sender->get_user_id() 
			<< " to " << recipient->get_user_id() << ".\n";
	}
	transaction_list.push_back(trans);
}

void Bank::list_transaction() {
	// l 08:03:01:40:22:36 09:03:01:40:22:34
	string tmp;
	cin >> tmp;
	uint64_t start_time = parse_time(tmp);
	cin >> tmp;
	uint64_t end_time = parse_time(tmp);
	getline(cin, tmp);
	
	auto iter = transaction_list.begin();
	uint32_t cnt = 0;
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < start_time) {
		iter++;
	}
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < end_time) {
		Transaction* trans = *iter;
		cout << trans->get_trans_ID() << ": " << trans->get_sender() << " sent ";
		if (trans->get_amount() == 1) {
			cout << "1 dollar ";
		}
		else {
			cout << trans->get_amount() << " dollars ";
		}
		cout << "to " << trans->get_recipient() << " at " << trans->get_exec_date() << ".\n";
		iter++;
		cnt++;
	}
	if (cnt == 1) {
		cout << "There was " << 1 << " transaction that was placed between time "
			<< start_time << " to " << end_time << ".\n";
		return;
	}
	cout << "There were " << cnt << " transactions that were placed between time "
		<< start_time << " to " << end_time << ".\n";
}

void Bank::calc_time_interval(uint64_t start_time, uint64_t end_time) {
	uint64_t period = end_time - start_time;
	uint64_t second = period % 100;
	period /= 100;
	uint64_t minute = period % 100;
	period /= 100;
	uint64_t hour = period % 100;
	period /= 100;
	uint64_t day = period % 100;
	period /= 100;
	uint64_t month = period % 100;
	period /= 100;
	uint64_t year = period % 100;
	period /= 100;

	if (year == 1) {
		cout << "1 year ";
	}
	else if (year > 1) {
		cout << year << " years ";
	}
	if (month == 1) {
		cout << "1 month ";
	}
	else if (month > 1) {
		cout << month << " months ";
	}
	if (day == 1) {
		cout << "1 day ";
	}
	else if (day > 1) {
		cout << day << " days ";
	}
	if (hour == 1) {
		cout << "1 hour ";
	}
	else if (hour > 1) {
		cout << hour << " hours ";
	}
	if (minute == 1) {
		cout << "1 minute ";
	}
	else if (minute > 1 && second != 0) {
		cout << minute << " minutes ";
	}
	else if (minute > 1 && second == 0) {
		cout << minute << " minutes";
	}
	if (second == 1) {
		cout << "1 second";
	}
	else if (second > 1) {
		cout << second << " seconds";
	}
}

void Bank::display_transaction(Transaction* trans) {
	if (trans->get_amount() != 1) {
		cout << trans->get_trans_ID() << ": " << trans->get_sender() << " sent "
			<< trans->get_amount() << " dollars to " << trans->get_recipient() << " at "
			<< trans->get_exec_date() << ".\n";
		return;
	}
	cout << trans->get_trans_ID() << ": " << trans->get_sender() << " sent "
		<< "1 dollar to " << trans->get_recipient() << " at "
		<< trans->get_exec_date() << ".\n";
}

void Bank::report_revenue() {
	// r 08:03:01:40:22:36 08:23:07:11:62:34
	string tmp;
	cin >> tmp;
	uint64_t start_time = parse_time(tmp);
	cin >> tmp;
	uint64_t end_time = parse_time(tmp);
	getline(cin, tmp);

	uint32_t revenue = 0;
	auto iter = transaction_list.begin();
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < start_time) {
		iter++;
	}
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < end_time) {
		pair<uint32_t, uint32_t> fees = calc_fee(master_map.find((*iter)->get_sender())->second, *iter);
		revenue += (fees.first + fees.second);
		iter++;
	}
	cout << "281Bank has collected " << revenue << " dollars in fees over ";
	calc_time_interval(start_time, end_time);
	cout << ".\n";
}

void Bank::customer_history() {
	// h mertg
	string junk;
	string user_id;
	cin >> user_id;
	getline(cin, junk);

	auto iter = master_map.find(user_id);
	if (iter == master_map.end()) {
		cout << "User " << user_id << " does not exist.\n";
		return;
	}

	(*iter).second->summary();
}

void Bank::summarize_day() {
	// s 08:03:02:30:22:36
	string timestamp_str;
	cin >> timestamp_str;
	
	string junk;
	getline(cin, junk);
	uint64_t timestamp = parse_time(timestamp_str);
	uint64_t start_time = timestamp - timestamp % 1000000ULL;
	uint64_t end_time = start_time + 1000000ULL;
	
	cout << "Summary of [" << start_time << ", " << end_time << "):\n";
	uint32_t revenue = 0;
	uint32_t cnt = 0;
	auto iter = transaction_list.begin();
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < start_time) {
		iter++;
	}
	while (iter != transaction_list.end() && (*iter)->get_exec_date() < end_time) {
		pair<uint32_t, uint32_t> fees = calc_fee(master_map.find((*iter)->get_sender())->second, *iter);
		display_transaction(*iter);
		revenue += (fees.first + fees.second);
		iter++;
		cnt++;
	}

	if (cnt == 1) {
		cout << "There was a total of 1 transaction, ";
	}
	else {
		cout << "There were a total of " << cnt << " transactions, ";
	}
	cout << "281Bank has collected " << revenue << " dollars in fees.\n";
}

pair<uint32_t, uint32_t> Bank::calc_fee(const User* sender, Transaction* trans) {
	uint32_t fee = trans->get_amount() / 100;
	fee = max(fee, 10U);
	fee = min(fee, 450U);
	if (trans->get_exec_date() - sender->get_reg_time() > 50000000000ULL) { // long standing customer
		fee = (fee * 3) / 4;
	}
	if (trans->get_mode()) { // share mode
		return make_pair(fee - fee / 2, fee / 2);
	}
	else { // only the sender pays the fee
		return make_pair(fee, 0);
	}
}

void Bank::init() {
	input_reg();
	string prompt;
	// process operations
	while (cin >> prompt) {
		// cout << prompt << "\n";
		if (prompt[0] == '#') {
			process_comment();
		}
		else if (prompt[0] == 'l') {
			process_login();
		}
		else if (prompt[0] == 'o') {
			process_logout();
		}
		else if (prompt[0] == 'p') {
			process_place();
		}
		else if (prompt[0] == '$') {
			break;
		}
	}
	// execute the rest transactions
	while (!pq.empty()) {
		execute_transactions();
	}
	// sort the transaction list according to exec_time
	// process queries
	while (cin >> prompt) {
		
		if (prompt[0] == 'l') {
			list_transaction();
		}
		if (prompt[0] == 'r') {
			report_revenue();
		}
		if (prompt[0] == 'h') {
			customer_history();
		}
		if (prompt[0] == 's') {
			summarize_day();
		}
	}
}