#ifndef BERT_QSET_H
#define BERT_QSET_H

#include "QHelper.h"
#include <unordered_set>

namespace qedis
{

using QSet = std::unordered_set<QString,
        my_hash,
        std::equal_to<QString> >;

QObject CreateSetObject();

size_t   SScanKey(const QSet& qset, size_t cursor, size_t count, std::vector<QString>& res);
    
}

#endif

