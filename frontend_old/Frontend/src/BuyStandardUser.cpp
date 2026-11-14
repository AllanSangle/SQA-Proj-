#include "BuyStandardUser.h"

//sets buy standard users permissions
bool BuyStandardUser::canBuy() const {
    return true;
}

bool BuyStandardUser::canSell() const {
    return false;
}

bool BuyStandardUser::isAdmin() const {
    return false;
}

std::string BuyStandardUser::getUserType() const {
    return "BS";
}
