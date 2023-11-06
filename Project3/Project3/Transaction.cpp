// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#include "Transaction.h"

Transaction::Transaction(string IP, string sender, string recipient,
	uint64_t timestamp, uint64_t exec_date, uint32_t amount, uint32_t trans_id, char mode) :
	IP(IP), sender(sender), recipient(recipient), timestamp(timestamp), 
	exec_date(exec_date), amount(amount), trans_id(trans_id), mode(false) {
	if (mode == 'o') {
		this->mode = false;
	}
	else {
		this->mode = true;
	}
}

Transaction::Transaction() : IP(""), sender(""), recipient(""), timestamp(0),
exec_date(0), amount(0), trans_id(0), mode(false) {
	
}