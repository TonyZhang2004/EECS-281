// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#ifndef TRANSACTION_COMPARATOR_H
#define TRANSACTION_COMPARATOR_H

#include "Transaction.h"

class Transaction_Comparator {
public:
	bool operator()(Transaction* trans_a, Transaction* trans_b) {
		if (trans_a->get_exec_date() == trans_b->get_exec_date()) {
			return trans_b->get_trans_ID() < trans_a->get_trans_ID();
		}
		return trans_a->get_exec_date() > trans_b->get_exec_date();
	}
};

#endif // !TRANSACTION_COMPARATOR_H

