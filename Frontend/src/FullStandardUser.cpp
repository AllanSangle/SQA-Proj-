#include "FullStandardUser.h"

bool FullStandardUser::canBuy() const {
    return true;
}

bool FullStandardUser::canSell() const {
    return true;
}

bool FullStandardUser::isAdmin() const {
    return false;
}

std::string FullStandardUser::getUserType() const {
    return "FS";
}
