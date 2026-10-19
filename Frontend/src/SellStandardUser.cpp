#include "SellStandardUser.h"

bool SellStandardUser::canBuy() const {
    return false;
}

bool SellStandardUser::canSell() const {
    return true;
}

bool SellStandardUser::isAdmin() const {
    return false;
}

std::string SellStandardUser::getUserType() const {
    return "SS";
}
